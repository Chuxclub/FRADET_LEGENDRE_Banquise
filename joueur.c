#include "joueur.h"


/* ============================================ */
/* ========== INITIALISATION JOUEURS ========== */
/* ============================================ */

T_player **initPlayers(int nb_players)
{
    T_player **players = (T_player **) malloc(sizeof(T_player *) * nb_players);

    for(int i = 0; i < nb_players; i++)
        players[i] = (T_player *) malloc(sizeof(T_player));


    for(int i = 0; i < nb_players; i++)
    {
        players[i]->id = (i + 1);
        players[i]->details.freedom = player_free;
        players[i]->details.health = full_health;
    }

    return players;
}


//Recherche une position disponible dans une zone de recherche
//Que la fonction incr�mente au fur et � mesure, s'arr�te quand toute la banquise a �t� explor�e ou qu'une position a �t� trouv�e
int *searchAvailablePos(T_banquise *banquise, int Ligne_a, int Col_a)
{
    /*Initialisation des constantes et variables n�cessaires*/
    //pos_tab correspond aux positions qui seront
    //dans les limites du tableau
    int dist_A = 0;
    int found = 0;
    int ligne_max = BANQUISE_SIZE - 1;
    int col_max  = BANQUISE_SIZE - 1;
    int col_min  = 0;
    int *pos_tab = (int *) malloc(sizeof(int) * NB_OF_COORDINATES);

    do
    {
        /*D�finition des limites de la zone de recherche*/
        //Positionnement des indices lignes/colonnes par rapport � A (Col_a, Ligne_a)
        //Et par rapport au rayon de recherche autour de A (dist_A)
        //Rectification de ces indices si ces-derniers d�passent le plateau de jeu
        int col_begin = Col_a - dist_A;
        int col_end = Col_a + dist_A;
        int ligne_begin = Ligne_a - dist_A;
        int ligne_end = Ligne_a + dist_A;

        while(ligne_end > ligne_max)
            ligne_end--;

        while(col_begin < col_min)
            col_begin++;

        while(col_end > col_max)
            col_end--;


        /*Balayage de la zone de recherche du haut en bas, de la gauche vers la droite*/
        //Sauvegarde des positions dans le tableau au fur et � mesure de la recherche et que celles-ci sont libres
        //Sauvegarde de la taille du tableau avec la variable size au fur et � mesure qu'on rajoute des positions
        for(int ligne_index = ligne_begin; ligne_index <= ligne_end; ligne_index++)
        {
            for(int col_index = col_begin; col_index <= col_end; col_index++)
            {
                if(IsPlacementAvailable(banquise->grid[ligne_index][col_index]))
                {
                    pos_tab[0] = ligne_index;
                    pos_tab[1] = col_index;
                    found = 1;
                    break;
                }
            }

            if(found == 1)
                break;
        }

        /*Extension de la zone et boucle si aucune position de trouv�e*/
        if(found == 0)
            dist_A++;

        /*Arr�t de la recherche si on est parti du coin haut gauche du plateau et qu'on n'a rien trouv� par balayage*/
        if((col_begin == 0 && ligne_begin == 0) && found == 0)
        {
            fprintf(stderr, "No available position to place player\(s)\n");
            exit(EXIT_FAILURE);
        }
    }while(found == 0);

    return pos_tab;
}


//Ajoute les joueurs au plus pr�s du point A sur la banquise
//Renvoit les positions où elle a ajouté les joueurs
void addPlayers(T_banquise *banquise, T_player **players, int nb_players)
{
    /*Recherche point A (rappel: ne peut �tre que dans les trois derni�res lignes)*/
    int Col_a = 0;
    int Ligne_a = 0;

    for(int line = BANQUISE_SIZE - 1; line > BANQUISE_SIZE - 4; line--)
    {
        for(int col = 0; col < BANQUISE_SIZE; col++)
        {
            if(banquise->grid[line][col].flag == A)
            {
                Col_a = col;
                Ligne_a = line;
            }
        }
    }


    /*Positionnement des joueurs*/
    //Boucle -> recherche d'une position la plus proche, ajout joueur, etc.
    for(int i = 0; i < nb_players; i++)
    {
        int *found_pos = searchAvailablePos(banquise, Ligne_a, Col_a);
        int available_line = found_pos[0];
        int available_col = found_pos[1];

        /* On mémorise dans le joueur sa position... */
        players[i]->details.pos.line = available_line;
        players[i]->details.pos.col = available_col;

        /* Puis on assigne le joueur à la banquise, à la position correspondante */
        banquise->grid[available_line][available_col].player = players[i];
        free(found_pos);
    }
}


