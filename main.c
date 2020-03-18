#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
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
    /* Initialisation de la dimension de la console par rapport à la résolution de l'écran du joueur (cf. windows.h) */
    HWND wh = GetConsoleWindow(); // Recuperation de la console windows dans laquelle le jeu est affiché
    DWORD dwWidth = GetSystemMetrics(SM_CXSCREEN); // Recuperation de la resolution de l'écran de l'utilisateur (la largeur)
    DWORD dwHeight = GetSystemMetrics(SM_CYSCREEN); //Meme chose mais pour la longueur
    MoveWindow(wh, 0, 0, dwWidth, dwHeight, TRUE); // Agrandissement de la taille de la console

    /* Initialisation du jeu */
    srand(time(NULL));

    int nb_players = main_menu();
    T_game_parts theGame = initGame(nb_players);

    //Affichage initial
    system("@cls||clear");
    printBanquise(theGame.banquise);


    /* Boucle du jeu */
    T_end_game_type end_game_type = salvation;
    char move;
    bool wrong_input;

    while(theGame.game_on)
    {
        /* On v�rifie si au moins un joueur est encore en vie. Si c'est le cas, le jeu continue */
        theGame.game_on = false;

        for(int i = 0; i < nb_players; i++)
        {
            if(theGame.players[i]->details.health == full_health)
                theGame.game_on = true;
        }

        //Si tout le monde est mort on a une fin du type "boneyard", fosse commune
        if(theGame.game_on == false)
            end_game_type = boneyard;

        /* Boucle d'input au tour par tour des joueurs encore en vie */
        for(int i = 0; i < nb_players; i++)
        {
            //Si le joueur i est mort on saute son tour
            if(theGame.players[i]->details.health == dead)
                continue;

                do
                {
                    wrong_input = false;

                    //Mise � jour du deplacement du joueur par le joueur
                    fflush(stdin);
                    scanf("%c", &move);

                    switch(move)
                    {
                        case 'z':
                            moveUp(theGame.players[i], &theGame);
                            break;

                        case 'q':
                            moveLeft(theGame.players[i], &theGame);
                            break;

                        case 's':
                            moveDown(theGame.players[i], &theGame);
                            break;

                        case 'd':
                            moveRight(theGame.players[i], &theGame);
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


            //Si apr�s un d�placement le jeu est fini, on termine le jeu
            if(theGame.game_on == false)
            {
                end_game_type = salvation;
                break;
            }


            //Si commande administrateur: pas de mise � jour des elements du jeu
            //mais affichage banquise pour donner des informations au debuggeur
            else if(move == '$')
                printBanquise(theGame.banquise);

            else
            {
                updateFlakes(NB_FLAKES, theGame.flakes, theGame.banquise);
                //updateHammers(NB_HAMMERS, theGame.hammers, theGame.banquise);
                updateHammers(NB_HAMMERS, &theGame);
                Fontebanquise(theGame.banquise);
                //road(theGame, nb_players);

                //Rafraichissement banquise
                system("@cls||clear"); //Commenter cette ligne pour faciliter les tests!
                printBanquise(theGame.banquise);
            }
        }
    }

    /* Fins alternatives */
    if(end_game_type == boneyard)
        printf("\nTous les joueurs sont morts, l'antarctique a ete sans pitie... \n");

    else if(end_game_type == salvation)
        printf("\nUn des joueurs a reussi a s'enfuir et a contacter (hypocritement ou non) les secours!\n");


    return 0;
}
