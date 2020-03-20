#include "test_funcs.h"
#include <string.h>

// ------> Fonctions de test "en jeu"
/* ============================================ */
/* ========= FONCTIONS D'AJOUT EN JEU ========= */
/* ============================================ */

/*
    Auteur(e)(s): Florian Legendre
    Utilité: Permet d'ajouter de l'eau à la position indiquée
    Fonctionnement: Verifie, par securite, qu'il n'y a pas un joueur ou un drapeau (ce qui pourrait nuire gravement
                    a de futurs tests), ecrase le reste par de l'eau.
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: Comme pour toutes les fonctions de tous les modules qui ont des int line et int col, on aurait
                                       pu directement passer en parametre une position...
*/
void adminAddWater(T_game_parts game_parts, int line, int col)
{
    if(game_parts.banquise->grid[line][col].player != NULL || game_parts.banquise->grid[line][col].flag != no_flag)
        printf("Command could not be executed as there's either a player or a flag on designated location");

    //Seuls les objets et le sol peuvent être effacé par cette commande admin, pas les joueurs ni les drapeaux
    else
    {
        game_parts.banquise->grid[line][col].ground = water;
        game_parts.banquise->grid[line][col].object = NULL;
    }
}


/* ====================================================== */
/* ========= FONCTIONS DE TELEPORTATIONS EN JEU ========= */
/* ====================================================== */

/*
    Auteur(e)(s): Florian Legendre
    Utilité: Permet de teleporter le joueur a la position indiquee
    Fonctionnement: On verifie, par securite pour de futurs tests, que la case destination est bien dans le plateau et qu'il n'y a pas d'objets autre que "reserved"
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: /
*/
void adminTpPlayer(T_game_parts game_parts, int player_id, int line, int col)
{
    if(!(IsCaseAvailable(game_parts.banquise->grid[line][col])) || !(IsInbound(BANQUISE_SIZE, line, col)))
        printf("Command could not be executed as there's either a player or a flag on designated location or location is out of bounds");

    else
    {
        int previous_line = game_parts.players[player_id]->details.pos.line;
        int previous_col = game_parts.players[player_id]->details.pos.col;

        game_parts.players[player_id]->details.pos.line = line;
        game_parts.players[player_id]->details.pos.col = col;

        updatePlayer(game_parts.players[player_id], previous_line, previous_col, game_parts.banquise);
    }
}


/*
    Auteur(e)(s): Florian Legendre
    Utilité: Meme chose que precedemment mais avec des flocons.
    Fonctionnement: On prend ici en compte, en plus, le vecteur vitesse actuel d'un glacon
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: On pourrait, pour ameliorer la lisibilite, utiliser les fonctions
                                       updateObjectOnBanquise() dans banquise.c et stopFlake() dans glacon.c
*/
void adminTpFlake(T_game_parts game_parts, int flake_id, int line, int col)
{
    if(!(IsCaseAvailable(game_parts.banquise->grid[line][col])) || !(IsInbound(BANQUISE_SIZE, line, col)))
        printf("Command could not be executed as there's either a player or a flag on designated location or location is out of bounds");

    //Seuls les objets et le sol peuvent être effacé par cette commande admin, pas les joueurs ni les drapeaux
    else
    {
        int previous_line = game_parts.flakes[flake_id]->flake->pos.line;
        int previous_col = game_parts.flakes[flake_id]->flake->pos.col;

        //Mise à jour des coordonnées du glaçon, arrêt du glaçon s'il était en mouvement
        game_parts.flakes[flake_id]->flake->pos.line = line;
        game_parts.flakes[flake_id]->flake->pos.col = col;
        game_parts.flakes[flake_id]->flake->vect.d_line = 0;
        game_parts.flakes[flake_id]->flake->vect.d_col = 0;

        //Mise à jour de la banquise: débranchement du glaçon à sa précédente position puis rebranchement
        game_parts.banquise->grid[previous_line][previous_col].object = NULL;

        game_parts.banquise->grid[line][col].object = game_parts.flakes[flake_id];
        game_parts.banquise->grid[line][col].object->object_type = flake;
        game_parts.banquise->grid[line][col].object->flake = game_parts.flakes[flake_id]->flake;
    }
}


