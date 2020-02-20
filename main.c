#include <stdio.h>
#include <stdlib.h>
#include "banquise.h"
#include "glacon.h"
#include "piege.h"
#include "ressort.h"
#include "utils.h"
#include "joueur.h"

#define BANQUISE_SIZE 10
#define BANQUISE_CASES (BANQUISE_SIZE * BANQUISE_SIZE)

int main()
{
    srand(time(NULL));

    //Initialisation de la banquise (que de la glace)
    T_banquise *myBanquise = initBanquise(BANQUISE_SIZE);

    //Initialise une variable avec le nombre de joueurs
    int nb_player = HowManyPlayers();


    addPlayers(myBanquise, nb_player);

    //Affichage
    printBanquise(myBanquise);


 /*   //Début du jeu
    char move;
    int end = 0;

    do
    {
        switch(move)
        {
            case 'z':
                moveUp(player);
                break;

            case 'q':
                moveLeft(player);
                break;

            case 's':
                moveDown(player);
                break;

            case 'd':
                moveRight(player);
                break;

            default:
                perror("wrong input");
                exit(EXIT_FAILURE);
                break;
        }

    }while(scanf("%c", move) && !(end))
*/
    return 0;
}
