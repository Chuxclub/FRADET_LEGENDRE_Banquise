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
        for(int i = 0; i < banquise->sizeB; i++)
        {
            for(int j = 0; j < banquise->sizeB; j++)
            {
                int loto_flake = rand() % PERCENT;

                if(counter < 0)
                    return;

                else if(loto_flake < 5 && IsPlacementAvailable(banquise->grid[i][j]))
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

void accelerateOpposite(T_object *bumped_flake)
{
    /*Assignation d'une vitesse au glaçon bougé (ou 'bumpé'...)*/
    bumped_flake->flake->vect.d_line *= -1;
    bumped_flake->flake->vect.d_col *= -1;
}

void stopFlake(T_object *bumped_flake)
{
    bumped_flake->flake->vect = null_vect();
}


// ------------> Réactions aux objets et à l'environnement
void BecomeIce(T_object *bumped_flake, int water_line, int water_col, T_banquise *banquise)
{
    int flake_line = bumped_flake->flake->pos.line;
    int flake_col = bumped_flake->flake->pos.col;

    /*"Suppression" du glaçon. Pour pouvoir vraiment le supprimer il faudrait une pile
    et le supprimer dans la fonction UpdateFlakes() avec un free()...*/
    bumped_flake->object_type = no_object;
    bumped_flake->flake = NULL;
    banquise->grid[flake_line][flake_col].object = NULL;

    banquise->grid[water_line][water_col].ground = ice;
}

void BumpSpring(T_object *bumped_flake, int neighbour_line, int neighbour_col, T_banquise *banquise)
{
    int previous_flake_line = bumped_flake->flake->pos.line;
    int previous_flake_col = bumped_flake->flake->pos.col;
    banquise->grid[previous_flake_line][previous_flake_col].object = NULL;

    int potential_new_flake_line = bumped_flake->flake->pos.line + (bumped_flake->flake->vect.d_line * -1);
    int potential_new_flake_col = bumped_flake->flake->pos.col + (bumped_flake->flake->vect.d_col * -1);

    if(IsPlayer(banquise->grid[potential_new_flake_line][potential_new_flake_col]))
    {
        banquise->grid[potential_new_flake_line][potential_new_flake_col].player->details.health = dead;
        stopFlake(bumped_flake);
        banquise->grid[previous_flake_line][previous_flake_col].object = bumped_flake;
    }

    else
    {
        accelerateOpposite(bumped_flake);
        bumped_flake->flake->pos.line += bumped_flake->flake->vect.d_line;
        bumped_flake->flake->pos.col += bumped_flake->flake->vect.d_col;

        banquise->grid[bumped_flake->flake->pos.line][bumped_flake->flake->pos.col].object = bumped_flake;
    }
}

void FlakeInteraction(T_object *bumped_flake, int neighbour_line, int neighbour_col, T_banquise *banquise)
{
    if(IsWater(banquise->grid[neighbour_line][neighbour_col]))
        BecomeIce(bumped_flake, neighbour_line, neighbour_col, banquise);

    else if(IsObject(banquise->grid[neighbour_line][neighbour_col]))
    {
        switch(banquise->grid[neighbour_line][neighbour_col].object->object_type)
        {
            case spring:
                BumpSpring(bumped_flake, neighbour_line, neighbour_col, banquise);
                break;

            case hammer_head:
            {
                int scalar_left = scalar_product(bumped_flake->flake->vect, banquise->grid[neighbour_line][neighbour_col].object->hammer_head->left_face);


                if(scalar_left != 0)
                {
                    if(scalar_left > 0)
                    {
                       banquise->grid[neighbour_line][neighbour_col].object->hammer_head->rot_dir = anticlockwise;
                       banquise->grid[neighbour_line][neighbour_col].object->hammer_head->momentum = full_momentum;
                       banquise->grid[neighbour_line][neighbour_col].object->hammer_head->vector_carrier = bumped_flake->flake->vect;
                    }


                    else
                    {
                       banquise->grid[neighbour_line][neighbour_col].object->hammer_head->rot_dir = clockwise;
                       banquise->grid[neighbour_line][neighbour_col].object->hammer_head->momentum = full_momentum;
                       banquise->grid[neighbour_line][neighbour_col].object->hammer_head->vector_carrier = bumped_flake->flake->vect;
                    }
                }

                stopFlake(bumped_flake);

                break;
            }

            default:
                printf("\n\n Something unknown\n");
                break;
        }
    }


    else if(IsPlayer(banquise->grid[neighbour_line][neighbour_col]))
    {
        banquise->grid[neighbour_line][neighbour_col].player->details.health = dead;
        stopFlake(bumped_flake);
    }

}



// ------------> Regroupement déplacements et réactions aux objets et à l'environnement
void updateFlakes(int nb_flakes, T_object **flakes,  T_banquise *banquise)
{

    for(int i = 0; i < nb_flakes; i++)
    {
        if(flakes[i]->object_type != no_object)
        {
            /* Calcul des nouvelles positions des flocons par rapport à leurs vecteurs vitesse */
            T_pos new_pos = translate_point(flakes[i]->flake->pos, flakes[i]->flake->vect);


            /* Vérification de la validité des nouvelles positions calculées*/
            if(IsInbound(BANQUISE_SIZE, new_pos.line, new_pos.col))
            {
                /* Si la case est disponible: déplacement du glaçon sur cette case
                   On a donc: modification des coordonnées du glaçon et débranchement/branchement sur banquise */
                if(IsCaseAvailable(banquise->grid[new_pos.line][new_pos.col]))
                {
                    banquise->grid[flakes[i]->flake->pos.line][flakes[i]->flake->pos.col].object = NULL;
                    flakes[i]->flake->pos = new_pos;
                    banquise->grid[flakes[i]->flake->pos.line][flakes[i]->flake->pos.col].object = flakes[i];
                }

                /* Si la case n'est pas disponible car il y a un objet interagissable: lancement de l'interaction */
                else if(IsFlakeIN(BANQUISE_SIZE, banquise, new_pos.line, new_pos.col))
                    FlakeInteraction(flakes[i], new_pos.line, new_pos.col, banquise);

                /* Sinon, c'est un rocher ou un drapeau, on arrête le glaçon et on conserve l'ancienne position */
                else
                {
                    stopFlake(flakes[i]);
                    banquise->grid[flakes[i]->flake->pos.line][flakes[i]->flake->pos.col].object = flakes[i];
                }
             }

             /* Si la nouvelle position calculée est en-dehors du plateau on arrête le flocon et on conserve l'ancienne position */
             else
             {
                 stopFlake(flakes[i]);
                 banquise->grid[flakes[i]->flake->pos.line][flakes[i]->flake->pos.col].object = flakes[i];
             }

        }
    }
}
