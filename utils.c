#include <stdlib.h>

#include "utils.h"

//Teste si une case est disponible pour y placer un objet interactif ou un joueur
//Renvoit 1 si la case est disponible, 0 sinon
int IsCaseAvailable(T_case banquise_case)
{
    if((banquise_case.ground == ice && (banquise_case.object == NULL || (banquise_case.object != NULL && banquise_case.object->object_type == reserved))) && (banquise_case.flag == no_flag && banquise_case.player == NULL))
        return 1;

    else
        return 0;
}

int IsPlacementAvailable(T_case banquise_case)
{
    if((banquise_case.ground == ice && (banquise_case.object == NULL)) && (banquise_case.flag == no_flag && banquise_case.player == NULL))
        return 1;

    else
        return 0;
}

int AreSurroundingsAvailable(T_banquise *banquise, int line, int col)
{
    int res = 1;
    int start_line = line-1;
    int stop_line = line+1;
    int start_col = col-1;
    int stop_col = col+1;

    for(int current_line = start_line; current_line <= stop_line; current_line++)
    {
        for(int current_col = start_col; current_col <= stop_col; current_col++)
        {
            if(!(IsInbound(banquise->sizeB, current_line, current_col) && IsPlacementAvailable(banquise->grid[current_line][current_col])))
                res = 0;
        }
    }

    return res;
}

void PlaceReservedZone(T_banquise *banquise, int line, int col)
{
    int start_line = line-1;
    int stop_line = line+1;
    int start_col = col-1;
    int stop_col = col+1;
    T_object *reserved_case = (T_object *) malloc(sizeof(T_object));
    reserved_case->object_type = reserved;
    reserved_case->flake = NULL;
    reserved_case->spring = NULL;
    reserved_case->hammer_handle = NULL;
    reserved_case->hammer_head = NULL;
    reserved_case->trap = NULL;

    for(int current_line = start_line; current_line <= stop_line; current_line++)
    {
        for(int current_col = start_col; current_col <= stop_col; current_col++)
        {
            if(IsInbound(banquise->sizeB, current_line, current_col) && IsCaseAvailable(banquise->grid[current_line][current_col]) && (!(current_col == col && current_line == line)))
            {
                banquise->grid[current_line][current_col].object = reserved_case;
            }
        }
    }
}

int IsFlake(T_case banquise_case)
{
    if(banquise_case.object->object_type == flake)
        return 1;

    else
        return 0;
}

int IsFlagB(T_case banquise_case)
{
    if(banquise_case.flag == B)
        return 1;

    else
        return 0;
}

int IsTrap(T_case banquise_case)
{
    if(banquise_case.object->object_type == trap)
        return 1;

    else
        return 0;
}

int IsPlayer(T_case banquise_case)
{
    if(banquise_case.player != NULL)
        return 1;

    else
        return 0;
}

int IsPlayerInRange(T_hammer_head *hammer_head, T_banquise *banquise)
{
    int hammer_head_line = hammer_head->pos.line;
    int hammer_head_col = hammer_head->pos.col;
    int hammer_head_state = hammer_head->state;
    T_vector clockw[4] = {{-1, 1},{1, 1},{1, -1},{-1, -1}};
    T_vector anticlockw[4] = {{1, 1},{1, -1},{-1, -1},{-1, 1}};
    bool found_player = false;

    if(hammer_head->rot_dir == clockwise)
    {
        for(int i = 0; i < 2; i++)
        {
            hammer_head_line += clockw[hammer_head_state].d_line;
            hammer_head_col += clockw[hammer_head_state].d_col;

            if(IsPlayer(banquise->grid[hammer_head_line][hammer_head_col]))
                found_player = true;
        }

        return found_player;
    }

    else
    {
        for(int i = 0; i < 2; i++)
        {
            hammer_head_line += clockw[hammer_head_state].d_line;
            hammer_head_col += clockw[hammer_head_state].d_col;
            hammer_head_state--;

            if(IsPlayer(banquise->grid[hammer_head_line][hammer_head_col]))
                found_player = true;
        }

        return found_player;
    }
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
    if(banquise_case.object != NULL && banquise_case.object->object_type != reserved)
        return 1;

    else
        return 0;
}

int IsHammerHead(T_case banquise_case)
{
    if(banquise_case.object != NULL && banquise_case.object->object_type == hammer_head)
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
        if (IsWater(banquise->grid[neighbour_line][neighbour_col]) || IsSpring(banquise->grid[neighbour_line][neighbour_col]) || IsHammerHead(banquise->grid[neighbour_line][neighbour_col]) || IsPlayer(banquise->grid[neighbour_line][neighbour_col]))
            return 1;
    }

    else
        return 0;
}

/* =================================== */
/* ============== MATHS ============== */
/* =================================== */

int scalar_product(T_vector A, T_vector B)
{
    return A.d_line * B.d_line + A.d_col * B.d_col;
}
