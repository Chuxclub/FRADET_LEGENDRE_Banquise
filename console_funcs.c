#include "console_funcs.h"

/*
    Auteur(e)(s): Florian Legendre
    Utilité: Centrer horizontalement un affichage effectué sur la ligne qui suit cette fonction (pourvu qu'il n'y ait pas de
             \n entre center_printf_col() et le printf() qui suit.
    Fonctionnement: Affiche un espace autant de fois que nécessaire pour centrer horizontalement l'affichage qui suivra sur la même ligne.
    Complexité en temps (au pire): O(width), il s'agit donc d'une complexité linéaire.
    Hypothèse d'amélioration possible: /
*/
void center_printf_col(int width)
{
    for(int i = 0; i < width; i++)
        printf(" ");
}


/*
    Auteur(e)(s): Florian Legendre
    Utilité: Centrer verticalement un affichage effectué sur la ligne qui suit cette fonction (pourvu qu'il n'y ait pas de
             \n entre center_printf_row() et le printf() qui suit.
    Fonctionnement: Affiche un saut de ligne autant de fois que nécessaire pour centrer verticalement l'affichage qui suivra.
    Complexité en temps (au pire): O(height), il s'agit donc d'une complexité linéaire.
    Hypothèse d'amélioration possible: /
*/
void center_printf_row(int height)
{
    for(int i = 0; i < height; i++)
        printf("\n");
}
