//
// Created by florian on 13/02/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "banquise.h"
#include "windows_colors.h" //Ajoute des couleurs au terminal Windows
#include "joueur.h"
#include "ressort.h"
#include "utils.h"
#include "piege.h"

#define RAND_MAX 101
#define BANQUISE_SIZE 10
#define NB_OF_COORDINATES 2


/* ============================================ */
/* ========= INITIALISATION BANQUISE ========== */
/* ============================================ */

//Crée un simple tableau de glace uniquement
T_banquise *initRawBanquise(int size)
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
            res->grid[i][j].ice = ice;
            res->grid[i][j].player = no_player;
            res->grid[i][j].object = no_object;

            //res.grid[i][j].glacon.pos.px = 0;
            //res.grid[i][j].glacon.pos.py = 0;
            //res.grid[i][j].glacon.vecteur.dx = 0;
            //res.grid[i][j].glacon.vecteur.dy = 0;

            res->grid[i][j].A = 0;
            res->grid[i][j].B = 0;
        }

    }

    return res;
}

//Crée une banquise prête pour démarrer le jeu
T_banquise *initBanquise(int size)
{
    //Initialisation de la banquise (que de la glace)
    T_banquise *myBanquise = initRawBanquise(size);

    //Génération aléatoire des éléments du terrain
    addWater(myBanquise);
    addRocks(myBanquise);
    addFlakes(myBanquise);
    addSprings(myBanquise);
    addTraps(myBanquise);

    addFlags(myBanquise);
    addPlayers(myBanquise, 4);

    return myBanquise;
}


/* ============================================ */
/* ========== MODIFICATIONS BANQUISE ========== */
/* ============================================ */

//Ajoute de l'eau aléatoirement dans la banquise, remplace la glace
void addWater(T_banquise *banquise)
{
    for(int i = 0; i < banquise->size; i++)
    {
        for(int j = 0; j < banquise->size; j++)
        {
            int loto = rand() % RAND_MAX;

            if(loto < 15)
                banquise->grid[i][j].ice = water;
        }
    }
}

//Ajoute des rochers aléatoirement sur la banquise, remplace la glace
//Mais ne remplace pas l'eau! Code pour rocher: object = 1
void addRocks(T_banquise *banquise)
{
    for(int i = 0; i < banquise->size; i++)
    {
        for(int j = 0; j < banquise->size; j++)
        {
            int loto = rand() % RAND_MAX;

            if(loto < 25 && banquise->grid[i][j].ice == 1)
                banquise->grid[i][j].object = rock;
        }
    }
}




//Ajoute les points de départ et d'arrivé
//Le point d'arrivé ne peut être qu'à trois lignes du haut du départ
//Le point de départ dans les trois lignes en bas du tableau
void addFlags(T_banquise *banquise)
{
    int Xa = BANQUISE_SIZE - 1 - (rand() % 3);
    int Ya = rand() % BANQUISE_SIZE;

    int Xb = 0 + (rand() % 3);
    int Yb = rand() % BANQUISE_SIZE;

    //Si la position de A définie ci-dessus comporte un objet ou de l'eau on redéfinit jusqu'à
    //ce que ce soit bon!
    while(banquise->grid[Xa][Ya].object > 0 || banquise->grid[Xa][Ya].ice == 0)
    {
        Xa = BANQUISE_SIZE - 1 - (rand() % 3);
        Ya = rand() % BANQUISE_SIZE;
    }
    banquise->grid[Xa][Ya].A = 1;


    //Même chose pour B
    while((banquise->grid[Xb][Yb].object > 0 || banquise->grid[Xb][Yb].ice == 0))
    {
        Xb = 0 + (rand() % 3);
        Yb = rand() % BANQUISE_SIZE;
    }
    banquise->grid[Xb][Yb].B = 1;
}



/* ============================================ */
/* ================ AFFICHAGES ================ */
/* ============================================ */

