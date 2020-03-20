#include "console_funcs.h"

/*
    Auteur(e)(s):
    Utilité:
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
void center_printf_col(int width)
{
    for(int i = 0; i < width; i++)
        printf(" ");
}


/*
    Auteur(e)(s):
    Utilité:
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
void center_printf_row(int height)
{
    for(int i = 0; i < height; i++)
        printf("\n");
}
