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
                break;

            case hammer_handle:
                printf("Object: hammer handle\n\n");
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
        printf("\t Player Id: %i\n", player->id);
        printf("\t Player Line: %i\n", player->details.pos.line);
        printf("\t Player Column: %i\n", player->details.pos.col);
    }

    printf("\n/!\\ =============================== /!\\ \n", line, col);
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

    scanf("%s %i %i", cmd, &line, &col);

    if(strcmp(cmd, "adminAddWater") == 0)
        adminAddWater(game_parts, line, col);

    else if(strcmp(cmd, "adminTpPlayer") == 0)
    {
        int player_id = 0;
        printf("\nIndicate which player you want to move: ");
        scanf("%i", &player_id);
        adminTpPlayer(game_parts, player_id - 1, line, col); //-1 pour pouvoir écrire adminTpPlayer 0 0 1 par exemple...
    }

    else if(strcmp(cmd, "adminTpFlake") == 0)
    {
        int flake_id = 0;
        printf("\nIndicate which flake you want to move: ");
        scanf("%i", &flake_id);
        adminTpFlake(game_parts, flake_id - 1, line, col); //-1 pour pouvoir écrire adminTpPlayer 0 0 1 par exemple...
    }

    else if(strcmp(cmd, "adminTpSpring") == 0)
    {
        int spring_id = 0;
        printf("\nIndicate which spring you want to move: ");
        scanf("%i", &spring_id);
        adminTpSpring(game_parts, spring_id - 1, line, col); //-1 pour pouvoir écrire adminTpPlayer 0 0 1 par exemple...
    }

    else if(strcmp(cmd, "adminInfoCase") == 0)
        adminInfoCase(game_parts, line, col);

    else
        return;
}



// ------> Fonctions de test "en code"
/* ============================================ */
/* =========== INTERACTIONS GLACONS =========== */
/* ============================================ */

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
