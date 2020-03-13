#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "banquise.h"
#include "glacon.h"
#include "joueur.h"
#include "piege.h"
#include "ressort.h"
#include "menus.h"
#include "marteau.h"
#include "jeu.h"


int main()
{
    srand(time(NULL));

    int nb_players = main_menu();
    T_game_parts theGame = initGame(nb_players);

    //Affichage initial
    system("@cls||clear");
    printBanquise(theGame.banquise);

   //Boucle du jeu
    char move;
    int end = 0;

    while(!(end))
    {
        end = 1;

        for(int i = 0; i < nb_players; i++)
        {
            if(theGame.players[i]->details.health == full_health)
                end = 0;
        }

        for(int i = 0; i < nb_players; i++)
        {
            if(theGame.players[i]->details.health == dead)
                continue;

            //Mise à jour du dï¿½placement du joueur par le joueur
            fflush(stdin);
            scanf("%c", &move);


            switch(move)
            {
                case 'z':
                    moveUp(theGame.players[i], theGame.banquise);
                    break;

                case 'q':
                    moveLeft(theGame.players[i], theGame.banquise);
                    break;

                case 's':
                    moveDown(theGame.players[i], theGame.banquise);
                    break;

                case 'd':
                    moveRight(theGame.players[i], theGame.banquise);
                    break;

                case 'p':
                    theGame.players[i]->details.freedom++;

                    if(theGame.players[i]->details.freedom > 4)
                        theGame.players[i]->details.freedom = 4;

                    break;

                case '$':
                    adminPrivileges(theGame);
                    break;

                default:
                    perror("wrong input");
                    exit(EXIT_FAILURE);
                    break;

            }
            //Fontebanquise(myBanquise);
            //Mise à jour, par le calcul, des positions des objets mouvants (s'il n'y a pas eu de commandes admin...
            if(move != '$')
            {
                updateFlakes(NB_FLAKES, theGame.flakes, theGame.banquise);
                updateHammers(NB_HAMMERS, theGame.hammers, theGame.banquise);

                //Rafraîchissement banquise
                system("@cls||clear"); //Commenter cette ligne pour faciliter les tests!
                printBanquise(theGame.banquise);
            }


        }
        //printBanquise(myBanquise);
    }

    printf("\n Game Over ;) !\n");
    return 0;
}


/*
    //Menu d'accueil, demande un nombre de joueurs
    int nb_players = main_menu();
    T_player **players = initPlayers(nb_players);

    //Initialisation de la banquise (que de la glace)
    T_banquise *myBanquise = initBanquise(BANQUISE_SIZE);


    //Initialisation et ajout des objets sur la banquise
    T_object **hammers = initHammers(NB_HAMMERS);
    addHammers(myBanquise, hammers, NB_HAMMERS);

    T_object **flakes = initFlakes(NB_FLAKES);
    addFlakes(myBanquise, flakes, NB_FLAKES);

    T_object **traps = initTraps(NB_TRAPS);
    addTraps(myBanquise, traps, NB_TRAPS);

    T_object **springs = initSprings(NB_SPRINGS);
    addSprings(myBanquise, springs, NB_SPRINGS);


    //VÃ©rification d'un passage de A vers B
    T_test T = collectInfos(myBanquise, initTest(BANQUISE_SIZE));
    T_pos *tab = initTab();
    isRoad(T, T.posA.line, T.posA.col, tab, 0);


    //Ajout des joueurs sur la banquise
    addPlayers(myBanquise, players, nb_players);

    //Rassemblement des parties du jeu pour les fonctions admins utiles aux tests
    T_game_parts game_parts;
    game_parts.banquise = myBanquise;
    game_parts.players = players;
    game_parts.flakes = flakes;
    game_parts.springs = springs;
    game_parts.traps = traps;
//    game_parts.hammers = hammers;

*/
