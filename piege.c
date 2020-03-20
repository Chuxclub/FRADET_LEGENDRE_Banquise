#include <stdlib.h>
#include "utils.h"
#include "user_types.h"
#include "constants.h"



/*
    Auteur(e)(s):
    Utilité:
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
T_trap *initTrap()
{
    T_trap *res = (T_trap *) malloc(sizeof(T_trap));

    res->pos.line = 0;
    res->pos.col = 0;

    return res;
}



/*
    Auteur(e)(s):
    Utilité:
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
T_object **initTraps(int nb_traps)
{
    T_object **traps = (T_object **) malloc(sizeof(T_object *) * nb_traps);

    for(int i = 0; i < nb_traps; i++)
        traps[i] = (T_object *) malloc(sizeof(T_object));


    for(int i = 0; i < nb_traps; i++)
    {
        traps[i]->object_type = trap;
        traps[i]->flake = NULL;
        traps[i]->spring = NULL;
        traps[i]->trap = initTrap();
    }

    return traps;
}



/*
    Auteur(e)(s):
    Utilité:
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
void addTraps(T_banquise *banquise, T_object **traps, int nb_traps)
{
    int counter = nb_traps - 1;

    while(counter >= 0)
    {
        for(int i = 0; i < banquise->sizeB; i++)
        {
            for(int j = 0; j < banquise->sizeB; j++)
            {
                int loto_trap = rand() % PERCENT;

                if(counter < 0)
                    return;

                else if(loto_trap < 5 && IsPlacementAvailable(banquise->grid[i][j]))
                {
                    traps[counter]->trap->pos.line = i;
                    traps[counter]->trap->pos.col = j;
                    banquise->grid[i][j].object = traps[counter];
                    counter--;
                }
            }
        }
    }
}
