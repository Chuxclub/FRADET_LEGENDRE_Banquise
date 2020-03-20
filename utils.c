#include <stdlib.h>

#include "utils.h"

/* ====================================== */
/* ============== BOOLEANS ============== */
/* ====================================== */

/*
    Auteur(e)(s): Florian Legendre
    Utilité: IsCaseAvailable() est utilisée pour les déplacements des joueurs et objets movibles (comme les glacons). Elle controle qu'aucun
             objet ou element de terrain n'est present sur la case à l'exception de l'objet reserved qui est utilisé par addHammers() pour éviter que
             des objets ne soient placés dans le rayon de rotation du marteau. L'objet reserved ne doit pas empêcher le déplacement d'un joueur ou
             objet movible, cette condition la distingue donc de IsPlacementAvailable() qui est utilisée pour le placement d'objets et ne tolère
             aucun objet déjà présent (y compris les objets "reserved" donc)
    Fonctionnement: Simple renvoit de 1 si la condition est satisfaite, 0 sinon.
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: /
*/
int IsCaseAvailable(T_case banquise_case)
{
    return ((banquise_case.ground == ice && (banquise_case.object == NULL || (banquise_case.object != NULL && banquise_case.object->object_type == reserved))) && (banquise_case.flag == no_flag && banquise_case.player == NULL));
}

int IsPlacementAvailable(T_case banquise_case)
{
    return ((banquise_case.ground == ice && (banquise_case.object == NULL)) && (banquise_case.flag == no_flag && banquise_case.player == NULL));
}


/*
    Auteur(e)(s): Florian Legendre
    Utilité: Verifie qu'il n'y a aucun objet directement autour d'une position (ie rayon de 1 autour) passee en parametre
    Fonctionnement: Definie une ligne de depart, une ligne d'arrivee, meme chose pour les colonnes et controle toutes les
                    cases comprises dans cette aire ainsi delimitee
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: Passer en parametre directement une position au lieu d'un int line et int col
*/
int AreSurroundingsAvailable(T_banquise *banquise, int line, int col)
{
    int res = 1;
    int start_line = line-1;
    int stop_line = line+1;
    int start_col = col-1;
    int stop_col = col+1;

    for(int current_line = start_line; current_line <= stop_line; current_line++)
    {
        for(int current_col = start_col; current_col <= stop_col; current_col++)
        {
            if(!(IsInbound(banquise->sizeB, current_line, current_col) && IsPlacementAvailable(banquise->grid[current_line][current_col])))
                res = 0;
        }
    }

    return res;
}


/*
    Auteur(e)(s): Florian Legendre
    Utilité: Placer une zone où aucun autre objet ne pourra etre dorenavant place. Est utile au marteau qui a besoin que son
             rayon de rotation soit libre. Mais pourrait etre utile pour de futurs autres objets!
    Fonctionnement: Presque la meme chose que precedemment avec en plus l'initialisation d'un objet reserved_case
                    qui sera copiée dans toutes les cases autour de l'objet qu'on souhaite placer (pour eviter qu'un reserved_case
                    soit copie a l'endroit ou on veut placer le manche de notre marteau on ajout (... && (!(current_col == col && current_line == line))
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: 1) Passer en parametre directement une position au lieu d'un int line et int col
                                       2) Supprimer la condition (... && (!(current_col == col && current_line == line)) et simplement
                                          ecraser les "reserved" au moment de l'ajout du marteau a l'endroit desire en utilisant
                                          IsCaseAvailable() au lieu de IsPlacementAvailable()
*/
void PlaceReservedZone(T_banquise *banquise, int line, int col)
{
    int start_line = line-1;
    int stop_line = line+1;
    int start_col = col-1;
    int stop_col = col+1;
    T_object *reserved_case = (T_object *) malloc(sizeof(T_object));
    reserved_case->object_type = reserved;
    reserved_case->flake = NULL;
    reserved_case->spring = NULL;
    reserved_case->hammer_handle = NULL;
    reserved_case->hammer_head = NULL;
    reserved_case->trap = NULL;

    for(int current_line = start_line; current_line <= stop_line; current_line++)
    {
        for(int current_col = start_col; current_col <= stop_col; current_col++)
        {
            if(IsInbound(banquise->sizeB, current_line, current_col) && IsCaseAvailable(banquise->grid[current_line][current_col]) && (!(current_col == col && current_line == line)))
            {
                banquise->grid[current_line][current_col].object = reserved_case;
            }
        }
    }
}


