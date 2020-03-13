#include "marteau.h"
#include <math.h>


/* ============================================ */
/* ========== INITIALISATION MARTEAU ========== */
/* ============================================ */


T_hammer *initHammer()
{
    T_hammer_handle *res_handle = (T_hammer_handle *) malloc(sizeof(T_hammer_handle));
    T_hammer_head *res_head = (T_hammer_head *) malloc(sizeof(T_hammer_head));
    T_hammer *res_hammer = (T_hammer *) malloc(sizeof(T_hammer));

    res_handle->id = 0;
    res_handle->pos.line = 0;
    res_handle->pos.col = 0;
    res_handle->aspect = horizontal;

    //Le marteau est initialisé dans cette position: o-
    res_head->id = 0;
    res_head->up_face.d_line = 0;
    res_head->up_face.d_col = -1;

    res_head->left_face.d_line = +1;
    res_head->left_face.d_col = 0;


    //Initilisation des autres paramètres de la tête du marteau
    res_head->pos.line = 0;
    res_head->pos.col = 0;

    res_head->momentum = no_momentum;
    res_head->rot_dir = no_rotation;
    res_head->state = left;

    res_hammer->handle = res_handle;
    res_hammer->head = res_head;

    return res_hammer;
}

T_object **initHammers(int nb_hammers)
{
    T_object **hammers = (T_object **) malloc(sizeof(T_object *) * 2* nb_hammers);

    for(int i = 0; i < 2 * nb_hammers; i++)
        hammers[i] = (T_object *) malloc(sizeof(T_object));

    for(int i = 0; i < 2 * nb_hammers; i+=2)
    {
        T_hammer *myHammer = initHammer();

        hammers[i]->object_type = hammer_head;
        hammers[i + 1]->object_type = hammer_handle;

        hammers[i]->hammer_head = myHammer->head;
        hammers[i]->hammer_handle = NULL;

        hammers[i + 1]->hammer_head = NULL;
        hammers[i + 1]->hammer_handle = myHammer->handle;


        hammers[i]->flake = NULL;
        hammers[i + 1]->flake = NULL;

        hammers[i]->spring = NULL;
        hammers[i + 1]->spring = NULL;

        hammers[i]->trap = NULL;
        hammers[i + 1]->trap = NULL;
    }

    return hammers;
}

void addHammers(T_banquise *banquise, T_object **hammers, int nb_hammers)
{
    int counter = 2*nb_hammers - 1;

    while(counter >= 0)
    {
        for(int i = 0; i < banquise->sizeB; i++)
        {
            for(int j = 0; j < banquise->sizeB; j++)
            {
                int loto_hammer = rand() % PERCENT;

                if(counter < 0)
                    return;

                else if(loto_hammer < 5 && AreSurroundingsAvailable(banquise, i, j))
                {
                    int hammer_head_line = i + hammers[counter - 1]->hammer_head->up_face.d_line;
                    int hammer_head_col = j + hammers[counter - 1]->hammer_head->up_face.d_col;

                    //On calcule d'abord la position de la tête du marteau puis celle de sa poignée
                    hammers[counter - 1]->hammer_head->pos.line = hammer_head_line;
                    hammers[counter - 1]->hammer_head->pos.col = hammer_head_col;

                    hammers[counter]->hammer_handle->pos.line = i;
                    hammers[counter]->hammer_handle->pos.col = j;

                    //Branchement sur la banquise
                    PlaceReservedZone(banquise, i, j);
                    banquise->grid[i][j].object = hammers[counter];
                    banquise->grid[hammer_head_line][hammer_head_col].object = hammers[counter - 1];
                    counter -= 2;
                }
            }
        }
    }
}

