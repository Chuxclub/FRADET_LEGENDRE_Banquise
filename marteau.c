#include "marteau.h"


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

    res_head->right_face.d_line = -1;
    res_head->right_face.d_col = 0;

    //Initilisation des autres paramètres de la tête du marteau
    res_head->pos.line = 0;
    res_head->pos.col = 0;

    res_head->momentum = no_momentum;
    res_head->rot_dir = no_rotation;

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


/* ======================================= */
/* ========== AFFICHAGE MARTEAU ========== */
/* ======================================= */

void printHammer(T_case banquise_case)
{
    //if(banquise_case.object->hammer.)
}

/* ============================================ *//* ============================================ */
