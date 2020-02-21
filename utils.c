#include <stdlib.h>
#include "banquise.h"

//Teste si une case est disponible pour y placer un objet interactif ou un joueur
//Renvoit 1 si la case est disponible, 0 sinon
int IsCaseAvailable(T_case banquise_case)
{
    if((banquise_case.ice == ice && banquise_case.object == no_object) && (banquise_case.A != 1 && banquise_case.B != 1) && banquise_case.player == NULL)
        return 1;

    else
        return 0;
}

int IsFlake(T_case banquise_case)
{
    if(banquise_case.object == flake)
        return 1;

    else
        return 0;
}

int IsInbound(int banquise_size, int line, int col)
{
    if((line >= 0 && line < banquise_size) && (col >= 0 && col < banquise_size))
        return 1;

    else
        return 0;
}
