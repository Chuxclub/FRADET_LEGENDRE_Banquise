#include "banquise.h"

#define RAND_MAX 101

//ajoute un piege aléatoirement sur la banquise
void addTraps(T_banquise *banquise)
{
    int loto_trap = rand() % RAND_MAX;
    //printf("loto_trap: %i\n", loto_trap);

    do
    {
        for(int i = 0; i < banquise->size; i++)
        {
            for(int j = 0; j < banquise->size; j++)
            {
                if(loto_trap < 1 && IsCaseAvailable(banquise->grid[i][j]))
                {
                    banquise->grid[i][j].object = trap;
                    return;
                }


                else
                    loto_trap --;
            }
        }
    }while(loto_trap > 5);
    //Si la variable trap n'est pas en-dessous de 5, on recommence le parcours de la banquise => Je m'assure
    //qu'un ressort sera toujours sur la banquise

    /*srand(time(NULL));

    for(int i = 0; i < banquise->size; i++)
    {
        for(int j = 0; j < banquise->size; j++)
        {
            int trap = rand() % RAND_MAX;

            if(trap < 21 && banquise->grid[i][j].ice == 1)
                banquise->grid[i][j].object = 5;
        }
    }*/
}