//demande et stocke le nombre de joueur
int HowManyPlayers()
{
    int nb_players;
    nb_players = 0;

    printf("Combien de joueurs etes-vous ?\n\n");
    printf("-> 0 pour quitter le jeu\n");
    printf("-> 1 joueur\n");
    printf("-> 2 joueurs\n");
    printf("-> 3 joueurs\n");
    printf("-> 4 joueurs\n\n");

    scanf("%d", &nb_players);

    //Dans le cas où l'input ne correspond pas à un bon nombre de joueur
    //Ou à l'input correspondant à une sortie du jeu immédiate
    while(nb_players < 0 || nb_players > 4)
    {
        printf("Veuillez entrer une valeur correcte (1 ou 2 ou 3 ou 4) : ");
        scanf("%d", &nb_players);
    }

    return nb_players;
}



/* ============================================ */
/* =========== DEPLACEMENT JOUEURS ============ */
/* ============================================ */
void moveUp(T_player *player, T_banquise *banquise)
{
    /* Passage du joueur en paramètre pour récupérer sa position */
    int previous_line = player->details.pos.line;
    int previous_col = player->details.pos.col;
    int new_line = previous_line - 1;
    T_vector up_vect = {-1, 0};

    if(player->details.freedom != player_free)
    {
        player->details.freedom++;
        return;
    }

    if(IsInbound(banquise->sizeB, new_line, previous_col))
    {
        if(IsCaseAvailable(banquise->grid[new_line][previous_col]))
        {
            /* Modification de la position du joueur */
            player->details.pos.line = new_line;
            player->details.pos.col = previous_col;

            /* Assignation à la case voulue du joueur sur la banquise*/
            updatePlayer(player, previous_line, previous_col, banquise);
        }

        else
        {
            if(IsObject(banquise->grid[new_line][previous_col]))
            {
                if(IsFlake(banquise->grid[new_line][previous_col]) && (scalar_product(banquise->grid[new_line][previous_col].object->flake->vect, up_vect) >= 0))
                    accelerateUp(banquise->grid[new_line][previous_col].object);

                else if(IsFlake(banquise->grid[new_line][previous_col]) && (scalar_product(banquise->grid[new_line][previous_col].object->flake->vect, up_vect) < 0))
                {
                    accelerateUp(banquise->grid[new_line][previous_col].object);
                    player->details.health = dead;
                }


                if(IsTrap(banquise->grid[new_line][previous_col]))
                {
                    /* Modification de la position du joueur */
                    player->details.pos.line = new_line;
                    player->details.pos.col = previous_col;

                    /* Assignation à la case voulue du joueur sur la banquise*/
                    updatePlayer(player, previous_line, previous_col, banquise);

                    /* Modification du statut 'freedom' du joueur */
                    player->details.freedom = player_trapped;
                }
            }
        }
    }

}

void moveLeft(T_player *player, T_banquise *banquise)
{
    if(player->details.freedom != player_free)
    {
        player->details.freedom++;
        return;
    }

    /* Passage du joueur en paramètre pour récupérer sa position */
    int previous_line = player->details.pos.line;
    int previous_col = player->details.pos.col;
    int new_col = previous_col - 1;
    T_vector left_vect = {0, -1};

    if(IsInbound(banquise->sizeB, previous_line, new_col))
    {
        if(IsCaseAvailable(banquise->grid[previous_line][new_col]))
        {
            /* Modification de la position du joueur */
            player->details.pos.line = previous_line;
            player->details.pos.col = new_col;

            /* Assignation à la case voulue du joueur sur la banquise*/
            updatePlayer(player, previous_line, previous_col, banquise);
        }

        else
        {
            if(IsObject(banquise->grid[previous_line][new_col]))
            {
                if(IsFlake(banquise->grid[previous_line][new_col]) && (scalar_product(banquise->grid[previous_line][new_col].object->flake->vect, left_vect) >= 0))
                    accelerateLeft(banquise->grid[previous_line][new_col].object);

                else if(IsFlake(banquise->grid[previous_line][new_col]) && (scalar_product(banquise->grid[previous_line][new_col].object->flake->vect, left_vect) < 0))
                {
                    accelerateLeft(banquise->grid[previous_line][new_col].object);
                    player->details.health = dead;
                }

                if(IsTrap(banquise->grid[previous_line][new_col]))
                {
                    /* Modification de la position du joueur */
                    player->details.pos.line = previous_line;
                    player->details.pos.col = new_col;

                    /* Assignation à la case voulue du joueur sur la banquise*/
                    updatePlayer(player, previous_line, previous_col, banquise);

                    /* Modification du statut 'freedom' du joueur */
                    player->details.freedom = player_trapped;
                }
            }
        }
    }
}