/*
    Auteur(e)(s): Florian Legendre
    Utilité: Pouvoir teleporter des ressorts
    Fonctionnement: Meme chose que pour les glacons avec les vecteurs vitesses en moins
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: /
*/
void adminTpSpring(T_game_parts game_parts, int spring_id, int line, int col)
{
    if(!(IsCaseAvailable(game_parts.banquise->grid[line][col])) || !(IsInbound(BANQUISE_SIZE, line, col)))
        printf("Command could not be executed as there's either a player or a flag on designated location or location is out of bounds");

    //Seuls les objets et le sol peuvent être effacé par cette commande admin, pas les joueurs ni les drapeaux
    else
    {
        int previous_line = game_parts.springs[spring_id]->spring->pos.line;
        int previous_col = game_parts.springs[spring_id]->spring->pos.col;

        //Mise à jour des coordonnées du ressort
        game_parts.springs[spring_id]->spring->pos.line = line;
        game_parts.springs[spring_id]->spring->pos.col = col;

        //Mise à jour de la banquise: débranchement du glaçon à sa précédente position puis rebranchement
        game_parts.banquise->grid[previous_line][previous_col].object = NULL;

        game_parts.banquise->grid[line][col].object = game_parts.springs[spring_id];
        game_parts.banquise->grid[line][col].object->object_type = spring;
        game_parts.banquise->grid[line][col].object->flake = game_parts.springs[spring_id]->spring;
    }
}


/* =================================================== */
/* ========= FONCTIONS DE SUPPRESSION EN JEU ========= */
/* =================================================== */


/*
    Auteur(e)(s): Florian Legendre
    Utilité: Pouvoir tuer un joueur designe par son identifiant
    Fonctionnement: Modifie son etat "health" a "dead"
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: /
*/
void adminKillPlayer(T_game_parts game_parts, int player_id)
{
        game_parts.players[player_id]->details.health = dead;
}


/* ====================================================== */
/* ========= FONCTIONS D'IDENTIFICATION EN JEU ========= */
/* ====================================================== */



/*
    Auteur(e)(s): Florian Legendre
    Utilité: Fournit toutes les informations possibles sur une case et l'etat d'un eventuel objet/joueur present
             sur la case au moment de l'appel de la fonction dans le jeu
    Fonctionnement: Des switch() permettent de gerer la presence ou non de drapeau ou d'elements de terrains.
                    La presence ou non de joueurs/objets est gere par des if/then/else. Une fois qu'une presence
                    est avere on gere, si necessaire, les informations a afficher par de nouveaux switch()
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: /
*/
void adminInfoCase(T_game_parts game_parts, int line, int col)
{
    T_flag flag = game_parts.banquise->grid[line][col].flag;
    T_ground ground = game_parts.banquise->grid[line][col].ground;
    T_object *object = game_parts.banquise->grid[line][col].object;
    T_player *player = game_parts.banquise->grid[line][col].player;

    printf("\n\n/!\\ ==== DESCRIPTION CASE %i %i ==== /!\\ \n", line, col);

    switch(flag)
    {
        case no_flag:
            printf("Flag: No flag\n\n");
            break;

        case A:
            printf("Flag: A\n\n");
            break;

        case B:
            printf("Flag: B\n\n");
            break;

        default:
            color(12, 0);
            printf("Something unknown\n\n");
            color(15, 0);
            break;
    }

    switch(ground)
    {
        case ice:
            printf("Ground: ice\n\n");
            break;

        case water:
            printf("Flag: water\n\n");
            break;

        case rock:
            printf("Flag: rock\n\n");
            break;

        default:
            color(12, 0);
            printf("Something unknown\n\n");
            color(15, 0);
            break;
    }

    if(object == NULL)
        printf("Object: no object\n\n");

    else
    {
        switch(object->object_type)
        {
            case flake:
                printf("Object: flake\n");
                printf("\t Flake line : %i\n", object->flake->pos.line);
                printf("\t Flake column : %i\n", object->flake->pos.col);
                printf("\t Flake d_line : %i\n", object->flake->vect.d_line);
                printf("\t Flake d_column : %i\n\n", object->flake->vect.d_col);
                break;

            case trap:
                printf("Object: trap\n");
                printf("\t Trap line : %i\n", object->trap->pos.line);
                printf("\t Trap column : %i\n\n", object->trap->pos.col);
                break;

            case spring:
                printf("Object: spring\n");
                printf("\t Spring line : %i\n", object->spring->pos.line);
                printf("\t Spring column : %i\n\n", object->spring->pos.col);
                break;

            case hammer_head:
                printf("Object: hammer head\n\n");
                printf("\t Hammer id : %i\n\n", object->hammer_head->id);
                printf("\t Hammer momentum : %i\n", object->hammer_head->momentum);
                printf("\t Hammer rotation direction : %i\n", object->hammer_head->rot_dir);
                printf("\t Hammer state : %i\n", object->hammer_head->state);
                printf("\t Hammer vector carrier line : %i\n", object->hammer_head->vector_carrier.d_line);
                printf("\t Hammer vector carrier col : %i\n\n", object->hammer_head->vector_carrier.d_col);
                printf("\t Hammer line : %i\n", object->hammer_head->pos.line);
                printf("\t Hammer column : %i\n\n", object->hammer_head->pos.col);
                printf("\t Hammer up_face line : %i\n", object->hammer_head->up_face.d_line);
                printf("\t Hammer up_face col : %i\n", object->hammer_head->up_face.d_col);
                printf("\t Hammer left_face line : %i\n", object->hammer_head->left_face.d_line);
                printf("\t Hammer left_face col : %i\n", object->hammer_head->left_face.d_col);
                break;

            case hammer_handle:
                printf("Object: hammer handle\n\n");
                break;

            case reserved:
                printf("Object: reserved case\n\n");
                break;

            default:
                color(12, 0);
                printf("Something unknown\n\n");
                color(15, 0);
                break;
        }
    }

    if(player == NULL)
        printf("Player: no player\n\n");

    else
    {
        printf("Player Id: %i\n", player->id);
        printf("\t Player Line: %i\n", player->details.pos.line);
        printf("\t Player Column: %i\n", player->details.pos.col);
        printf("\t Player Freedom: %i\n", player->details.freedom);
        printf("\t Player Health: %i\n", player->details.health);
    }

    printf("\n/!\\ =============================== /!\\ \n", line, col);
}