/*
    Auteur(e)(s): Florian Legendre
    Utilité: Ameliore la lisibilite des procedures testant la presence d'objets donnes
    Fonctionnement: Simplement renvoit d'un test booleen
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: /
*/
int IsFlake(T_case banquise_case)
{
    return (banquise_case.object->object_type == flake);
}

int IsFlagB(T_case banquise_case)
{
    return (banquise_case.flag == B);
}

int IsTrap(T_case banquise_case)
{
    return (banquise_case.object->object_type == trap);
}

int IsPlayer(T_case banquise_case)
{
    return (banquise_case.player != NULL);
}



/*
    Auteur(e)(s): Florian Legendre

    Utilité: Verifie si un joueur est a portee du marteau lorsque celui-ci est en cours de rotation. Si c'est le cas, renvoit
             la valeur du test booleen qui menera, dans une autre fonction que celle-ci, a la mort du joueur tue par le passage
             du marteau. On sauvegarde, de plus, dans un pointeur, l'identifiant du joueur qu'on a identifie comme etant a portee
             pour pouvoir le tuer ulterieurement.

    Fonctionnement: Une variable booleene found_player est initialisee a false. Si le joueur est detecte sa valeur sera irremediablement
                    modifie a true. Pour detecter le joueur on recupere les matrices d'etats-transformations horaire et antihoraire.  On recupere
                    egalement l'etat du marteau et son sens de rotation.

                    Dans tous les cas il y a toujours deux positions a verifier: celle ou la tete
                    du marteau arrivera (cf. "check_pos2") et la case dans le coin entre la tete du marteau courante et check_pos2 (cf. "check_pos1").

                    Dans tous les cas check_pos1 peut être calculee avec une simple transition de la tete du marteau par le vecteur qu'il transporte
                    (le "vector_carrier"). Check_pos2 doit etre calculee en deux cas: le cas horaire et le cas antihoraire. Dans les deux cas on applique
                    la matrice d'etat-transformation correspondante au sens de rotation du marteau et son etat courant et on obtient ainsi check_pos2.

                    Une fois check_pos1 et check_pos2 calcules on controle la presence du joueur sur chacune de ces cases et si c'est le cas on sauvegarde
                    l'identifiant du joueur concernant et on passe found_player a vrai.

    Complexité en temps (au pire): O(1)

    Hypothèse d'amélioration possible: /
*/
int IsPlayerInRange(T_hammer_head *hammer_head, T_banquise *banquise, int *player_id_in_range)
{
    T_pos H_pos = {hammer_head->pos.line, hammer_head->pos.col};
    T_vector H_vector_carrier = hammer_head->vector_carrier;
    int H_state = hammer_head->state;
    T_vector clockw[4] = {{-1, 1},{1, 1},{1, -1},{-1, -1}};
    T_vector anticlockw[4] = {{1, 1},{1, -1},{-1, -1},{-1, 1}};
    bool found_player = false;

    //Defining positions in range of hammer's head
    T_pos check_pos1 = translate_point(H_pos, H_vector_carrier);

    if(hammer_head->rot_dir == clockwise)
    {
        T_pos check_pos2 = translate_point(H_pos, clockw[H_state]);

        if(IsPlayer(banquise->grid[check_pos1.line][check_pos1.col]))
        {
            found_player = true;
            *player_id_in_range = banquise->grid[check_pos1.line][check_pos1.col].player->id;
        }

        else if(IsPlayer(banquise->grid[check_pos2.line][check_pos2.col]))
        {
            found_player = true;
            *player_id_in_range = banquise->grid[check_pos2.line][check_pos2.col].player->id;
        }
    }

    else
    {
        T_pos check_pos2 = translate_point(H_pos, anticlockw[H_state]);

        if(IsPlayer(banquise->grid[check_pos1.line][check_pos1.col]))
        {
            found_player = true;
            *player_id_in_range = banquise->grid[check_pos1.line][check_pos1.col].player->id;
        }

        else if(IsPlayer(banquise->grid[check_pos2.line][check_pos2.col]))
        {
            found_player = true;
            *player_id_in_range = banquise->grid[check_pos2.line][check_pos2.col].player->id;
        }
    }

    return found_player;
}


