#include "menus.h"

int main_menu()
{
    printf("\n\t\t\t\t# ============================================== #\n");
    printf("\t\t\t\t* =============== BANQUISE GAME  =============== *\n");
    printf("\t\t\t\t# ============================================== #\n\n");

    //Initialisation des joueurs
    int nb_players = HowManyPlayers();

    if (nb_players == 0)
        exit(EXIT_SUCCESS);

    else
        return nb_players;
}
