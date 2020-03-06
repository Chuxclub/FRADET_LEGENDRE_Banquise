#include <stdlib.h>

#include "utils.h"

//Teste si une case est disponible pour y placer un objet interactif ou un joueur
//Renvoit 1 si la case est disponible, 0 sinon
int IsCaseAvailable(T_case banquise_case)
{
    if((banquise_case.ground == ice && banquise_case.object == NULL) && (banquise_case.flag == no_flag && banquise_case.player == NULL))
        return 1;

    else
        return 0;
}

int IsFlake(T_case banquise_case)
{
    if(banquise_case.object->object_type == flake)
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

int IsObject(T_case banquise_case)
{
    if(banquise_case.object != NULL)
        return 1;

    else
        return 0;
}

int IsSpring(T_case banquise_case)
{
    if(banquise_case.object != NULL && banquise_case.object->object_type == spring)
        return 1;

    else
        return 0;
}

int IsWater(T_case banquise_case)
{
    if(banquise_case.ground == water)
        return 1;

    else
        return 0;
}


int IsFlakeIN(int banquise_size, T_banquise *banquise, int neighbour_line, int neighbour_col)
{
    if(IsInbound(BANQUISE_SIZE, neighbour_line, neighbour_col))
    {
        if (IsWater(banquise->grid[neighbour_line][neighbour_col]) || IsSpring(banquise->grid[neighbour_line][neighbour_col]))
            return 1;
    }

    else
        return 0;
}