void moveDown(T_player *player, T_banquise *banquise)
{
    /* Passage du joueur en paramètre pour récupérer sa position */
    int previous_line = player->details.pos.line;
    int previous_col = player->details.pos.col;
    int new_line = previous_line + 1;
    T_vector down_vect = {1, 0};

    if(player->details.freedom != player_free)
    {
        player->details.freedom++;
        return;
    }


    if(IsInbound(banquise->sizeB, new_line, previous_col))
    {
        if(IsCaseAvailable(banquise->grid[new_line][previous_col]))
        {
            /* Modification de la position du joueur */
            player->details.pos.line = new_line;
            player->details.pos.col = previous_col;

            /* Assignation à la case voulue du joueur sur la banquise*/
            updatePlayer(player, previous_line, previous_col, banquise);
        }

        else
        {
            if(IsObject(banquise->grid[new_line][previous_col]))
            {
                if(IsFlake(banquise->grid[new_line][previous_col]) && (scalar_product(banquise->grid[new_line][previous_col].object->flake->vect, down_vect) >= 0))
                    accelerateDown(banquise->grid[new_line][previous_col].object);

                else if(IsFlake(banquise->grid[new_line][previous_col]) && (scalar_product(banquise->grid[new_line][previous_col].object->flake->vect, down_vect) < 0))
                {
                    accelerateDown(banquise->grid[new_line][previous_col].object);
                    player->details.health = dead;
                }

                if(IsTrap(banquise->grid[new_line][previous_col]))
                {
                    /* Modification de la position du joueur */
                    player->details.pos.line = new_line;
                    player->details.pos.col = previous_col;

                    /* Assignation à la case voulue du joueur sur la banquise*/
                    updatePlayer(player, previous_line, previous_col, banquise);

                    /* Modification du statut 'freedom' du joueur */
                    player->details.freedom = player_trapped;
                }
            }
        }
    }
}

void moveRight(T_player *player, T_banquise *banquise)
{
    /* Passage du joueur en paramètre pour récupérer sa position */
    int previous_line = player->details.pos.line;
    int previous_col = player->details.pos.col;
    int new_col = previous_col + 1;
    T_vector right_vect = {0, 1};

    if(player->details.freedom != player_free)
    {
        player->details.freedom++;
        return;
    }

    if(IsInbound(banquise->sizeB, previous_line, new_col))
    {
        if(IsCaseAvailable(banquise->grid[previous_line][new_col]))
        {
            /* Modification de la position du joueur */
            player->details.pos.line = previous_line;
            player->details.pos.col = new_col;

            /* Assignation à la case voulue du joueur sur la banquise*/
            updatePlayer(player, previous_line, previous_col, banquise);
        }

        else
        {
            if(IsObject(banquise->grid[previous_line][new_col]))
            {
                if(IsFlake(banquise->grid[previous_line][new_col]) && (scalar_product(banquise->grid[previous_line][new_col].object->flake->vect, right_vect) >= 0))
                    accelerateRight(banquise->grid[previous_line][new_col].object);

                else if(IsFlake(banquise->grid[previous_line][new_col]) && (scalar_product(banquise->grid[previous_line][new_col].object->flake->vect, right_vect) < 0))
                {
                    accelerateLeft(banquise->grid[previous_line][new_col].object);
                    player->details.health = dead;
                }

                if(IsTrap(banquise->grid[previous_line][new_col]))
                {
                    /* Modification de la position du joueur */
                    player->details.pos.line = previous_line;
                    player->details.pos.col = new_col;

                    /* Assignation à la case voulue du joueur sur la banquise*/
                    updatePlayer(player, previous_line, previous_col, banquise);

                    /* Modification du statut 'freedom' du joueur */
                    player->details.freedom = player_trapped;
                }
            }
        }
    }
}


//Met à jour la banquise en fonction des nouvelles positions du joueur
//Les anciennes coordonnées du joueurs doivent être données
void updatePlayer(T_player *myPlayer, int previous_line, int previous_col, T_banquise *banquise)
{
    banquise->grid[previous_line][previous_col].player = NULL;
    banquise->grid[myPlayer->details.pos.line][myPlayer->details.pos.col].player = myPlayer;
}
