#include "test_funcs.h"
#include <string.h>

// ------> Fonctions de test "en jeu"
/* ============================================ */
/* ========= FONCTIONS D'AJOUT EN JEU ========= */
/* ============================================ */

void adminAddWater(T_game_parts game_parts, int line, int col)
{
    if(game_parts.banquise->grid[line][col].player != NULL || game_parts.banquise->grid[line][col].flag != no_flag)
        printf("Command could not be executed as there's either a player or a flag on designated location");

    //Seuls les objets et le sol peuvent �tre effac� par cette commande admin, pas les joueurs ni les drapeaux
    else
    {
        game_parts.banquise->grid[line][col].ground = water;
        game_parts.banquise->grid[line][col].object = NULL;
    }
}


/* ====================================================== */
/* ========= FONCTIONS DE TELEPORTATIONS EN JEU ========= */
/* ====================================================== */

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

void adminTpFlake(T_game_parts game_parts, int flake_id, int line, int col)
{
    if(!(IsCaseAvailable(game_parts.banquise->grid[line][col])) || !(IsInbound(BANQUISE_SIZE, line, col)))
        printf("Command could not be executed as there's either a player or a flag on designated location or location is out of bounds");

    //Seuls les objets et le sol peuvent �tre effac� par cette commande admin, pas les joueurs ni les drapeaux
    else
    {
        int previous_line = game_parts.flakes[flake_id]->flake->pos.line;
        int previous_col = game_parts.flakes[flake_id]->flake->pos.col;

        //Mise � jour des coordonn�es du gla�on, arr�t du gla�on s'il �tait en mouvement
        game_parts.flakes[flake_id]->flake->pos.line = line;
        game_parts.flakes[flake_id]->flake->pos.col = col;
        game_parts.flakes[flake_id]->flake->vect.d_line = 0;
        game_parts.flakes[flake_id]->flake->vect.d_col = 0;

        //Mise � jour de la banquise: d�branchement du gla�on � sa pr�c�dente position puis rebranchement
        game_parts.banquise->grid[previous_line][previous_col].object = NULL;

        game_parts.banquise->grid[line][col].object = game_parts.flakes[flake_id];
        game_parts.banquise->grid[line][col].object->object_type = flake;
        game_parts.banquise->grid[line][col].object->flake = game_parts.flakes[flake_id]->flake;
    }
}

void adminTpSpring(T_game_parts game_parts, int spring_id, int line, int col)
{
    if(!(IsCaseAvailable(game_parts.banquise->grid[line][col])) || !(IsInbound(BANQUISE_SIZE, line, col)))
        printf("Command could not be executed as there's either a player or a flag on designated location or location is out of bounds");

    //Seuls les objets et le sol peuvent �tre effac� par cette commande admin, pas les joueurs ni les drapeaux
    else
    {
        int previous_line = game_parts.springs[spring_id]->spring->pos.line;
        int previous_col = game_parts.springs[spring_id]->spring->pos.col;

        //Mise � jour des coordonn�es du ressort
        game_parts.springs[spring_id]->spring->pos.line = line;
        game_parts.springs[spring_id]->spring->pos.col = col;

        //Mise � jour de la banquise: d�branchement du gla�on � sa pr�c�dente position puis rebranchement
        game_parts.banquise->grid[previous_line][previous_col].object = NULL;

        game_parts.banquise->grid[line][col].object = game_parts.springs[spring_id];
        game_parts.banquise->grid[line][col].object->object_type = spring;
        game_parts.banquise->grid[line][col].object->flake = game_parts.springs[spring_id]->spring;
    }
}


/* =================================================== */
/* ========= FONCTIONS DE SUPPRESSION EN JEU ========= */
/* =================================================== */

void adminKillPlayer(T_game_parts game_parts, int player_id)
{
        game_parts.players[player_id]->details.health = dead;
}


/* ====================================================== */
/* ========= FONCTIONS D'IDENTIFICATION EN JEU ========= */
/* ====================================================== */

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

