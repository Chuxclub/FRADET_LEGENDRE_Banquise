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

// ------------> Déplacements
void accelerateUp(T_object *bumped_flake)
{
    /*Assignation d'une vitesse au glaçon bougé (ou 'bumpé'...)*/
    bumped_flake->flake->vect.d_line = -1;
}

void accelerateLeft(T_object *bumped_flake)
{
    /*Assignation d'une vitesse au glaçon bougé (ou 'bumpé'...)*/
    bumped_flake->flake->vect.d_col = -1;
}

void accelerateDown(T_object *bumped_flake)
{
    /*Assignation d'une vitesse au glaçon bougé (ou 'bumpé'...)*/
    bumped_flake->flake->vect.d_line = +1;
}

void accelerateRight(T_object *bumped_flake)
{
    /*Assignation d'une vitesse au glaçon bougé (ou 'bumpé'...)*/
    bumped_flake->flake->vect.d_col = +1;
}


// ------------> Réactions à l'environnement
void BecomeIce(T_object *bumped_flake, int water_line, int water_col, T_banquise *banquise)
{
    int flake_line = bumped_flake->flake->pos.line;
    int flake_col = bumped_flake->flake->pos.col;

    /*"Suppression" du glaçon. Pour pouvoir vraiment le supprimer il faudrait une pile
    et le supprimer dans la fonction UpdateFlakes()...*/
    bumped_flake->object_type = no_object;
    bumped_flake->flake = NULL;
    banquise->grid[flake_line][flake_col].object = NULL;

    banquise->grid[water_line][water_col].ground = ice;
}

void FlakeInteraction(T_object *bumped_flake, int neighbour_line, int neighbour_col, T_banquise *banquise)
{
    if(IsWater(banquise->grid[neighbour_line][neighbour_col]))
    {
        BecomeIce(bumped_flake, neighbour_line, neighbour_col, banquise);
    }
}


// ------------> Regroupement déplacements et réactions à l'environnement
void updateFlakes(int nb_flakes, T_object **flakes,  T_banquise *banquise)
{
    for(int i = 0; i < nb_flakes; i++)
    {
        if(flakes[i]->object_type != no_object)
        {
            /* Suppression sur la banquise des flocons de leurs anciennes positions */
            banquise->grid[flakes[i]->flake->pos.line][flakes[i]->flake->pos.col].object = NULL;

            /* Calcul des nouvelles positions des flocons par rapport à leurs vecteurs vitesse */
            int new_line = flakes[i]->flake->pos.line + flakes[i]->flake->vect.d_line;
            int new_col = flakes[i]->flake->pos.col + flakes[i]->flake->vect.d_col;

            /* Vérification de la validité des nouvelles positions calculées*/
            if(IsInbound(BANQUISE_SIZE, new_line, new_col))
            {
                //Si la case est disponible: déplacement du glaçon sur cette case
                if(IsCaseAvailable(banquise->grid[new_line][new_col]))
                {
                    flakes[i]->flake->pos.line = new_line;
                    flakes[i]->flake->pos.col = new_col;
                    banquise->grid[flakes[i]->flake->pos.line][flakes[i]->flake->pos.col].object = flakes[i];
                }

                //Si la case n'est pas disponible car il y a un objet interagissable: lancement de l'interaction
                else if(IsFlakeIN(BANQUISE_SIZE, banquise, new_line, new_col))
                {
                    flakes[i]->flake->vect.d_line = 0;
                    flakes[i]->flake->vect.d_col = 0;

                    FlakeInteraction(flakes[i], new_line, new_col, banquise);
                }

                //Sinon, arrêt du glaçon. Ce-dernier préserve ainsi ses anciennes positions
                else
                {
                    flakes[i]->flake->vect.d_line = 0;
                    flakes[i]->flake->vect.d_col = 0;
                    banquise->grid[flakes[i]->flake->pos.line][flakes[i]->flake->pos.col].object = flakes[i];
                }
             }

             /* Si la nouvelle position calculée est en-dehors du plateau on conserve l'ancienne position */
             else
                banquise->grid[flakes[i]->flake->pos.line][flakes[i]->flake->pos.col].object = flakes[i];
        }
    }
}
