#include "menus.h"



/*
    Auteur(e)(s): Florian Legendre
    Utilité: Affiche le menu d'accueil du jeu
    Fonctionnement: Affiche le titre en ASCII Art ("Banquise Game") en le centrant puis fait appel à HowManyPlayers()
                    qui est ainsi intégré à ce menu d'accueil. La fonction renvoit le nombre de joueurs pour renseigner
                    game_parts (cf. la "boite" du jeu)
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: /
*/
int main_menu()
{
  /* Recuperation des données de la console pour centrage de l'affichage par rapport aux colonnes/lignes */
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  int columns, rows;

  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
  rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

  /* Centrage par rapport aux lignes (axe vertical) */
  center_printf_row(rows / 6);

  /* Centrage de chaque ligne de printf (cf. titre du jeu ici) par rapport à l'axe horizontal */
  center_printf_col(columns / 2 - (BANQUISE_SIZE * 2 + 4));
  printf(" ____                          _             _____\n");

  center_printf_col(columns / 2 - (BANQUISE_SIZE * 2 + 4));
  printf("|  _ \\                        (_)           / ____|\n");

  center_printf_col(columns / 2 - (BANQUISE_SIZE * 2 + 4));
  printf("| |_) | __ _ _ __   __ _ _   _ _ ___  ___  | |  __  __ _ _ __ ___   ___ \n");

  center_printf_col(columns / 2 - (BANQUISE_SIZE * 2 + 4));
  printf("|  _ < / _` | '_ \\ / _` | | | | / __|/ _ \\ | | |_ |/ _` | '_ ` _ \\ / _ \\ \n");

  center_printf_col(columns / 2 - (BANQUISE_SIZE * 2 + 4));
  printf("| |_) | (_| | | | | (_| | |_| | \\__ \\  __/ | |__| | (_| | | | | | |  __/ \n");

  center_printf_col(columns / 2 - (BANQUISE_SIZE * 2 + 4));
  printf("|____/ \\__,_|_| |_|\\__, |\\__,_|_|___/\\___|  \\_____|\\__,_|_| |_| |_|\\___| \n");

  center_printf_col(columns / 2 - (BANQUISE_SIZE * 2 + 4));
  printf("                      | |                                               \n");

  center_printf_col(columns / 2 - (BANQUISE_SIZE * 2 + 4));
  printf("                      |_|                                               \n\n");

    //Initialisation des joueurs
    int nb_players = HowManyPlayers();

    if (nb_players == 0)
        exit(EXIT_SUCCESS);

    else
        return nb_players;
}
