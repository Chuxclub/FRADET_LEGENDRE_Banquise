#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "banquise.h"
#include "glacon.h"
#include "joueur.h"
#include "piege.h"
#include "ressort.h"


int main()
{
    srand(time(NULL));

    T_flag_test *flag_test = initTabFlag();


    //Initialisation de la banquise (que de la glace)
    T_banquise *myBanquise = initBanquise(BANQUISE_SIZE, flag_test);


    //Initialisation des objets
    T_object **flakes = initFlakes(NB_FLAKES);
    addFlakes(myBanquise, flakes, NB_FLAKES);

    T_object **traps = initTraps(NB_TRAPS);
    addTraps(myBanquise, traps, NB_TRAPS);

    T_object **springs = initSprings(NB_SPRINGS);
    addSprings(myBanquise, springs, NB_SPRINGS);

    //Copie de la banquise pour tester isARoad
    /*T_banquise *testBanquise = myBanquise;
    isARoad(*testBanquise, flag_test);
    a_test(flag_test->find2);*/

    //Initialisation des joueurs
    int nb_players = HowManyPlayers();
    T_player **players = initPlayers(nb_players);
    addPlayers(myBanquise, players, nb_players);


    //Affichage initial
    printBanquise(myBanquise);



   //D�but du jeu
    char move;
    int end = 0;

    while(!(end))
    {
        for(int i = 0; i < nb_players; i++)
        {
            getchar();
            scanf("%c", &move);

            switch(move)
            {
                case 'z':
                    moveUp(players[i], myBanquise);
                    printBanquise(myBanquise);
                    break;

                case 'q':
                    moveLeft(players[i], myBanquise);
                    printBanquise(myBanquise);
                    break;

                case 's':
                    moveDown(players[i], myBanquise);
                    printBanquise(myBanquise);
                    break;

                case 'd':
                    moveRight(players[i], myBanquise);
                    printBanquise(myBanquise);
                    break;

                default:
                    perror("wrong input");
                    exit(EXIT_FAILURE);
                    break;
            }
        }

    }

    return 0;
}
