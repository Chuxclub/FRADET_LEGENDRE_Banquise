#include "banquise.h"


#define RAND_MAX 101

//ajoute un ressort aléatoirement sur la banquise
//object = 3 pour un glacon
void addSprings(T_banquise *banquise)
{
    int loto_spring = rand() % RAND_MAX;
    //printf("loto_spring: %i\n", loto_spring);

    do
    {
        for(int i = 0; i < banquise->size; i++)
        {
            for(int j = 0; j < banquise->size; j++)
            {
                if(loto_spring < 5 && IsCaseAvailable(banquise->grid[i][j]))
                {
                    banquise->grid[i][j].object = spring;
                    return;
                }


                else
                    loto_spring --;
            }
        }
    }while(loto_spring > 5);
    //Si Spring n'est pas en-dessous de 5, on recommence le parcours de la banquise => Je m'assure
    //qu'un ressort sera toujours sur la banquise

}