/*
    Auteur(e)(s): Florian Legendre
    Utilité: Afficher toutes les informations disponibles sur tous les glacons dans leur tableau (cf. dans game_parts)
    Fonctionnement: Boucle parmi les elements du tableau. Affichage si le glacon est encore present dans le jeu (!= NULL)
                    sinon on affiche le fait qu'un certain glacon n'existe plus dans le jeu
    Complexité en temps (au pire): O(nb_flakes)
    Hypothèse d'amélioration possible: /
*/
void adminShowAllFlakes(T_game_parts game_parts, int nb_flakes)
{
    printf("\n\n/!\\ ==== DESCRIPTION DES FLOCONS EN JEU ==== /!\\ \n\n");
    printf("Reminder: {no_object = 0, flake, spring, hammer_handle, hammer_head, trap, reserved} \n\n");

    for(int i = 0; i < nb_flakes; i++)
    {
        if(game_parts.flakes[i]->flake != NULL)
        {
            printf("Flake #%i:\n", i);
            printf("\tObject_type: %i \n", game_parts.flakes[i]->object_type);
            printf("\tPos_line: %i \n", game_parts.flakes[i]->flake->pos.line);
            printf("\tPos_col: %i \n", game_parts.flakes[i]->flake->pos.col);
            printf("\tVect_line: %i \n", game_parts.flakes[i]->flake->vect.d_line);
            printf("\tVect_col: %i \n\n", game_parts.flakes[i]->flake->vect.d_col);
        }

        else
        {
            printf("Flake #%i:\n", i);
            printf("\tFlake doesn't exist anymore \n\n", game_parts.flakes[i]->object_type);
        }
    }

    printf("\n/!\\ ========================================== /!\\ \n");
}