//Affiche un code d'une case avec le symbole correspondant
//Suit ordre logique: A/B > object > player > ice
void printCase(T_case banquise_case)
{
    //Si c'est de l'eau inutile de chercher plus loins, on l'affiche
    if(banquise_case.ice == 0)
    {
        color(9, 9); //En bleu, fond bleu
        printf("~"); //text_blue(stdout)
        color(15, 0); //Couleur d'origine de la console = reset de la couleur
        printf(" | "); //text_white(stdout)
    }

    //S'il y a un objet, un drapeau ou un joueur on va l'afficher
    else if(banquise_case.object > 0 || banquise_case.A > 0 || banquise_case.B > 0 || banquise_case.player > 0)
    {
        //C'est soit un objet...
        if(banquise_case.object > 0)
        {
            switch(banquise_case.object)
            {
                case flake:
                    color(11, 0); //En turquoise
                    printf("*"); //text_yellow(stdout)
                    color(15, 0);
                    printf(" | "); //text_white(stdout)
                    break;

                case rock:
                    color(6, 6); //En kaki, fond kaki
                    printf("o"); //text_yellow(stdout)
                    color(15, 0);
                    printf(" | "); //text_white(stdout)
                    break;

                case spring:
                    color(2, 0); //En vert
                    printf("@"); //text_blue(stdout)
                    color(15, 0);
                    printf(" | "); //text_white(stdout)
                    break;

                case hammer:
                    color(2, 0); //En vert
                    printf("m"); //text_blue(stdout)
                    color(15, 0);
                    printf(" | "); //text_white(stdout)
                    break;

                case trap:
                    color(4, 0); //En marron
                    printf("!"); //text_blue(stdout)
                    color(15, 0);
                    printf(" | "); //text_white(stdout)
                    break;

                default:
                    perror("Wrong object value in printCase() in banquise.c");
                    exit(EXIT_FAILURE);
                    break;
            }
        }

        //Soit un drapeau
        else if(banquise_case.A > 0 || banquise_case.B > 0)
            {
               if(banquise_case.A == 1)
                {
                    color(5, 0);
                    printf("A"); //text_purple(stdout)
                    color(15, 0);
                    printf(" | "); //text_white(stdout)
                }

                else
                {
                    color(5, 0); //En violet
                    printf("B"); //text_purple(stdout)
                    color(15, 0);
                    printf(" | "); //text_white(stdout)
                }
            }

        //Soit un joueur
        else if(banquise_case.player > 0)
        {
            switch(banquise_case.player)
            {
                case player_1:
                    color(12, 0); //En rouge
                    printf("1"); //text_purple(stdout)
                    color(15, 0);
                    printf(" | "); //text_white(stdout)
                    break;

                case player_2:
                    color(12, 0);
                    printf("2"); //text_purple(stdout)
                    color(15, 0);
                    printf(" | "); //text_white(stdout)
                    break;

                case player_3:
                    color(12, 0);
                    printf("3"); //text_purple(stdout)
                    color(15, 0);
                    printf(" | "); //text_white(stdout)
                    break;

                case player_4:
                    color(12, 0);
                    printf("4"); //text_purple(stdout)
                    color(15, 0);
                    printf(" | "); //text_white(stdout)
                    break;

                default:
                    perror("Wrong player value in printCase() in banquise.c");
                    exit(EXIT_FAILURE);
                    break;
                }
        }
    }

    //S'il n'y a ni eau, ni objet/drapeau/joueur, c'est de la glace
    else
        {
            color(15, 15); //En blanc, fond blanc
            printf("#"); //text_bold(stdout)
            color(15, 0);
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




/* ============================================ */
/* =================== FONTE ================== */
/* ============================================ */

//Calcule la fonte de la banquise => Pour le besoin des tests, ne pas y faire appel maintenant
T_banquise Fontebanquise (T_banquise *banquise)
{
    srand(time (NULL));
    for (int i = 0; i < banquise->size; i++)
    {
        for (int j = 0; j < banquise->size; j++)
        {
            if ((banquise->grid[i][j].ice = 0) && (rand()%(16) < 16))
            {   //fond avec un pourcentage de 15%
                banquise->grid[i][j-1].ice = 0;  // toutes les cases autour de ce point d'eau
                banquise->grid[i][j+1].ice = 0;
                banquise->grid[i-1][j].ice = 0;
                banquise->grid[i+1][j].ice = 0;
                j += 2; //ajoute plus 2 Ã  j pour ne pas recommencer avec le point d'eau nouvellement crÃ©Ã©
            }
        }
    }

    return *banquise;
}
