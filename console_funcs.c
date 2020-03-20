#include "console_funcs.h"

/*
    Auteur(e)(s): Florian Legendre
    Utilit�: Centrer horizontalement un affichage effectu� sur la ligne qui suit cette fonction (pourvu qu'il n'y ait pas de
             \n entre center_printf_col() et le printf() qui suit.
    Fonctionnement: Affiche un espace autant de fois que n�cessaire pour centrer horizontalement l'affichage qui suivra sur la m�me ligne.
    Complexit� en temps (au pire): O(width), il s'agit donc d'une complexit� lin�aire.
    Hypoth�se d'am�lioration possible: /
*/
void center_printf_col(int width)
{
    for(int i = 0; i < width; i++)
        printf(" ");
}


/*
    Auteur(e)(s): Florian Legendre
    Utilit�: Centrer verticalement un affichage effectu� sur la ligne qui suit cette fonction (pourvu qu'il n'y ait pas de
             \n entre center_printf_row() et le printf() qui suit.
    Fonctionnement: Affiche un saut de ligne autant de fois que n�cessaire pour centrer verticalement l'affichage qui suivra.
    Complexit� en temps (au pire): O(height), il s'agit donc d'une complexit� lin�aire.
    Hypoth�se d'am�lioration possible: /
*/
void center_printf_row(int height)
{
    for(int i = 0; i < height; i++)
        printf("\n");
}