/*
    Auteur(e)(s): Florian Legendre
    Utilité: Meme chose que pour adminShowAllFlakes() mais avec les joueurs
    Fonctionnement: Les joueurs existent toujours dans le jeu qu'il soit dans l'etat mort ou non
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: /
*/
void adminShowAllPlayers(T_game_parts game_parts, int nb_players)
{
    printf("\n\n/!\\ ==== DESCRIPTION DES JOUEURS EN JEU ==== /!\\ \n\n");
    printf("Reminder: Freedom => {player_trapped = 0, cooldown3, cooldown2, cooldown1, player_free} \n");
    printf("          Health => {dead = 0, full_health} \n\n");

    for(int i = 0; i < nb_players; i++)
    {
        printf("Player #%i:\n", i);
        printf("\tPlayer_id: %i \n", game_parts.players[i]->id);
        printf("\tPos_line: %i \n", game_parts.players[i]->details.pos.line);
        printf("\tPos_col: %i \n", game_parts.players[i]->details.pos.col);
        printf("\tFreedom: %i \n", game_parts.players[i]->details.freedom);
        printf("\tHealth: %i \n", game_parts.players[i]->details.health);
    }

    printf("\n/!\\ ========================================== /!\\ \n");
}


/* ============================================ */
/* ============ FONCTION MAITRESSE ============ */
/* ============================================ */


/*
    Auteur(e)(s): Florian Legendre
    Utilité: Permet, selon la commande donnee par l'utilisateur, de lancer la fonction correspondant à la commande
             avec les parametres donnees par l'utilisateur.
    Fonctionnement: Compare la chaine de caractere donnee par le joueur et copiee dans cmd[30] avec une chaine attendue, constante,
                    pour autoriser l'execution d'une commande. Si la commande est valide on passe eventuellement les parametres entiers
                    qu'a fournit le joueur.
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: Eviter une saisie prematuree des entiers symbolisant une position souhaitee. Cette saisie n'est pas toujours
                                       pertinente et engendre parfois des confusions lors des tests, ce qui a pour effet de ralentir ces-derniers.
*/
void adminPrivileges(T_game_parts game_parts)
{
    printf("\n@dmin: ");

    char cmd[30];
    int line;
    int col;

    scanf("%s %i %i", cmd, &line, &col); //Un parser de syntaxe permettrait d'éviter la saisie parfois inutile des lignes/colonnes

    if(strcmp(cmd, "addWater") == 0)
        adminAddWater(game_parts, line, col);

    else if(strcmp(cmd, "killPlayer") == 0)
    {
        int player_id = 0;
        printf("\nIndicate which player you want to kill: ");
        scanf("%i", &player_id);

        if(game_parts.players[player_id]->details.health == dead)
            printf("\nPlayer %i is already dead", player_id);

        else
            adminKillPlayer(game_parts, player_id - 1); //-1 pour pouvoir écrire adminTpPlayer 0 0 1 par exemple...
    }

    else if(strcmp(cmd, "tpPlayer") == 0)
    {
        int player_id = 0;
        printf("\nIndicate which player you want to move: ");
        scanf("%i", &player_id);
        adminTpPlayer(game_parts, player_id - 1, line, col);
    }

    else if(strcmp(cmd, "tpFlake") == 0)
    {
        int flake_id = 0;
        printf("\nIndicate which flake you want to move: ");
        scanf("%i", &flake_id);

        if(game_parts.flakes[flake_id - 1]->flake == NULL)
            printf("\nFlake %i doesn't exist anymore (maybe it has melted?)", flake_id);

        else
            adminTpFlake(game_parts, flake_id - 1, line, col);
    }

    else if(strcmp(cmd, "tpSpring") == 0)
    {
        int spring_id = 0;
        printf("\nIndicate which spring you want to move: ");
        scanf("%i", &spring_id);
        adminTpSpring(game_parts, spring_id - 1, line, col);
    }

    else if(strcmp(cmd, "infoCase") == 0)
        adminInfoCase(game_parts, line, col);

    else if(strcmp(cmd, "showAllFlakes") == 0)
        adminShowAllFlakes(game_parts, NB_FLAKES);

    else if(strcmp(cmd, "showAllPlayers") == 0)
        adminShowAllPlayers(game_parts, game_parts.quantities[5]);

    else
        return;
}



// ------> Fonctions de test "en code"
/* ============================================ */
/* =========== INTERACTIONS GLACONS =========== */
/* ============================================ */



