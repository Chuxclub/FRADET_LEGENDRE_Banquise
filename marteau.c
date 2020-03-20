#include "marteau.h"


/* ============================================ */
/* ========== INITIALISATION MARTEAU ========== */
/* ============================================ */

/*
    Auteur(e)(s): Florian Legendre
    Utilité: Meme chose que pour initTrap() dans piege.c ou initSpring() dans ressort.c. Seulement ici le marteau est en deux parties: sa
             tete et son manche. Ses deux parties sont reunies dans un type structure T_hammer qui les regroupe. Comme les types structures
             T_hammer_head et T_hammer_handle (respectivement la tete et le manche) ont plus de champs, on a plus de valeurs par defaut a initialiser...
    Fonctionnement: Voir les commentaires dans le corps de la fonction
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: 1) Le type structure T_hammer regroupant tete et manche de marteau n'a finalement que peu d'utilite. Il faudrait donc:
                                          soit s'epargner l'utilisation d'un type inutile le reste du temps, soit verifier si ce-dernier est sous-utilise...
*/

T_hammer *initHammer()
{
    T_hammer_handle *res_handle = (T_hammer_handle *) malloc(sizeof(T_hammer_handle));
    T_hammer_head *res_head = (T_hammer_head *) malloc(sizeof(T_hammer_head));
    T_hammer *res_hammer = (T_hammer *) malloc(sizeof(T_hammer));

    res_handle->id = 0;
    res_handle->pos.line = 0;
    res_handle->pos.col = 0;
    res_handle->aspect = horizontal;

    //Le marteau est initialisé dans cette position: o-
    res_head->id = 0;
    res_head->up_face.d_line = 0;
    res_head->up_face.d_col = -1;

    res_head->left_face.d_line = +1;
    res_head->left_face.d_col = 0;


    //Initilisation des autres paramètres de la tête du marteau
    res_head->pos.line = 0;
    res_head->pos.col = 0;

    res_head->momentum = no_momentum;
    res_head->rot_dir = no_rotation;
    res_head->state = left;

    res_hammer->handle = res_handle;
    res_hammer->head = res_head;

    return res_hammer;
}


/*
    Auteur(e)(s): Florian Legendre
    Utilité: Initialise tous les marteaux du jeu et les regroupe dans un tableau T_object de tetes/manches de marteaux.
    Fonctionnement: Le tableau renvoye en sortie contient des paires tete/manche, d'abord la tete puis le manche. La tete et le manche
                    sont des T_object. A chaque nouveau marteau on doit donc avancer dans +2 dans la boucle, d'ou le "i+=2". De meme, le
                    parametre nb_hammers doit etre double lors de l'allocation dynamique car si on veut 1 marteau on a alors deux T_object generes,
                    la tete et le manche. Etc.
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: 1) C'est ici qu'on utilise pour la premiere fois le type T_hammer. Il faudrait questionner son utilite.
*/
T_object **initHammers(int nb_hammers)
{
    T_object **hammers = (T_object **) malloc(sizeof(T_object *) * 2* nb_hammers);

    for(int i = 0; i < 2 * nb_hammers; i++)
        hammers[i] = (T_object *) malloc(sizeof(T_object));

    for(int i = 0; i < 2 * nb_hammers; i+=2)
    {
        T_hammer *myHammer = initHammer();

        hammers[i]->object_type = hammer_head;
        hammers[i + 1]->object_type = hammer_handle;

        hammers[i]->hammer_head = myHammer->head;
        hammers[i]->hammer_handle = NULL;

        hammers[i + 1]->hammer_head = NULL;
        hammers[i + 1]->hammer_handle = myHammer->handle;


        hammers[i]->flake = NULL;
        hammers[i + 1]->flake = NULL;

        hammers[i]->spring = NULL;
        hammers[i + 1]->spring = NULL;

        hammers[i]->trap = NULL;
        hammers[i + 1]->trap = NULL;
    }

    return hammers;
}



