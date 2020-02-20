#include <stdio.h>
#include <stdlib.h>
#include "joueur.h"

//demande et stocke le nombre de joueur
int HowManyPlayers()
{
    int nb_player;
    printf("Combien de joueurs êtes-vous ?\n");
    scanf("%d", &nb_player);
    return nb_player;
}
//effectue le deplacement des jours pour un tour
/*T_banquise move(T, int nb_player)
{
    for (int i = 1; i <= nb_player; i++)
    {
        char move[1];
        printf("C'est au tour du joueur %d\n", i);
        scanf("%c", &move);
        switch(move[0])
        {
        case 'H':

            break;
        }

    }
}

*/
