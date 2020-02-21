#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
    int nb_players = HowManyPlayers();

    T_player **players = initPlayers(nb_players);

    addPlayers(myBanquise, players, nb_players);

    //Affichage
    printBanquise(myBanquise);


   //Début du jeu
    char move;
    int end = 0;

    while(!(end))
    {
        getchar();
        scanf("%c", &move);

        switch(move)
        {
            case 'z':
                moveUp(players[0], myBanquise);
                printBanquise(myBanquise);
                break;

            case 'q':
                moveLeft(players[0], myBanquise);
                printBanquise(myBanquise);
                break;

            case 's':
                moveDown(players[0], myBanquise);
                printBanquise(myBanquise);
                break;

            case 'd':
                moveRight(players[0], myBanquise);
                printBanquise(myBanquise);
                break;

            default:
                perror("wrong input");
                exit(EXIT_FAILURE);
                break;
        }

    }

    return 0;
}