/*
    Auteur(e)(s): Florian Legendre
    Utilité: moveFlakeUp() et ses derivees (Down, Right, Left...) sont les anciennes fonctions de deplacements
             des glacons. Elle permettait le deplacement d'un glacon aussi loin qu'il le pouvait dans une direction
             donnee (Right, Left...). Elles sont conservees ici bien qu'elles ne soient plus utilisees car elles pourraient
             servir dans adminPrivilegies() a tester rapidement des interactions glacons/objets_interagissables.
    Fonctionnement: Le fonctionnement est detaille dans le corps de chacune des fonctions de cette famille de fonction.
    Complexité en temps (au pire): O(nb_de_case_ou_le_glacon_peut_se_deplacer)
    Hypothèse d'amélioration possible: /
*/
void moveFlakeUp(T_object *bumped_flake, T_banquise *banquise)
{
    /*Récupération des coordonnées de la case où se trouve initialement le glaçon*/
    int previous_line = bumped_flake->flake->pos.line;
    int previous_col = bumped_flake->flake->pos.col;
    int new_line = previous_line;

    /*Assignation d'une vitesse au glaçon bougé (ou 'bumpé'...)*/
    bumped_flake->flake->vect.d_line = -1;

    /*Déplacement du glaçon tant qu'il peut se déplacer*/
    while(IsInbound(BANQUISE_SIZE, new_line - 1, previous_col))
    {
        if(IsCaseAvailable(banquise->grid[new_line - 1][previous_col]))
            new_line += bumped_flake->flake->vect.d_line;
        else
            break;
    }


    /*Lorsque le glaçon s'arrête on vérifie si l'arrêt a été provoqué par un voisin avec lequel il peut interagir...*/
    if(IsFlakeIN(BANQUISE_SIZE, banquise, new_line - 1, previous_col))
    {
        //Le glaçon s'arrête avant l'objet avec lequel interagir
        bumped_flake->flake->vect.d_line = 0;
        bumped_flake->flake->pos.line = new_line;

        //L'interaction est réalisée
        FlakeInteraction(bumped_flake, new_line - 1, previous_col, banquise);

        //Le glaçon est détruit de la banquise puis de la mémoire
        banquise->grid[previous_line][previous_col].object = NULL;
        free(bumped_flake);
    }


    /* ... Ou pas */
    else
    {
        //Annulation de la vitesse du glaçon
        bumped_flake->flake->vect.d_line = 0;

        //Modification de la ligne où se trouve à présent le glaçon
        bumped_flake->flake->pos.line = new_line;

        //Suppression du glaçon sur la banquise, à sa position précédente
        banquise->grid[previous_line][previous_col].object = NULL;

        //Ajout du glaçon sur la banquise, sur sa nouvelle position calculée
        banquise->grid[new_line][previous_col].object = bumped_flake;
    }
}

void moveFlakeLeft(T_object *bumped_flake, T_banquise *banquise)
{
    /*Récupération des coordonnées de la case où se trouve initialement le glaçon*/
    int previous_line = bumped_flake->flake->pos.line;
    int previous_col = bumped_flake->flake->pos.col;
    int new_col = previous_col;

    /*Assignation d'une vitesse au glaçon bougé (ou 'bumpé'...)*/
    bumped_flake->flake->vect.d_col = -1;

    /*Déplacement du glaçon tant qu'il peut se déplacer*/
    while(IsInbound(BANQUISE_SIZE, previous_line, new_col - 1))
    {
        if(IsCaseAvailable(banquise->grid[previous_line][new_col - 1]))
            new_col += bumped_flake->flake->vect.d_col;
        else
            break;
    }


    /*Lorsque le glaçon s'arrête on vérifie si l'arrêt a été provoqué par un voisin avec lequel il peut interagir...*/
    if(IsFlakeIN(BANQUISE_SIZE, banquise, previous_line, new_col - 1))
    {
        //Le glaçon s'arrête avant l'objet avec lequel interagir
        bumped_flake->flake->vect.d_line = 0;
        bumped_flake->flake->pos.col = new_col;

        //L'interaction est réalisée
        FlakeInteraction(bumped_flake, previous_line, new_col - 1, banquise);

        //Le glaçon est détruit de la banquise puis de la mémoire
        banquise->grid[previous_line][previous_col].object = NULL;
        free(bumped_flake);
    }


    /* ... Ou pas */
    else
    {
        //Annulation de la vitesse du glaçon
        bumped_flake->flake->vect.d_col = 0;

        //Modification de la ligne où se trouve à présent le glaçon
        bumped_flake->flake->pos.col = new_col;

        //Suppression du glaçon sur la banquise, à sa position précédente
        banquise->grid[previous_line][previous_col].object = NULL;

        //Ajout du glaçon sur la banquise, sur sa nouvelle position calculée
        banquise->grid[previous_line][new_col].object = bumped_flake;
    }
}

