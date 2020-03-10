#include "marteau.h"


/* ============================================ */
/* ========== INITIALISATION MARTEAU ========== */
/* ============================================ */


T_hammer *initHammer()
{
    T_hammer *res = (T_hammer *) malloc(sizeof(T_hammer));

    res->handle.pos.line = 0;
    res->handle.pos.col = 0;
    res->handle.aspect = horizontal;

    //Le marteau est initialisé dans cette position: 0-
    res->head.up_face.d_line = 0;
    res->head.up_face.d_col = -1;

    res->head.left_face.d_line = +1;
    res->head.left_face.d_col = 0;

    res->head.right_face.d_line = -1;
    res->head.right_face.d_col = 0;

    //Initilisation des autres paramètres de la tête du marteau
    res->head.pos.line = 0;
    res->head.pos.col = 0;

    res->head.momentum = no_momentum;
    res->head.rot_dir = no_rotation;

    return res;
}

T_object **initHammers(int nb_hammers)
{
    T_object **hammers = (T_object **) malloc(sizeof(T_object *) * nb_hammers);

    for(int i = 0; i < nb_hammers; i++)
        hammers[i] = (T_object *) malloc(sizeof(T_object));

    for(int i = 0; i < nb_hammers; i++)
    {
        hammers[i]->object_type = hammer;

        hammers[i]->hammer = initHammer();
        hammers[i]->flake = NULL;
        hammers[i]->spring = NULL;
        hammers[i]->trap = NULL;
    }

    return hammers;
}

void addHammers(T_banquise *banquise, T_object **hammers, int nb_hammers)
{
    int counter = nb_hammers - 1;

    while(counter >= 0)
    {
        for(int i = 0; i < banquise->size; i++)
        {
            for(int j = 0; j < banquise->size; j++)
            {
                int loto_hammer = rand() % PERCENT;

                if(counter < 0)
                    return;

                else if(loto_hammer < 5 && IsCaseAvailable(banquise->grid[i][j]))
                {
                    hammers[counter]->hammer->handle.pos.line = i;
                    hammers[counter]->hammer->handle.pos.col = j;
                    hammers[counter]->hammer->head.pos.line = i + hammers[counter]->hammer->head.up_face.d_line;
                    hammers[counter]->hammer->head.pos.col = j + hammers[counter]->hammer->head.up_face.d_col;


                    int hammer_head_line = hammers[counter]->hammer->head.pos.line;
                    int hammer_head_col = hammers[counter]->hammer->head.pos.col;
                    banquise->grid[i][j].object = hammers[counter];
                    banquise->grid[hammer_head_line][hammer_head_col].object = hammers[counter];
                    counter--;
                }
            }
        }
    }
}