void adminPrivileges(T_game_parts game_parts)
{
    printf("\n@dmin: ");

    char cmd[30];
    int line;
    int col;

    scanf("%s %i %i", cmd, &line, &col); //Un parser de syntaxe permettrait d'�viter la saisie parfois inutile des lignes/colonnes

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
            adminKillPlayer(game_parts, player_id - 1); //-1 pour pouvoir �crire adminTpPlayer 0 0 1 par exemple...
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

void moveFlakeUp(T_object *bumped_flake, T_banquise *banquise)
{
    /*R�cup�ration des coordonn�es de la case o� se trouve initialement le gla�on*/
    int previous_line = bumped_flake->flake->pos.line;
    int previous_col = bumped_flake->flake->pos.col;
    int new_line = previous_line;

    /*Assignation d'une vitesse au gla�on boug� (ou 'bump�'...)*/
    bumped_flake->flake->vect.d_line = -1;

    /*D�placement du gla�on tant qu'il peut se d�placer*/
    while(IsInbound(BANQUISE_SIZE, new_line - 1, previous_col))
    {
        if(IsCaseAvailable(banquise->grid[new_line - 1][previous_col]))
            new_line += bumped_flake->flake->vect.d_line;
        else
            break;
    }


    /*Lorsque le gla�on s'arr�te on v�rifie si l'arr�t a �t� provoqu� par un voisin avec lequel il peut interagir...*/
    if(IsFlakeIN(BANQUISE_SIZE, banquise, new_line - 1, previous_col))
    {
        //Le gla�on s'arr�te avant l'objet avec lequel interagir
        bumped_flake->flake->vect.d_line = 0;
        bumped_flake->flake->pos.line = new_line;

        //L'interaction est r�alis�e
        FlakeInteraction(bumped_flake, new_line - 1, previous_col, banquise);

        //Le gla�on est d�truit de la banquise puis de la m�moire
        banquise->grid[previous_line][previous_col].object = NULL;
        free(bumped_flake);
    }


    /* ... Ou pas */
    else
    {
        //Annulation de la vitesse du gla�on
        bumped_flake->flake->vect.d_line = 0;

        //Modification de la ligne o� se trouve � pr�sent le gla�on
        bumped_flake->flake->pos.line = new_line;

        //Suppression du gla�on sur la banquise, � sa position pr�c�dente
        banquise->grid[previous_line][previous_col].object = NULL;

        //Ajout du gla�on sur la banquise, sur sa nouvelle position calcul�e
        banquise->grid[new_line][previous_col].object = bumped_flake;
    }
}

void moveFlakeLeft(T_object *bumped_flake, T_banquise *banquise)
{
    /*R�cup�ration des coordonn�es de la case o� se trouve initialement le gla�on*/
    int previous_line = bumped_flake->flake->pos.line;
    int previous_col = bumped_flake->flake->pos.col;
    int new_col = previous_col;

    /*Assignation d'une vitesse au gla�on boug� (ou 'bump�'...)*/
    bumped_flake->flake->vect.d_col = -1;

    /*D�placement du gla�on tant qu'il peut se d�placer*/
    while(IsInbound(BANQUISE_SIZE, previous_line, new_col - 1))
    {
        if(IsCaseAvailable(banquise->grid[previous_line][new_col - 1]))
            new_col += bumped_flake->flake->vect.d_col;
        else
            break;
    }


    /*Lorsque le gla�on s'arr�te on v�rifie si l'arr�t a �t� provoqu� par un voisin avec lequel il peut interagir...*/
    if(IsFlakeIN(BANQUISE_SIZE, banquise, previous_line, new_col - 1))
    {
        //Le gla�on s'arr�te avant l'objet avec lequel interagir
        bumped_flake->flake->vect.d_line = 0;
        bumped_flake->flake->pos.col = new_col;

        //L'interaction est r�alis�e
        FlakeInteraction(bumped_flake, previous_line, new_col - 1, banquise);

        //Le gla�on est d�truit de la banquise puis de la m�moire
        banquise->grid[previous_line][previous_col].object = NULL;
        free(bumped_flake);
    }


    /* ... Ou pas */
    else
    {
        //Annulation de la vitesse du gla�on
        bumped_flake->flake->vect.d_col = 0;

        //Modification de la ligne o� se trouve � pr�sent le gla�on
        bumped_flake->flake->pos.col = new_col;

        //Suppression du gla�on sur la banquise, � sa position pr�c�dente
        banquise->grid[previous_line][previous_col].object = NULL;

        //Ajout du gla�on sur la banquise, sur sa nouvelle position calcul�e
        banquise->grid[previous_line][new_col].object = bumped_flake;
    }
}

void moveFlakeDown(T_object *bumped_flake, T_banquise *banquise)
{
    /*R�cup�ration des coordonn�es de la case o� se trouve initialement le gla�on*/
    int previous_line = bumped_flake->flake->pos.line;
    int previous_col = bumped_flake->flake->pos.col;
    int new_line = previous_line;

    /*Assignation d'une vitesse au gla�on boug� (ou 'bump�'...)*/
    bumped_flake->flake->vect.d_line = +1;

    /*D�placement du gla�on tant qu'il peut se d�placer*/
    while(IsInbound(BANQUISE_SIZE, new_line + 1, previous_col))
    {
        if(IsCaseAvailable(banquise->grid[new_line + 1][previous_col]))
            new_line += bumped_flake->flake->vect.d_line;
        else
            break;
    }


    /*Lorsque le gla�on s'arr�te on v�rifie si l'arr�t a �t� provoqu� par un voisin avec lequel il peut interagir...*/
    if(IsFlakeIN(BANQUISE_SIZE, banquise, new_line + 1, previous_col))
    {
        //Le gla�on s'arr�te avant l'objet avec lequel interagir
        bumped_flake->flake->vect.d_line = 0;
        bumped_flake->flake->pos.line = new_line;

        //L'interaction est r�alis�e
        FlakeInteraction(bumped_flake, new_line + 1, previous_col, banquise);

        //Le gla�on est d�truit de la banquise puis de la m�moire
        banquise->grid[previous_line][previous_col].object = NULL;
        free(bumped_flake);
    }


    /* ... Ou pas */
    else
    {
        //Annulation de la vitesse du gla�on
        bumped_flake->flake->vect.d_line = 0;

        //Modification de la ligne o� se trouve � pr�sent le gla�on
        bumped_flake->flake->pos.line = new_line;

        //Suppression du gla�on sur la banquise, � sa position pr�c�dente
        banquise->grid[previous_line][previous_col].object = NULL;

        //Ajout du gla�on sur la banquise, sur sa nouvelle position calcul�e
        banquise->grid[new_line][previous_col].object = bumped_flake;
    }
}

void moveFlakeRight(T_object *bumped_flake, T_banquise *banquise)
{
    /*R�cup�ration des coordonn�es de la case o� se trouve initialement le gla�on*/
    int previous_line = bumped_flake->flake->pos.line;
    int previous_col = bumped_flake->flake->pos.col;
    int new_col = previous_col;

    /*Assignation d'une vitesse au gla�on boug� (ou 'bump�'...)*/
    bumped_flake->flake->vect.d_col = +1;

    /*D�placement du gla�on tant qu'il peut se d�placer*/
    while(IsInbound(BANQUISE_SIZE, previous_line, new_col + 1))
    {
        if(IsCaseAvailable(banquise->grid[previous_line][new_col + 1]))
            new_col += bumped_flake->flake->vect.d_col;
        else
            break;
    }


    /*Lorsque le gla�on s'arr�te on v�rifie si l'arr�t a �t� provoqu� par un voisin avec lequel il peut interagir...*/
    if(IsFlakeIN(BANQUISE_SIZE, banquise, previous_line, new_col + 1))
    {
        //Le gla�on s'arr�te avant l'objet avec lequel interagir
        bumped_flake->flake->vect.d_line = 0;
        bumped_flake->flake->pos.col = new_col;

        //L'interaction est r�alis�e
        FlakeInteraction(bumped_flake, previous_line, new_col + 1, banquise);

        //Le gla�on est d�truit de la banquise puis de la m�moire
        banquise->grid[previous_line][previous_col].object = NULL;
        free(bumped_flake);
    }


    /* ... Ou pas */
    else
    {
        //Annulation de la vitesse du gla�on
        bumped_flake->flake->vect.d_col = 0;

        //Modification de la ligne o� se trouve � pr�sent le gla�on
        bumped_flake->flake->pos.col = new_col;

        //Suppression du gla�on sur la banquise, � sa position pr�c�dente
        banquise->grid[previous_line][previous_col].object = NULL;

        //Ajout du gla�on sur la banquise, sur sa nouvelle position calcul�e
        banquise->grid[previous_line][new_col].object = bumped_flake;
    }
}
