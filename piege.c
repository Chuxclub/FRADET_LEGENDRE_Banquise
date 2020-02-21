#include "user_types.h"
#include "constants.h"



T_object **initTraps(int nb_traps)
{
    T_object **traps = (T_object **) malloc(sizeof(T_object *) * nb_traps);

    for(int i = 0; i < nb_traps; i++)
        traps[i] = (T_object *) malloc(sizeof(T_object));


    for(int i = 0; i < nb_traps; i++)
    {
        traps[i]->object_type = trap;

        traps[i]->flake.pos.line = 0;
        traps[i]->flake.pos.col = 0;
        traps[i]->flake.vect.d_line = 0;
        traps[i]->flake.vect.d_col = 0;

        traps[i]->spring.pos.line = 0;
        traps[i]->spring.pos.col = 0;

        traps[i]->trap.pos.line = 0;
        traps[i]->trap.pos.col = 0;
    }

    return traps;
}

void addTraps(T_banquise *banquise, T_object **traps, int nb_traps)
{
    int counter = nb_traps - 1;

    while(counter >= 0)
    {
        for(int i = 0; i < banquise->size; i++)
        {
            for(int j = 0; j < banquise->size; j++)
            {
                int loto_trap = rand() % PERCENT;

                if(counter < 0)
                    return;

                else if(loto_trap < 5 && IsCaseAvailable(banquise->grid[i][j]))
                {
                    traps[counter]->flake.pos.line = i;
                    traps[counter]->flake.pos.col = j;
                    banquise->grid[i][j].object = traps[counter];
                    counter--;
                }
            }
        }
    }
}
