#include "glacon.h"


/* ============================================ */
/* ========== INITIALISATION GLACONS ========== */
/* ============================================ */


T_flake *initFlake()
{
    T_flake *res = (T_flake *) malloc(sizeof(T_flake));

    res->pos.line = 0;
    res->pos.col = 0;
    res->vect.d_line = 0;
    res->vect.d_col = 0;

    return res;
}

T_object **initFlakes(int nb_flakes)
{
    T_object **flakes = (T_object **) malloc(sizeof(T_object *) * nb_flakes);

    for(int i = 0; i < nb_flakes; i++)
        flakes[i] = (T_object *) malloc(sizeof(T_object));

    for(int i = 0; i < nb_flakes; i++)
    {
        flakes[i]->object_type = flake;

        flakes[i]->flake = initFlake();
        flakes[i]->spring = NULL;
        flakes[i]->trap = NULL;
    }


    /*for(int i = 0; i < NB_FLAKES; i++)
    {
        printf("object_type de %i est %i\n", i, flakes[i]->object_type);
        printf("Pos.line de %i est de %i\n", i, flakes[i]->flake->pos.line);
        printf("Pos.col de %i est de %i\n\n", i, flakes[i]->flake->pos.col);
    }

    printf("***************\n\n");*/


    return flakes;
}

void addFlakes(T_banquise *banquise, T_object **flakes, int nb_flakes)
{
    int counter = nb_flakes - 1;

    while(counter >= 0)
    {
        for(int i = 0; i < banquise->size; i++)
        {
            for(int j = 0; j < banquise->size; j++)
            {
                int loto_flake = rand() % PERCENT;

                if(counter < 0)
                    return;

                else if(loto_flake < 5 && IsCaseAvailable(banquise->grid[i][j]))
                {
                    flakes[counter]->flake->pos.line = i;
                    flakes[counter]->flake->pos.col = j;
                    banquise->grid[i][j].object = flakes[counter];
                    counter--;
                }
            }
        }
    }
}



/* ============================================ */
/* =========== INTERACTIONS GLACONS =========== */
/* ============================================ */

void moveFlakeUp(T_object *bumped_flake, T_banquise *banquise)
{
    /*R�cup�ration des coordonn�es du gla�on*/
    int previous_line = bumped_flake->flake->pos.line;
    int previous_col = bumped_flake->flake->pos.col;
    int new_line = previous_line - 1;

    if(IsInbound(BANQUISE_SIZE, new_line, previous_col))
    {
        if(IsCaseAvailable(banquise->grid[new_line][previous_col]))
        {
            /*Assignation d'une vitesse au gla�on boug� (ou 'bump�'...)*/
            bumped_flake->flake->vect.d_line = -1;

            /*D�placement du gla�on tant qu'il peut se d�placer*/
            while(IsInbound(BANQUISE_SIZE, new_line - 1, previous_col))
            {
                if(IsCaseAvailable(banquise->grid[new_line - 1][previous_col]))
                    new_line += bumped_flake->flake->vect.d_line;
                else
                    break;
            }

            /*Arr�t du gla�on*/
            bumped_flake->flake->vect.d_col = 0;

            /*Modification des coordonn�es internes du gla�on*/
            bumped_flake->flake->pos.line = new_line;
            bumped_flake->flake->pos.col = previous_col;

            /*Ajout du gla�on sur la banquise, sur sa nouvelle position calcul�e*/
            banquise->grid[new_line][previous_col].object = bumped_flake;

            /*Suppression du gla�on sur la banquise, � sa position pr�c�dente*/
            banquise->grid[previous_line][previous_col].object = NULL;
        }
    }
}

