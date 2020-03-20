#include <stdlib.h>

#include "utils.h"

/* ====================================== */
/* ============== BOOLEANS ============== */
/* ====================================== */

/*
    Auteur(e)(s):
    Utilité:
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
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


/*
    Auteur(e)(s):
    Utilité:
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
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


/*
    Auteur(e)(s):
    Utilité:
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
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


/*
    Auteur(e)(s):
    Utilité:
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
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



/*
    Auteur(e)(s):
    Utilité:
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
int IsPlayerInRange(T_hammer_head *hammer_head, T_banquise *banquise, int *player_id_in_range)
{
    T_pos H_pos = {hammer_head->pos.line, hammer_head->pos.col};
    T_vector H_vector_carrier = hammer_head->vector_carrier;
    int H_state = hammer_head->state;
    T_vector clockw[4] = {{-1, 1},{1, 1},{1, -1},{-1, -1}};
    T_vector anticlockw[4] = {{1, 1},{1, -1},{-1, -1},{-1, 1}};
    bool found_player = false;

    //Defining positions in range of hammer's head
    T_pos check_pos1 = translate_point(H_pos, H_vector_carrier);

    if(hammer_head->rot_dir == clockwise)
    {
        T_pos check_pos2 = translate_point(H_pos, clockw[H_state]);

        if(IsPlayer(banquise->grid[check_pos1.line][check_pos1.col]))
        {
            found_player = true;
            *player_id_in_range = banquise->grid[check_pos1.line][check_pos1.col].player->id;
        }

        else if(IsPlayer(banquise->grid[check_pos2.line][check_pos2.col]))
        {
            found_player = true;
            *player_id_in_range = banquise->grid[check_pos2.line][check_pos2.col].player->id;
        }
    }

    else
    {
        T_pos check_pos2 = translate_point(H_pos, anticlockw[H_state]);

        if(IsPlayer(banquise->grid[check_pos1.line][check_pos1.col]))
        {
            found_player = true;
            *player_id_in_range = banquise->grid[check_pos1.line][check_pos1.col].player->id;
        }

        else if(IsPlayer(banquise->grid[check_pos2.line][check_pos2.col]))
        {
            found_player = true;
            *player_id_in_range = banquise->grid[check_pos2.line][check_pos2.col].player->id;
        }
    }

    return found_player;
}


/*
    Auteur(e)(s):
    Utilité:
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
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



/* ====================================== */
/* =============== OTHERS =============== */
/* ====================================== */


/*
    Auteur(e)(s):
    Utilité:
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
int enum_cycle_right(int n, int max, int right)
{
    return ((n + right) % max);
}


/*
    Auteur(e)(s):
    Utilité:
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
int enum_cycle_left(int n, int max, int left)
{
    return ((n + (max - left)) % max);
}