/*
    Auteur(e)(s): Florian Legendre
    Utilité: IsInbound() teste si une position est hors du plateau ou non. IsObject() teste s'il y a un objet, peu importe lequel, tant
             que cet objet n'est pas du type reserved. Pour toutes les autres fonctions il s'agit d'ameliorer la lisibilite des procedures
             qui teste la presence de certains objets donnes.
    Fonctionnement: Renvoit la valeur d'un test booleen
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: /
*/
int IsInbound(int banquise_size, int line, int col)
{
    return ((line >= 0 && line < banquise_size) && (col >= 0 && col < banquise_size));
}


int IsObject(T_case banquise_case)
{
    return (banquise_case.object != NULL && banquise_case.object->object_type != reserved);
}

int IsHammerHead(T_case banquise_case)
{
    return (banquise_case.object != NULL && banquise_case.object->object_type == hammer_head);
}

int IsSpring(T_case banquise_case)
{
    return (banquise_case.object != NULL && banquise_case.object->object_type == spring);
}

int IsWater(T_case banquise_case)
{
    return (banquise_case.ground == water);
}



/*
    Auteur(e)(s): Florian Legendre
    Utilité: Verifie si a une case donnee il y a un objet avec lequel peut interagir un glacon
    Fonctionnement: On controle d'abord que la case est dans le plateau (si ce n'est pas le cas -> renvoit 0), si c'est le cas on controle ensuite
                    si l'objet est interagissable, valeur 1 si oui, 0 sinon.
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: /
*/

int IsFlakeIN(int banquise_size, T_banquise *banquise, int neighbour_line, int neighbour_col)
{
    if(IsInbound(BANQUISE_SIZE, neighbour_line, neighbour_col))
        return (IsWater(banquise->grid[neighbour_line][neighbour_col]) || IsSpring(banquise->grid[neighbour_line][neighbour_col]) || IsHammerHead(banquise->grid[neighbour_line][neighbour_col]) || IsPlayer(banquise->grid[neighbour_line][neighbour_col]));

    else
        return 0;
}



/* ====================================== */
/* =============== OTHERS =============== */
/* ====================================== */


/*
    Auteur(e)(s): Florian Legendre
    Utilité: Permet de se deplacer "right" fois a droite dans les valeurs possibles d'un type enumere quelconque
             pourvu que ce-dernier n'incrémente ses valeurs que d'un entre chacune de ses valeurs. L'entier
             renvoye correspond ainsi a l'etat suivant du type enumere.
    Fonctionnement: Si on deborde des valeurs possibles du type, le modulo nous renvoit au debut (int max represente
                    ainsi la valeur maximale du type enumere)
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: /
*/
int enum_cycle_right(int n, int max, int right)
{
    return ((n + right) % max);
}


/*
    Auteur(e)(s): Florian Legendre
    Utilité: Meme chose que precedemment mais ici on se deplace "left" fois a gauche et donc on explore les etats precedents
    Fonctionnement: Au lieu de le faire directement aller a l'etat precedent et faire un test si la valeur devient negative,
                    ici on le fait aller a droite avec une valeur egale au complement du nombre de pas a gauche ("left"). Le modulo nous
                    ramene alors a la case ou nous souhaitions aller.
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: /
*/
int enum_cycle_left(int n, int max, int left)
{
    return ((n + (max - left)) % max);
}
