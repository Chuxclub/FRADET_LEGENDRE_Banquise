#include "glacon.h"


/* ============================================ */
/* ========== INITIALISATION GLACONS ========== */
/* ============================================ */


/*
    Auteur(e)(s): Florian Legendre
    Utilité: Meme chose que pour initTrap() dans piege.c ou initSpring() dans ressort.c, etc. En tenant compte des spécificités de l'objet glacon.
    Fonctionnement: Meme chose que pour initTrap() dans piege.c ou initSpring() dans ressort.c, etc. En tenant compte des spécificités de l'objet glacon.
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: /
*/
T_flake *initFlake()
{
    T_flake *res = (T_flake *) malloc(sizeof(T_flake));

    res->pos.line = 0;
    res->pos.col = 0;
    res->vect.d_line = 0;
    res->vect.d_col = 0;
    res->id = 0;

    return res;
}


/*
    Auteur(e)(s): Florian Legendre
    Utilité: Meme chose que pour initTraps() dans piege.c ou initSprings() dans ressort.c, etc.
    Fonctionnement: Meme chose que pour initTraps() dans piege.c ou initSprings() dans ressort.c, etc.
    Complexité en temps (au pire): O(nb_flakes)
    Hypothèse d'amélioration possible: /
*/
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

    return flakes;
}


/*
    Auteur(e)(s): Florian Legendre
    Utilité: Meme chose que pour addTraps() dans piege.c ou addSprings() dans ressort.c, etc.
    Fonctionnement: Meme chose que pour addTraps() dans piege.c ou addSprings() dans ressort.c, etc.
    Complexité en temps (au pire): O(banquise_size²)
    Hypothèse d'amélioration possible: Meme chose que pour addTraps() dans piege.c ou addSprings() dans ressort.c, etc.
*/
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


/*
    Auteur(e)(s): Florian Legendre
    Utilité: Une acceleration correspond, en physique, a une modification du vecteur vitesse. C'est ce qu'on fait ici avec la
             famille de fonction accelerate() et ses derivees Up, Down, Left... selon l'orientation de la variation.
    Fonctionnement: Va chercher dans la structure le paramètre qui correspond au vecteur vitesse souhaité et lui donne une valeur
                    constante correspondant à l'orientation voulue. Comme on veut que le glacon se déplace d'une case tous les tours,
                    cette valeur est toujours (+/-)1.
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: /
*/
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

/*
    Auteur(e)(s): Florian Legendre
    Utilité: Realiser l'interaction glacon/eau et donc la transformation de l'eau en glace et la disparition du glacon
    Fonctionnement: Copie localement les anciennes positions du glacon. "Detruit" le glacon en supprimant son pointeur et
                    en fixant son type a no_object. La destruction n'est cependant pas effective en memoire (cf. Hypothese
                    d'amelioration ci-dessous)
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: Pour une gestion plus propre de la memoire il faudrait pouvoir free() le glacon
                                       mais pour l'instant cela entraine des erreurs qui empechent la compilation
*/
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


/*
    Auteur(e)(s): Florian Legendre
    Utilité: Realiser l'interaction glacon/ressort
    Fonctionnement: Comme pour BecomeIce() on commence par copier les anciennes positions du glacons dans des variables locales.
                    On deconnecte alors le glacon de la banquise. Ensuite on calcule immediatement de nouvelles coordonnees candidates
                    pour le glacon (on veut qu'il n'y ait pas de delai d'un tour entre la rencontre avec le ressort et le debut de son chemin
                    dans le sens inverse, si on ne fait pas ca alors le vecteur vitesse est certes modifie mais il faut alors attendre un second tour
                    pour voir le glacon emprunter la direction en sens inverse. On aurait donc: 1 tour ou le glacon arrive pres du ressort, 1 tour ou
                    son vecteur vitesse est altere et enfin au tour suivant le glacon commencerait a partir dans l'autre sens grace a UpdateFlakes().
                    C'est ce comportement qu'on souhaite eviter).

                    Avec ces nouvelles coordonnees candidates on verifie qu'il n'y a pas de joueur qui s'est place entre le ressort et le glacon entre temps.
                    Si c'est le cas -> on tue le joueur et le glacon s'immobilise. Sinon on accelere le glacon dans le sens oppose a son sens initial et on le rebranche
                    a la banquise a la nouvelle position.
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: /
*/
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


/*
    Auteur(e)(s): Florian Legendre
    Utilité: Aiguiller selon l'objet interagissable rencontre par le glacon la procedure d'interaction a lancer
    Fonctionnement: Aiguillage par des if/then/else et eventuellement des switch() imbriques
    Complexité en temps (au pire): O(1) car toutes les procedures aiguillees ont au pire une complexite en O(1)
    Hypothèse d'amélioration possible: Creer une procedure pour ameliorer la lisibilite au niveau du cas hammer_head dans le switch()
*/
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


/*
    Auteur(e)(s): Florian Legendre
    Utilité: Realiser pour chaque glacon du jeu une mise a jour de leurs positions dans leurs donnees internes et sur la banquise.
             Cette fonction est utilisee dans la boucle de rafraichissement du jeu dans main.c, a chaque nouveau tour du jeu elle est donc appelee.
    Fonctionnement: Parcourt tous les glacons dans leur tableau correspondant. Si le glacon a ete "detruit" (change en no_object, voir BecomeIce() dans ce
                    meme module) sa mise a jour est ignoree. Dans le cas contraire on calcule le nouveau point candidat et on verifie que ce point candidat
                    remplit certaines conditions. Si c'est le cas, soit un fait la mise a jour du glacon, soit on lance l'aiguillage des interactions.
    Complexité en temps (au pire): O(1) car toutes les procedures que cette fonction comprend ont jusqu'ici une complexite au pire en O(1)
    Hypothèse d'amélioration possible: /
*/
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