/*
    Auteur(e)(s): Florian Legendre
    Utilité: Ajoute les marteaux sur la banquise
    Fonctionnement: Comme pour addFlakes() dans glacon.c, on parcourt chaque case de la banquise avec une pourcentage de chance ou on placera
                    le potentiel manche du marteau. Si le pourcentage est valide, on verifie qu'il n'y a rien autour du rayon de rotation du marteau
                    qui pourrait en toute logique humaine (meme si du point de vue programmation ce n'est serait pas le cas) entraver la rotation du marteau.
                    Si tout est bon, alors on ajoute le manche et la tete du marteau par simple translation de la position du manche avec le vecteur up_face de la tete
                    du marteau (ce vecteur represente dans quelle direction pointe la face du dessus de la tete du marteau).

                    Note 1: Au moment de placer le marteau on place egalement une zone "reserved" qui empechera le placement d'autres objets
                    et/ou elements de terrain dans cette zone de rotation.
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: /
*/
void addHammers(T_banquise *banquise, T_object **hammers, int nb_hammers)
{
    int counter = 2*nb_hammers - 1;

    while(counter >= 0)
    {
        for(int i = 0; i < banquise->sizeB; i++)
        {
            for(int j = 0; j < banquise->sizeB; j++)
            {
                int loto_hammer = rand() % PERCENT;

                if(counter < 0)
                    return;

                else if(loto_hammer < 5 && AreSurroundingsAvailable(banquise, i, j))
                {
                    int hammer_head_line = i + hammers[counter - 1]->hammer_head->up_face.d_line;
                    int hammer_head_col = j + hammers[counter - 1]->hammer_head->up_face.d_col;

                    //On calcule d'abord la position de la tête du marteau puis celle de sa poignée
                    hammers[counter - 1]->hammer_head->pos.line = hammer_head_line;
                    hammers[counter - 1]->hammer_head->pos.col = hammer_head_col;

                    hammers[counter]->hammer_handle->pos.line = i;
                    hammers[counter]->hammer_handle->pos.col = j;

                    //Branchement sur la banquise
                    PlaceReservedZone(banquise, i, j);
                    banquise->grid[i][j].object = hammers[counter];
                    banquise->grid[hammer_head_line][hammer_head_col].object = hammers[counter - 1];
                    counter -= 2;
                }
            }
        }
    }
}



