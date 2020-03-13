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
    bool wrong_input;

    while(theGame.game_on)
    {
        /* On vérifie si au moins un joueur est encore en vie. Si c'est le cas, le jeu continue */
        theGame.game_on = false;

        for(int i = 0; i < nb_players; i++)
        {
            if(theGame.players[i]->details.health == full_health)
                theGame.game_on = true;
        }

        /* Boucle d'input au tour par tour des joueurs encore en vie */
        for(int i = 0; i < nb_players; i++)
        {
            //Si le joueur i est mort on saute son tour
            if(theGame.players[i]->details.health == dead)
                continue;

                do
                {
                    wrong_input = false;

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

                        case 'l':
                            exit(EXIT_SUCCESS);
                            break;

                        case '$':
                            adminPrivileges(theGame);
                            break;

                        default:
                            printf("Wrong input, please type in a valid command ('z', 'q', 's', 'd', 'p', 'l'): \n");
                            wrong_input = true;
                            break;

                    }

                } while(wrong_input);


            //Si commande administrateur: pas de mise à jour des elements du jeu
            //mais affichage banquise pour donner des informations au debuggeur
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
