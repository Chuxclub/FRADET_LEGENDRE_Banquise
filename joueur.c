#include <stdio.h>
#include <stdlib.h>
#include "joueur.h"
#include "glacon.h"
#include "banquise.h"
#include "glacon.h"
#include "utils.h"

#define BANQUISE_SIZE 10
#define NB_OF_COORDINATES 2


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
                if(IsCaseAvailable(banquise->grid[ligne_index][col_index]))
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

    for(int ligne = BANQUISE_SIZE - 1; ligne > BANQUISE_SIZE - 4; ligne--)
    {
        for(int col = 0; col < BANQUISE_SIZE; col++)
        {
            if(banquise->grid[ligne][col].A == 1)
            {
                Col_a = col;
                Ligne_a = ligne;
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

        players[i]->details.pos.py = available_line;
        players[i]->details.pos.px = available_col;

        banquise->grid[available_line][available_col].player = players[i];
        //printf("What is banquise_case.player->id ? %i\n", banquise->grid[available_line][available_col].player->id);
        free(found_pos);
    }
}


//demande et stocke le nombre de joueur
int HowManyPlayers()
{
    int nb_player;
    printf("Combien de joueurs etes-vous ?\n");
    scanf("%d", &nb_player);
    return nb_player;
}



/* ============================================ */
/* =========== DEPLACEMENT JOUEURS ============ */
/* ============================================ */

void moveUp(T_player *player, T_banquise *banquise)
{
    /* Passage du joueur en paramètre pour récupérer sa position */
    int previous_line = player->details.pos.py;
    int previous_col = player->details.pos.px;

    /* Assignation à la case voulue du joueur sur la banquise*/
    int new_line = previous_line - 1;
    banquise->grid[new_line][previous_col].player = player;
    banquise->grid[previous_line][previous_col].player = NULL;

    /* Modification de la position du joueur */
    player->details.pos.py = new_line;
    player->details.pos.px = previous_col;
}

void moveLeft(T_player *player, T_banquise *banquise)
{
    /* Passage du joueur en paramètre pour récupérer sa position */
    int previous_line = player->details.pos.py;
    int previous_col = player->details.pos.px;

    /* Assignation à la case voulue du joueur sur la banquise*/
    int new_col = previous_col - 1;
    banquise->grid[previous_line][new_col].player = player;
    banquise->grid[previous_line][previous_col].player = NULL;

    /* Modification de la position du joueur */
    player->details.pos.py = previous_line;
    player->details.pos.px = new_col;
}

void moveDown(T_player *player, T_banquise *banquise)
{
    /* Passage du joueur en paramètre pour récupérer sa position */
    int previous_line = player->details.pos.py;
    int previous_col = player->details.pos.px;

    /* Assignation à la case voulue du joueur sur la banquise*/
    int new_line = previous_line + 1;
    banquise->grid[new_line][previous_col].player = player;
    banquise->grid[previous_line][previous_col].player = NULL;

    /* Modification de la position du joueur */
    player->details.pos.py = new_line;
    player->details.pos.px = previous_col;
}

void moveRight(T_player *player, T_banquise *banquise)
{
    /* Passage du joueur en paramètre pour récupérer sa position */
    int previous_line = player->details.pos.py;
    int previous_col = player->details.pos.px;

    /* Assignation à la case voulue du joueur sur la banquise*/
    int new_col = previous_col + 1;
    banquise->grid[previous_line][new_col].player = player;
    banquise->grid[previous_line][previous_col].player = NULL;

    /* Modification de la position du joueur */
    player->details.pos.py = previous_line;
    player->details.pos.px = new_col;
}
//effectue le deplacement des jours pour un tour
/*T_banquise move(T, int nb_player)
{
    for (int i = 1; i <= nb_player; i++)
    {
        char move[1];
        printf("C'est au tour du joueur %d\n", i);
        scanf("%c", &move);
        switch(move[0])
        {
        case 'H':

            break;
        }

    }
}

*/