void moveFlakeDown(T_object *bumped_flake, T_banquise *banquise)
{
    /*Récupération des coordonnées de la case où se trouve initialement le glaçon*/
    int previous_line = bumped_flake->flake->pos.line;
    int previous_col = bumped_flake->flake->pos.col;
    int new_line = previous_line;

    /*Assignation d'une vitesse au glaçon bougé (ou 'bumpé'...)*/
    bumped_flake->flake->vect.d_line = +1;

    /*Déplacement du glaçon tant qu'il peut se déplacer*/
    while(IsInbound(BANQUISE_SIZE, new_line + 1, previous_col))
    {
        if(IsCaseAvailable(banquise->grid[new_line + 1][previous_col]))
            new_line += bumped_flake->flake->vect.d_line;
        else
            break;
    }


    /*Lorsque le glaçon s'arrête on vérifie si l'arrêt a été provoqué par un voisin avec lequel il peut interagir...*/
    if(IsFlakeIN(BANQUISE_SIZE, banquise, new_line + 1, previous_col))
    {
        //Le glaçon s'arrête avant l'objet avec lequel interagir
        bumped_flake->flake->vect.d_line = 0;
        bumped_flake->flake->pos.line = new_line;

        //L'interaction est réalisée
        FlakeInteraction(bumped_flake, new_line + 1, previous_col, banquise);

        //Le glaçon est détruit de la banquise puis de la mémoire
        banquise->grid[previous_line][previous_col].object = NULL;
        free(bumped_flake);
    }


    /* ... Ou pas */
    else
    {
        //Annulation de la vitesse du glaçon
        bumped_flake->flake->vect.d_line = 0;

        //Modification de la ligne où se trouve à présent le glaçon
        bumped_flake->flake->pos.line = new_line;

        //Suppression du glaçon sur la banquise, à sa position précédente
        banquise->grid[previous_line][previous_col].object = NULL;

        //Ajout du glaçon sur la banquise, sur sa nouvelle position calculée
        banquise->grid[new_line][previous_col].object = bumped_flake;
    }
}

void moveFlakeRight(T_object *bumped_flake, T_banquise *banquise)
{
    /*Récupération des coordonnées de la case où se trouve initialement le glaçon*/
    int previous_line = bumped_flake->flake->pos.line;
    int previous_col = bumped_flake->flake->pos.col;
    int new_col = previous_col;

    /*Assignation d'une vitesse au glaçon bougé (ou 'bumpé'...)*/
    bumped_flake->flake->vect.d_col = +1;

    /*Déplacement du glaçon tant qu'il peut se déplacer*/
    while(IsInbound(BANQUISE_SIZE, previous_line, new_col + 1))
    {
        if(IsCaseAvailable(banquise->grid[previous_line][new_col + 1]))
            new_col += bumped_flake->flake->vect.d_col;
        else
            break;
    }


    /*Lorsque le glaçon s'arrête on vérifie si l'arrêt a été provoqué par un voisin avec lequel il peut interagir...*/
    if(IsFlakeIN(BANQUISE_SIZE, banquise, previous_line, new_col + 1))
    {
        //Le glaçon s'arrête avant l'objet avec lequel interagir
        bumped_flake->flake->vect.d_line = 0;
        bumped_flake->flake->pos.col = new_col;

        //L'interaction est réalisée
        FlakeInteraction(bumped_flake, previous_line, new_col + 1, banquise);

        //Le glaçon est détruit de la banquise puis de la mémoire
        banquise->grid[previous_line][previous_col].object = NULL;
        free(bumped_flake);
    }


    /* ... Ou pas */
    else
    {
        //Annulation de la vitesse du glaçon
        bumped_flake->flake->vect.d_col = 0;

        //Modification de la ligne où se trouve à présent le glaçon
        bumped_flake->flake->pos.col = new_col;

        //Suppression du glaçon sur la banquise, à sa position précédente
        banquise->grid[previous_line][previous_col].object = NULL;

        //Ajout du glaçon sur la banquise, sur sa nouvelle position calculée
        banquise->grid[previous_line][new_col].object = bumped_flake;
    }
}
