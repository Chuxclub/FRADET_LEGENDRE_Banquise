#include <stdlib.h>
#include "user_types.h"
#include "constants.h"
#include "utils.h"


/*
    Auteur(e)(s): Florian Legendre
    Utilité:
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
T_spring *initSpring()
{
    T_spring *res = (T_spring *) malloc(sizeof(T_spring));

    res->pos.line = 0;
    res->pos.col = 0;

    return res;
}


/*
    Auteur(e)(s): Florian Legendre
    Utilité:
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
T_object **initSprings(int nb_springs)
{
    T_object **springs = (T_object **) malloc(sizeof(T_object *) * nb_springs);

    for(int i = 0; i < nb_springs; i++)
        springs[i] = (T_object *) malloc(sizeof(T_object));


    for(int i = 0; i < nb_springs; i++)
    {
        springs[i]->object_type = spring;

        springs[i]->flake = NULL;
        springs[i]->spring = initSpring();
        springs[i]->trap = NULL;
    }

    return springs;
}



/*
    Auteur(e)(s): Florian Legendre
    Utilité:
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
void addSprings(T_banquise *banquise, T_object **springs, int nb_springs)
{
    int counter = nb_springs - 1;

    while(counter >= 0)
    {
        for(int i = 0; i < banquise->sizeB; i++)
        {
            for(int j = 0; j < banquise->sizeB; j++)
            {
                int loto_spring = rand() % PERCENT;

                if(counter < 0)
                    return;

                else if(loto_spring < 5 && IsPlacementAvailable(banquise->grid[i][j]))
                {
                    springs[counter]->spring->pos.line = i;
                    springs[counter]->spring->pos.col = j;
                    banquise->grid[i][j].object = springs[counter];
                    counter--;
                }
            }
        }
    }
}