/*
    Auteur(e)(s): Florian Legendre
    Utilité: Mettre a jour sur la banquise, a chaque nouveau tour, les marteaux en mouvement et eventuellement realiser les interactions necessaires.
    Fonctionnement: Dans toute la fonction on ne s'interesse cette fois-ci qu'a la tete du marteau (on n'anime pas encore le manche). On verifie pour
                    chaque tete de marteau dans nos tableaux de marteaux que ces tetes sont en mouvement (cf. le "momentum") ou non. Si ce n'est pas la cas
                    -> pas de mise a jour! Si c'est le cas, voir le corps de la fonction pour l'explication du reste du fonctionnement.

                    Note 1: Une explication du "vector_carrier" s'impose. Quand le glacon rencontre la tete du marteau, ce-dernier transmet a la tete du marteau
                            son vecteur vitesse. Le champ "vector_carrier" de la tete du marteau recupere en effet le vecteur du glacon, d'ou la "transmission".
                            Quand la tete du marteau change de position on modifie egalement la valeur du "vector_carrier". Un calcul mathematique montre en effet
                            que le nouveau "vector_carrier" correspond a la soustraction entre le vecteur translation de la tete du marteau et l'ancienne valeur
                            du "vector_carrier" En effet:
                                                               A   B
                                                             *  o->           * est le glacon qui a percute o representant la tete du marteau.
                                                                   ||         A la position de depart de cette tete, B la case d'a cote et C la nouvelle case
                                                                   \/         ou sera la tete du marteau.  On a alors les vecteurs AB, AC et AB.
                                                                   C          AC correspond au vecteur horaire ou antihoraire de translation de la tete.
                                                                              AB correspond au "vector_carrier" transmis par le glacon. Le nouveau "vector_carrier"
                                                                              sera le vecteur BC apres translation de la tete du marteau. Ce vecteur est obtenu en faisant
                                                                              AB + BC = AC et donc: BC = AC - AB.

    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: 1) Eviter une separation en cas "horaire" et cas "antihoraire" par un unique calcul mathematique (comme par exemple
                                          avec les fonctions cos() et sin()...
*/
void updateHammers(int nb_hammers, T_game_parts *game_parts)
{
    //Rappel: ordre = tête puis manche => hammers[0] est une tête de marteau, hammers[1] est un manche de marteau, etc.
    for(int i = 0; i < nb_hammers*2; i+=2)
    {
        if(game_parts->hammers[i]->hammer_head->momentum != no_momentum)
        {
            /* Phase 1 : On verifie qu'un joueur n'est pas a portee du marteau, si c'est le cas, le marteau tue le joueu*/
            int player_id;

            if(IsPlayerInRange(game_parts->hammers[i]->hammer_head, game_parts->banquise, &player_id))
                killPlayer(game_parts->players[player_id - 1]);


            /* Phase 2 : Mise a jour de la tete du marteau en utilisant des matrices d'etats-transitions */
            T_vector clockw[4] = {{-1, 1},{1, 1},{1, -1},{-1, -1}};
            T_vector anticlockw[4] = {{1, 1},{1, -1},{-1, -1},{-1, 1}};
            int head_state = game_parts->hammers[i]->hammer_head->state;

            //----> Sens horaire
            if(game_parts->hammers[i]->hammer_head->rot_dir == clockwise)
            {
                //Calculating new head pos and updating banquise
                T_pos previous_H_pos = {game_parts->hammers[i]->hammer_head->pos.line, game_parts->hammers[i]->hammer_head->pos.col};
                T_pos new_H_pos = {previous_H_pos.line + clockw[head_state].d_line, previous_H_pos.col + clockw[head_state].d_col};
                updateObjectOnBanquise(game_parts->hammers[i], new_H_pos.line, new_H_pos.col, game_parts->banquise);

                //Calculating and updating new upf and leftf vectors  for enabling further interactions with flake (cf. FlakeInteraction() in glacon.c)
                int previous_head_state = enum_cycle_left(head_state, 4, 1);
                M_add_vectors(game_parts->hammers[i]->hammer_head->up_face, clockw[head_state], &(game_parts->hammers[i]->hammer_head->up_face));
                M_add_vectors(game_parts->hammers[i]->hammer_head->left_face, clockw[previous_head_state], &(game_parts->hammers[i]->hammer_head->left_face));

                //Turn by turn calculation and update of vector carrier for further transmission to flake (when momentum reaches 0)
                T_vector H_vector_carrier = game_parts->hammers[i]->hammer_head->vector_carrier;
                M_subtract_vectors(clockw[head_state], H_vector_carrier, &(game_parts->hammers[i]->hammer_head->vector_carrier));

                //Changing state and decrementing momentum
                game_parts->hammers[i]->hammer_head->state = enum_cycle_right(game_parts->hammers[i]->hammer_head->state, 4, 1);
                game_parts->hammers[i]->hammer_head->momentum--;

                //Transferring vector carrier to flake when momentum reaches 0
                if(game_parts->hammers[i]->hammer_head->momentum == no_momentum)
                {
                    T_pos transfer_neighb = translate_point(game_parts->hammers[i]->hammer_head->pos, game_parts->hammers[i]->hammer_head->vector_carrier);

                    if(game_parts->banquise->grid[transfer_neighb.line][transfer_neighb.col].object->object_type == flake)
                    {
                        game_parts->banquise->grid[transfer_neighb.line][transfer_neighb.col].object->flake->vect = game_parts->hammers[i]->hammer_head->vector_carrier;
                        game_parts->hammers[i]->hammer_head->vector_carrier = null_vect();
                    }

                    else
                        game_parts->hammers[i]->hammer_head->vector_carrier = null_vect();
                }
            }

            //----> Sens antihoraire
            else
            {
                //Calculating new head pos and updating banquise
                T_pos previous_H_pos = {game_parts->hammers[i]->hammer_head->pos.line, game_parts->hammers[i]->hammer_head->pos.col};
                T_pos new_H_pos = {previous_H_pos.line + anticlockw[head_state].d_line, previous_H_pos.col + anticlockw[head_state].d_col};
                updateObjectOnBanquise(game_parts->hammers[i], new_H_pos.line, new_H_pos.col, game_parts->banquise);

                //Calculating and updating new upf and leftf vectors for enabling further interactions with flake (cf. FlakeInteraction() in glacon.c)
                int previous_head_state = enum_cycle_left(head_state, 4, 1);
                M_add_vectors(game_parts->hammers[i]->hammer_head->up_face, anticlockw[head_state], &(game_parts->hammers[i]->hammer_head->up_face));
                M_add_vectors(game_parts->hammers[i]->hammer_head->left_face, anticlockw[previous_head_state], &(game_parts->hammers[i]->hammer_head->left_face));

                //Turn by turn calculation and update of vector carrier for further transmission to flake (when momentum reaches 0)
                T_vector H_vector_carrier = game_parts->hammers[i]->hammer_head->vector_carrier;
                M_subtract_vectors(anticlockw[head_state], H_vector_carrier, &(game_parts->hammers[i]->hammer_head->vector_carrier));

                //Changing state and decrementing momentum each turn
                game_parts->hammers[i]->hammer_head->state = enum_cycle_left(game_parts->hammers[i]->hammer_head->state, 4, 1);
                game_parts->hammers[i]->hammer_head->momentum--;

                //Transferring vector carrier to flake when momentum reaches 0
                if(game_parts->hammers[i]->hammer_head->momentum == no_momentum)
                {
                    T_pos transfer_neighb = translate_point(game_parts->hammers[i]->hammer_head->pos, game_parts->hammers[i]->hammer_head->vector_carrier);

                    if(game_parts->banquise->grid[transfer_neighb.line][transfer_neighb.col].object->object_type == flake)
                    {
                        game_parts->banquise->grid[transfer_neighb.line][transfer_neighb.col].object->flake->vect = game_parts->hammers[i]->hammer_head->vector_carrier;
                        game_parts->hammers[i]->hammer_head->vector_carrier = null_vect();
                    }

                    else
                        game_parts->hammers[i]->hammer_head->vector_carrier = null_vect();
                }
            }
        }
    }
}

