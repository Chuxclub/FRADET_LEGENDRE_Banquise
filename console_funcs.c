#include "console_funcs.h"

/*
    Auteur(e)(s):
    Utilit�:
    Fonctionnement:
    Complexit� en temps (au pire):
    Hypoth�se d'am�lioration possible:
*/
void center_printf_col(int width)
{
    for(int i = 0; i < width; i++)
        printf(" ");
}


/*
    Auteur(e)(s):
    Utilit�:
    Fonctionnement:
    Complexit� en temps (au pire):
    Hypoth�se d'am�lioration possible:
*/
void center_printf_row(int height)
{
    for(int i = 0; i < height; i++)
        printf("\n");
}
