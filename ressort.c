#include "user_types.h"
#include "constants.h"

T_object **initSprings(int nb_springs)
{
    T_object **springs = (T_object **) malloc(sizeof(T_object *) * nb_springs);

    for(int i = 0; i < nb_springs; i++)
        springs[i] = (T_object *) malloc(sizeof(T_object));


    for(int i = 0; i < nb_springs; i++)
    {
        springs[i]->object_type = spring;

        springs[i]->flake.pos.line = 0;
        springs[i]->flake.pos.col = 0;
        springs[i]->flake.vect.d_line = 0;
        springs[i]->flake.vect.d_col = 0;

        springs[i]->spring.pos.line = 0;
        springs[i]->spring.pos.col = 0;

        springs[i]->trap.pos.line = 0;
        springs[i]->trap.pos.col = 0;
    }

    return springs;
}

void addSprings(T_banquise *banquise, T_object **springs, int nb_springs)
{
    int counter = nb_springs - 1;

    while(counter >= 0)
    {
        for(int i = 0; i < banquise->size; i++)
        {
            for(int j = 0; j < banquise->size; j++)
            {
                int loto_spring = rand() % PERCENT;

                if(counter < 0)
                    return;

                else if(loto_spring < 5 && IsCaseAvailable(banquise->grid[i][j]))
                {
                    springs[counter]->flake.pos.line = i;
                    springs[counter]->flake.pos.col = j;
                    banquise->grid[i][j].object = springs[counter];
                    counter--;
                }
            }
        }
    }
}