void moveFlakeLeft(T_object *bumped_flake, T_banquise *banquise)
{
    /*R�cup�ration des coordonn�es du gla�on*/
    int previous_line = bumped_flake->flake->pos.line;
    int previous_col = bumped_flake->flake->pos.col;
    int new_col = previous_col - 1;

    if(IsInbound(BANQUISE_SIZE, previous_line, new_col))
    {
        if(IsCaseAvailable(banquise->grid[previous_line][new_col]))
        {
            /*Assignation d'une vitesse au gla�on boug� (ou 'bump�'...)*/
            bumped_flake->flake->vect.d_col = -1;

            /*D�placement du gla�on tant qu'il peut se d�placer*/
            while(IsInbound(BANQUISE_SIZE, previous_line, new_col - 1))
            {
                if(IsCaseAvailable(banquise->grid[previous_line][new_col - 1]))
                    new_col += bumped_flake->flake->vect.d_col;
                else
                    break;
            }

            /*Arr�t du gla�on et modification de ses coordonn�es internes*/
            bumped_flake->flake->vect.d_col = 0;
            bumped_flake->flake->pos.line = previous_line;
            bumped_flake->flake->pos.col = new_col;

            /*Ajout du gla�on sur la banquise, sur sa nouvelle position calcul�e*/
            banquise->grid[previous_line][new_col].object = bumped_flake;

            /*Suppression du gla�on sur la banquise, � sa position pr�c�dente*/
            banquise->grid[previous_line][previous_col].object = NULL;
        }
    }
}

void moveFlakeDown(T_object *bumped_flake, T_banquise *banquise)
{
    /*R�cup�ration des coordonn�es du gla�on*/
    int previous_line = bumped_flake->flake->pos.line;
    int previous_col = bumped_flake->flake->pos.col;
    int new_line = previous_line + 1;

    if(IsInbound(BANQUISE_SIZE, new_line, previous_col))
    {
        if(IsCaseAvailable(banquise->grid[new_line][previous_col]))
        {
            /*Assignation d'une vitesse au gla�on boug� (ou 'bump�'...)*/
            bumped_flake->flake->vect.d_line = +1;

            /*D�placement du gla�on tant qu'il peut se d�placer*/
            while(IsInbound(BANQUISE_SIZE, new_line + 1, previous_col))
            {
                if(IsCaseAvailable(banquise->grid[new_line + 1][previous_col]))
                    new_line += bumped_flake->flake->vect.d_line;
                else
                    break;
            }

            /*Arr�t du gla�on et modification de ses coordonn�es internes*/
            bumped_flake->flake->vect.d_col = 0;
            bumped_flake->flake->pos.line = new_line;
            bumped_flake->flake->pos.col = previous_col;

            /*Ajout du gla�on sur la banquise, sur sa nouvelle position calcul�e*/
            banquise->grid[new_line][previous_col].object = bumped_flake;

            /*Suppression du gla�on sur la banquise, � sa position pr�c�dente*/
            banquise->grid[previous_line][previous_col].object = NULL;
        }
    }
}

void moveFlakeRight(T_object *bumped_flake, T_banquise *banquise)
{
    /*R�cup�ration des coordonn�es du gla�on*/
    int previous_line = bumped_flake->flake->pos.line;
    int previous_col = bumped_flake->flake->pos.col;
    int new_col = previous_col + 1;

    if(IsInbound(BANQUISE_SIZE, previous_line, new_col))
    {
        if(IsCaseAvailable(banquise->grid[previous_line][new_col]))
        {
            /*Assignation d'une vitesse au gla�on boug� (ou 'bump�'...)*/
            bumped_flake->flake->vect.d_col = +1;

            /*D�placement du gla�on tant qu'il peut se d�placer*/
            while(IsInbound(BANQUISE_SIZE, previous_line, new_col + 1))
            {
                if(IsCaseAvailable(banquise->grid[previous_line][new_col + 1]))
                    new_col += bumped_flake->flake->vect.d_col;
                else
                    break;
            }

            /*Arr�t du gla�on et modification de ses coordonn�es internes*/
            bumped_flake->flake->vect.d_col = 0;
            bumped_flake->flake->pos.line = previous_line;
            bumped_flake->flake->pos.col = new_col;

            /*Ajout du gla�on sur la banquise, sur sa nouvelle position calcul�e*/
            banquise->grid[previous_line][new_col].object = bumped_flake;

            /*Suppression du gla�on sur la banquise, � sa position pr�c�dente*/
            banquise->grid[previous_line][previous_col].object = NULL;
        }
    }
}