void updateHammers(int nb_hammers, T_object **hammers,  T_banquise *banquise)
{
    /* Rappel: ordre = tête puis manche => hammers[0] est une tête de marteau, hammers[1] est un manche de marteau, etc. */
    for(int i = 0; i < nb_hammers*2; i+=2)
    {
        if(hammers[i]->hammer_head->momentum != no_momentum)
        {
            T_vector clockw[4] = {{-1, 1},{1, 1},{1, -1},{-1, -1}};
            T_vector anticlockw[4] = {{1, 1},{1, -1},{-1, -1},{-1, 1}};
            int head_state = hammers[i]->hammer_head->state;

            if(hammers[i]->hammer_head->rot_dir == clockwise)
            {
                //Calculating new head pos and updating banquise
                banquise->grid[hammers[i]->hammer_head->pos.line][hammers[i]->hammer_head->pos.col].object = NULL;
                hammers[i]->hammer_head->pos.line += clockw[head_state].d_line;
                hammers[i]->hammer_head->pos.col += clockw[head_state].d_col;
                banquise->grid[hammers[i]->hammer_head->pos.line][hammers[i]->hammer_head->pos.col].object = hammers[i];

                //Calculating and updating new upf and leftf vectors
                int previous_head_state = head_state - 1;

                if(previous_head_state < 0)
                    previous_head_state = 3;

                hammers[i]->hammer_head->up_face.d_line += clockw[head_state].d_line;
                hammers[i]->hammer_head->up_face.d_col += clockw[head_state].d_col;
                hammers[i]->hammer_head->left_face.d_line += clockw[previous_head_state].d_line;
                hammers[i]->hammer_head->left_face.d_col += clockw[previous_head_state].d_col;

                //Calculating and updating new vector carrier
                hammers[i]->hammer_head->vector_carrier.d_line = clockw[head_state].d_line - hammers[i]->hammer_head->vector_carrier.d_line;
                hammers[i]->hammer_head->vector_carrier.d_col = clockw[head_state].d_col - hammers[i]->hammer_head->vector_carrier.d_col;

                //Changing state and decrementing momentum
                hammers[i]->hammer_head->state++;
                hammers[i]->hammer_head->state %= 4;
                hammers[i]->hammer_head->momentum--;

                if(hammers[i]->hammer_head->momentum == no_momentum)
                {
                    int transfer_neighb_line = hammers[i]->hammer_head->pos.line + hammers[i]->hammer_head->vector_carrier.d_line;
                    int transfer_neighb_col = hammers[i]->hammer_head->pos.col + hammers[i]->hammer_head->vector_carrier.d_col;

                    if(banquise->grid[transfer_neighb_line][transfer_neighb_col].object->object_type == flake)
                    {
                        banquise->grid[transfer_neighb_line][transfer_neighb_col].object->flake->vect.d_line = hammers[i]->hammer_head->vector_carrier.d_line;
                        banquise->grid[transfer_neighb_line][transfer_neighb_col].object->flake->vect.d_col = hammers[i]->hammer_head->vector_carrier.d_col;
                        hammers[i]->hammer_head->vector_carrier.d_line = 0;
                        hammers[i]->hammer_head->vector_carrier.d_col = 0;
                    }

                    else
                    {
                        hammers[i]->hammer_head->vector_carrier.d_line = 0;
                        hammers[i]->hammer_head->vector_carrier.d_col = 0;
                    }
                }
            }

            else
            {
                //Calculating new head pos and updating banquise
                banquise->grid[hammers[i]->hammer_head->pos.line][hammers[i]->hammer_head->pos.col].object = NULL;
                hammers[i]->hammer_head->pos.line += anticlockw[head_state].d_line;
                hammers[i]->hammer_head->pos.col += anticlockw[head_state].d_col;
                banquise->grid[hammers[i]->hammer_head->pos.line][hammers[i]->hammer_head->pos.col].object = hammers[i];

                //Calculating and updating new upf and leftf vectors
                int previous_head_state = head_state - 1;

                if(previous_head_state < 0)
                    previous_head_state = 3;

                hammers[i]->hammer_head->up_face.d_line += clockw[head_state].d_line;
                hammers[i]->hammer_head->up_face.d_col += clockw[head_state].d_col;
                hammers[i]->hammer_head->left_face.d_line += clockw[previous_head_state].d_line;
                hammers[i]->hammer_head->left_face.d_col += clockw[previous_head_state].d_col;

                //Calculating and updating new vector carrier
                hammers[i]->hammer_head->vector_carrier.d_line = anticlockw[head_state].d_line - hammers[i]->hammer_head->vector_carrier.d_line;
                hammers[i]->hammer_head->vector_carrier.d_col = anticlockw[head_state].d_col - hammers[i]->hammer_head->vector_carrier.d_col;

                //Changing state and decrementing momentum
                hammers[i]->hammer_head->state--;
                hammers[i]->hammer_head->state %= 4;
                hammers[i]->hammer_head->momentum--;

                if(hammers[i]->hammer_head->momentum == no_momentum)
                {
                    int transfer_neighb_line = hammers[i]->hammer_head->pos.line + hammers[i]->hammer_head->vector_carrier.d_line;
                    int transfer_neighb_col = hammers[i]->hammer_head->pos.col + hammers[i]->hammer_head->vector_carrier.d_col;

                    if(banquise->grid[transfer_neighb_line][transfer_neighb_col].object->object_type == flake)
                    {
                        banquise->grid[transfer_neighb_line][transfer_neighb_col].object->flake->vect.d_line = hammers[i]->hammer_head->vector_carrier.d_line;
                        banquise->grid[transfer_neighb_line][transfer_neighb_col].object->flake->vect.d_col = hammers[i]->hammer_head->vector_carrier.d_col;
                        hammers[i]->hammer_head->vector_carrier.d_line = 0;
                        hammers[i]->hammer_head->vector_carrier.d_col = 0;
                    }

                    else
                    {
                        hammers[i]->hammer_head->vector_carrier.d_line = 0;
                        hammers[i]->hammer_head->vector_carrier.d_col = 0;
                    }
                }
            }
        }
    }
}

/* ======================================= */
/* ========== AFFICHAGE MARTEAU ========== */
/* ======================================= */
/* //Calcul des nouvelles positions des marteaux par rapport à leurs momentums et leurs directions de rotation
            int former_line = hammers[i]->hammer_head->pos.line;
            int former_col = hammers[i]->hammer_head->pos.col;
            int rot_dir = hammers[i]->hammer_head->rot_dir;
            double former_cos = acos((double) hammers[i]->hammer_head->up_face.d_col);
            double former_sin = asin((double) hammers[i]->hammer_head->up_face.d_line);

            printf("former line is : %i\n", former_line);
            printf("former col is : %i\n", former_col);
            printf("rot_dir is : %i\n", rot_dir);
            printf("former_cos is : %f\n", former_cos);
            printf("former_sin is : %f\n", former_sin);

            int new_line = (int) sin(former_sin + (double) 2.0 * rot_dir / M_PI);
            int new_col = (int) cos(former_cos + (double) 2.0 * rot_dir / M_PI);

            printf("new line is : %i\n", new_line);
            printf("new col is : %i\n", new_col);

            hammers[i]->hammer_head->pos.col = new_col;
            hammers[i]->hammer_head->pos.line = new_line;

            hammers[i]->hammer_head->momentum--;

            banquise->grid[new_line][new_col].object->object_type = hammer_head;
            banquise->grid[new_line][new_col].object = hammers[i];
            banquise->grid[former_line][former_col].object = NULL;*/
void printHammer(T_case banquise_case)
{
    //if(banquise_case.object->hammer.)
}

/* ============================================ *//* ============================================ */
