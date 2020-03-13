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

            //Mise à jour du deplacement du joueur par le joueur
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

            //Mise a jour, par le calcul, des positions des objets mouvants (s'il n'y a pas eu de commandes admin)...
            if(move == '$')
                printBanquise(theGame.banquise);

            else
            {
                updateFlakes(NB_FLAKES, theGame.flakes, theGame.banquise);
                updateHammers(NB_HAMMERS, theGame.hammers, theGame.banquise);
                Fontebanquise(theGame.banquise);

                //Rafraichissement banquise
                system("@cls||clear"); //Commenter cette ligne pour faciliter les tests!
                printBanquise(theGame.banquise);
            }
        }
    }

    printf("\n Game Over ;) !\n");
    return 0;
}
