#include <stdio.h>
#include <stdlib.h>
#include "banquise.h"
#include "joueur.h"
#include "glacon.h"
#include "banquise.h"
#include "glacon.h"

#define BANQUISE_SIZE 10
#define NB_OF_COORDINATES 2


//Recherche une position disponible dans une zone de recherche
//Que la fonction incrémente au fur et à mesure, s'arrête quand toute la banquise a été explorée ou qu'une position a été trouvée
int *searchAvailablePos(T_banquise *banquise, int Ligne_a, int Col_a)
{
    /*Initialisation des constantes et variables nécessaires*/
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
        /*Définition des limites de la zone de recherche*/
        //Positionnement des indices lignes/colonnes par rapport à A (Col_a, Ligne_a)
        //Et par rapport au rayon de recherche autour de A (dist_A)
        //Rectification de ces indices si ces-derniers dépassent le plateau de jeu
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
        //Sauvegarde des positions dans le tableau au fur et à mesure de la recherche et que celles-ci sont libres
        //Sauvegarde de la taille du tableau avec la variable size au fur et à mesure qu'on rajoute des positions
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

        /*Extension de la zone et boucle si aucune position de trouvée*/
        if(found == 0)
            dist_A++;

        /*Arrêt de la recherche si on est parti du coin haut gauche du plateau et qu'on n'a rien trouvé par balayage*/
        if((col_begin == 0 && ligne_begin == 0) && found == 0)
        {
            fprintf(stderr, "No available position to place player\(s)\n");
            exit(EXIT_FAILURE);
        }
    }while(found == 0);

    return pos_tab;
}

//Ajoute les joueurs au plus près du point A sur la banquise
void addPlayers(T_banquise *banquise, int nb_players)
{
    /*Recherche point A (rappel: ne peut être que dans les trois dernières lignes)*/
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
    for(int i = 1; i <= nb_players; i++)
    {
        int *found_pos = searchAvailablePos(banquise, Ligne_a, Col_a);
        int available_line = found_pos[0];
        int available_col = found_pos[1];
        banquise->grid[available_line][available_col].player = i;
        free(found_pos);
    }
}



/*
//variable stockant le nombre de joueur
int how_many = 0;

//demande et stocke le nombre de joueur
int HowManyPlayers()
{
    printf("Combien de joueurs êtes-vous ?\n");
    scanf("%d", &how_many);
    return how_many;
}

//
*/
