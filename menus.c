#include "menus.h"

int main_menu()
{
    printf("\n# ============================================== #\n");
    printf("* =============== BANQUISE GAME  =============== *\n");
    printf("# ============================================== #\n\n");

    //Initialisation des joueurs
    int nb_players = HowManyPlayers();

    if (nb_players == 0)
        exit(EXIT_SUCCESS);

    else
        return nb_players;
}
