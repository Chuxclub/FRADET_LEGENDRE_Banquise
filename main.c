#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "banquise.h"
#include "glacon.h"
#include "joueur.h"
#include "piege.h"
#include "ressort.h"
#include "menus.h"


int main()
{
    srand(time(NULL));

    //Menu d'accueil, demande un nombre de joueurs
    int nb_players = main_menu();
    T_player **players = initPlayers(nb_players);



    //Initialisation de la banquise (que de la glace)
    T_flag_test *flag_test = initTabFlag();
    T_banquise *myBanquise = initBanquise(BANQUISE_SIZE, flag_test);

    //Ajout des joueurs sur la banquise
    addPlayers(myBanquise, players, nb_players);

    //Initialisation et ajout des objets sur la banquise
    T_object **flakes = initFlakes(NB_FLAKES);
    addFlakes(myBanquise, flakes, NB_FLAKES);

    T_object **traps = initTraps(NB_TRAPS);
    addTraps(myBanquise, traps, NB_TRAPS);

    T_object **springs = initSprings(NB_SPRINGS);
    addSprings(myBanquise, springs, NB_SPRINGS);

    //Rassemblement des parties du jeu pour les fonctions admins utiles aux tests
    T_game_parts game_parts;
    game_parts.banquise = myBanquise;
    game_parts.players = players;
    game_parts.flakes = flakes;
    game_parts.springs = springs;
    game_parts.traps = traps;

    //Copie de la banquise pour tester isARoad
    /*T_banquise *testBanquise = myBanquise;
    isARoad(*testBanquise, flag_test);
    a_test(flag_test->find2);*/


    //Affichage initial
    printBanquise(myBanquise);



   //Boucle du jeu
    char move;
    int end = 0;

    while(!(end))
    {
        for(int i = 0; i < nb_players; i++)
        {

            //Mise à jour du déplacement du joueur par le joueur
            getchar();
            scanf("%c", &move);

            switch(move)
            {
                case 'z':
                    moveUp(players[i], myBanquise);
                    break;

                case 'q':
                    moveLeft(players[i], myBanquise);
                    break;

                case 's':
                    moveDown(players[i], myBanquise);
                    break;

                case 'd':
                    moveRight(players[i], myBanquise);
                    break;

                case 'p':
                    break;

                case '$':
                    adminPrivileges(game_parts);
                    break;

                default:
                    perror("wrong input");
                    exit(EXIT_FAILURE);
                    break;

            }

            //Mise à jour, par le calcul, des positions des objets mouvants (s'il n'y a pas eu de commandes admin...
            if(move != '$')
                updateFlakes(NB_FLAKES, flakes, myBanquise);

            //Rafraîchissement banquise
            printBanquise(myBanquise);
        }
    }

    return 0;
}
