//
// Created by florian on 13/02/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "banquise.h"
//#include "termcolor.h"

#define RAND_MAX 101

#define BANQUISE_SIZE 10

//Crée un simple tableau de glace uniquement
T_banquise *initBanquise(int size)
{
    T_banquise *res;
    res = ( T_banquise* ) malloc(sizeof(T_banquise));

    res->size = size;
    res->grid = (T_case **) malloc(sizeof(T_case *) * size);

    for(int i = 0; i < size;  i++)
        res->grid[i] = (T_case *) malloc(sizeof(T_case) * size);

    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            res->grid[i][j].ice = 1;
            res->grid[i][j].player = 0;
            res->grid[i][j].object = 0;
            res->grid[i][j].A = 0;
            res->grid[i][j].B = 0;
        }

    }

    return res;
}

//Ajoute de l'eau aléatoirement dans la banquise, remplace la glace
void addWater(T_banquise *banquise)
{
    srand(time(NULL));

    for(int i = 0; i < banquise->size; i++)
    {
        for(int j = 0; j < banquise->size; j++)
        {
            int loto = rand() % RAND_MAX;

            if(loto < 15)
                banquise->grid[i][j].ice = 0;
        }
    }
}

//Ajoute des rochers aléatoirement sur la banquise, remplace la glace
//Mais ne remplace pas l'eau! Code pour rocher: object = 1
void addRocks(T_banquise *banquise)
{
    srand(time(NULL));

    for(int i = 0; i < banquise->size; i++)
    {
        for(int j = 0; j < banquise->size; j++)
        {
            int loto = rand() % RAND_MAX;

            if(loto < 20 && banquise->grid[i][j].ice == 1)
                banquise->grid[i][j].object = 2;
        }
    }
}

//Ajoute les points de départ et d'arrivé
//Le point d'arrivé ne peut être qu'à trois lignes du haut du départ
//Le point de départ dans les trois lignes en bas du tableau
void addFlags(T_banquise *banquise)
{
    srand(time(NULL));

    int Xa = BANQUISE_SIZE - 1 - (rand() % 3);
    int Ya = rand() % BANQUISE_SIZE;

    int Xb = 0 + (rand() % 3);
    int Yb = rand() % BANQUISE_SIZE;

    while(banquise->grid[Xa][Ya].object > 0 || banquise->grid[Xa][Ya].ice == 1)
    {
        Xa = BANQUISE_SIZE - 1 - (rand() % 3);
        Ya = rand() % BANQUISE_SIZE;
    }


    banquise->grid[Xa][Ya].A = 1;


    while((banquise->grid[Xa][Ya].object > 0 || banquise->grid[Xa][Ya].ice == 1))
    {
        Xb = 0 + (rand() % 3);
        Yb = rand() % BANQUISE_SIZE;
    }

    banquise->grid[Xb][Yb].B = 1;
}

int **searchInboundPos(T_banquise *banquise, int Ligne_a, int Col_a, int dist_A, int *size_pos)
{
    //Initialisation variables nécessaires
    //pos_tab correspond aux positions qui seront
    //dans les limites du tableau
    int ligne_max = BANQUISE_SIZE - 1;
    int col_max  = BANQUISE_SIZE - 1;
    int col_min  = 0;
    int **pos_tab = (int **) malloc(sizeof(int *) * BANQUISE_SIZE);

    for(int i = 0; i < BANQUISE_SIZE;  i++)
        pos_tab[i] = (int *) malloc(sizeof(int) * 2);

    //Recherche position dans le tableau à tester après
    //La première position est toujours valide compte tenu
    //De la définition de A dans les trois dernières lignes du tableau
    int i = 0;
    int col_begin = Col_a - dist_A;
    int col_end = Col_a + dist_A;
    int ligne_begin = Ligne_a - dist_A;
    int ligne_end = Ligne_a + dist_A;

    /*printf("ligne_begin: %i\n", ligne_begin);
    printf("ligne_end: %i\n", ligne_end);

    printf("col_begin: %i\n", col_begin);
    printf("col_end: %i\n\n", col_end);*/

    while(ligne_end > ligne_max)
        ligne_end--;

    /*printf("new_ligne_end: %i\n", ligne_end);*/

    while(col_begin < col_min)
            col_begin++;

    while(col_end > col_max)
        col_end--;

    /*printf("new_col_begin: %i\n", col_begin);
    printf("new_col_end: %i\n", col_end);*/

    //Balayage de la zone à dist_A du haut en bas, de la gauche vers la droite
    //Sauvegarde des positions dans le tableau au fur et à mesure de la recherche
    for(int ligne_index = ligne_begin; ligne_index <= ligne_end; ligne_index++)
    {
        //Positionnement du début et fin de la recherche
        //pour les indices colonnes
        for(int col_index = col_begin; col_index <= col_end; col_index++)
        {
            pos_tab[i][0] = ligne_index;
            pos_tab[i][1] = col_index;
            //printf("Ligne  = %i, Colonne = %i\n", pos_tab[i][0], pos_tab[i][1]);
            i++;
        }
    }

    *size_pos = i;
    return pos_tab;
}

void addPlayers(T_banquise *banquise)
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

    /*Recherche position disponible autour du point A*/
    int size_pos  = 0;
    int *size_pos_ptr = &size_pos;
    int **pos_tab = searchInboundPos(banquise, Ligne_a, Col_a, 1, size_pos_ptr);

    for(int i = 0; i < size_pos; i++)
        printf("Ligne  = %i, Colonne = %i\n", pos_tab[i][0], pos_tab[i][1]);

}
//Affiche un code d'une case avec le symbole correspondant
//Suit ordre logique: A/B > object > player > ice
void printCase(T_case banquise_case)
{
    if(banquise_case.A == 1)
    {
        printf("A"); //text_purple(stdout)
        printf(" | "); //text_white(stdout)
    }

    else if(banquise_case.B == 1)
    {
        printf("B"); //text_purple(stdout)
        printf(" | "); //text_white(stdout)
    }

    else if(banquise_case.object == 2)
    {
        printf("o"); //text_yellow(stdout)
        printf(" | "); //text_white(stdout)
    }

    else if(banquise_case.ice == 0)
    {
        printf("~"); //text_blue(stdout)
        printf(" | "); //text_white(stdout)
    }

    else
    {
        printf("#"); //text_bold(stdout)
        printf(" | "); //text_white(stdout)
    }
}

//Affiche l'état de la banquise à l'instant de son appel
void printBanquise(T_banquise *banquise)
{
    //Upper Border
    printf("\n");
    for(int i = 0; i < banquise->size * 4 + 1; i++)
        printf("-"); //, text_white(stdout)

    //Grid
    int counter = banquise->size;

    for(int i = 0; i < banquise->size; i++)
    {
        for(int j = 0; j < banquise->size; j++)
        {
            if(counter == banquise->size)
            {
                printf("\n");

                //Right border of grid
                printf("| "); //, text_white(stdout)
                counter = 0;
            }

            printCase(banquise->grid[i][j]);

            counter++;
        }

        //Mid-lines + Bottom border
        printf("\n");
        for(int i = 0; i < banquise->size * 4 + 1; i++)
            printf("-");
    }

    //Newline
    printf("\n");
}
