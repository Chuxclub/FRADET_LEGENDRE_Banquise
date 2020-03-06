#include "test_funcs.h"

/* ============================================ */
/* =========== INTERACTIONS GLACONS =========== */
/* ============================================ */

void moveFlakeUp(T_object *bumped_flake, T_banquise *banquise)
{
    /*Récupération des coordonnées de la case où se trouve initialement le glaçon*/
    int previous_line = bumped_flake->flake->pos.line;
    int previous_col = bumped_flake->flake->pos.col;
    int new_line = previous_line;

    /*Assignation d'une vitesse au glaçon bougé (ou 'bumpé'...)*/
    bumped_flake->flake->vect.d_line = -1;

    /*Déplacement du glaçon tant qu'il peut se déplacer*/
    while(IsInbound(BANQUISE_SIZE, new_line - 1, previous_col))
    {
        if(IsCaseAvailable(banquise->grid[new_line - 1][previous_col]))
            new_line += bumped_flake->flake->vect.d_line;
        else
            break;
    }


    /*Lorsque le glaçon s'arrête on vérifie si l'arrêt a été provoqué par un voisin avec lequel il peut interagir...*/
    if(IsFlakeIN(BANQUISE_SIZE, banquise, new_line - 1, previous_col))
    {
        //Le glaçon s'arrête avant l'objet avec lequel interagir
        bumped_flake->flake->vect.d_line = 0;
        bumped_flake->flake->pos.line = new_line;

        //L'interaction est réalisée
        FlakeInteraction(bumped_flake, new_line - 1, previous_col, banquise);

        //Le glaçon est détruit de la banquise puis de la mémoire
        banquise->grid[previous_line][previous_col].object = NULL;
        free(bumped_flake);
    }


    /* ... Ou pas */
    else
    {
        //Annulation de la vitesse du glaçon
        bumped_flake->flake->vect.d_line = 0;

        //Modification de la ligne où se trouve à présent le glaçon
        bumped_flake->flake->pos.line = new_line;

        //Suppression du glaçon sur la banquise, à sa position précédente
        banquise->grid[previous_line][previous_col].object = NULL;

        //Ajout du glaçon sur la banquise, sur sa nouvelle position calculée
        banquise->grid[new_line][previous_col].object = bumped_flake;
    }
}

void moveFlakeLeft(T_object *bumped_flake, T_banquise *banquise)
{
    /*Récupération des coordonnées de la case où se trouve initialement le glaçon*/
    int previous_line = bumped_flake->flake->pos.line;
    int previous_col = bumped_flake->flake->pos.col;
    int new_col = previous_col;

    /*Assignation d'une vitesse au glaçon bougé (ou 'bumpé'...)*/
    bumped_flake->flake->vect.d_col = -1;

    /*Déplacement du glaçon tant qu'il peut se déplacer*/
    while(IsInbound(BANQUISE_SIZE, previous_line, new_col - 1))
    {
        if(IsCaseAvailable(banquise->grid[previous_line][new_col - 1]))
            new_col += bumped_flake->flake->vect.d_col;
        else
            break;
    }


    /*Lorsque le glaçon s'arrête on vérifie si l'arrêt a été provoqué par un voisin avec lequel il peut interagir...*/
    if(IsFlakeIN(BANQUISE_SIZE, banquise, previous_line, new_col - 1))
    {
        //Le glaçon s'arrête avant l'objet avec lequel interagir
        bumped_flake->flake->vect.d_line = 0;
        bumped_flake->flake->pos.col = new_col;

        //L'interaction est réalisée
        FlakeInteraction(bumped_flake, previous_line, new_col - 1, banquise);

        //Le glaçon est détruit de la banquise puis de la mémoire
        banquise->grid[previous_line][previous_col].object = NULL;
        free(bumped_flake);
    }


    /* ... Ou pas */
    else
    {
        //Annulation de la vitesse du glaçon
        bumped_flake->flake->vect.d_col = 0;

        //Modification de la ligne où se trouve à présent le glaçon
        bumped_flake->flake->pos.col = new_col;

        //Suppression du glaçon sur la banquise, à sa position précédente
        banquise->grid[previous_line][previous_col].object = NULL;

        //Ajout du glaçon sur la banquise, sur sa nouvelle position calculée
        banquise->grid[previous_line][new_col].object = bumped_flake;
    }
}

void moveFlakeDown(T_object *bumped_flake, T_banquise *banquise)
{
    /*Récupération des coordonnées de la case où se trouve initialement le glaçon*/
    int previous_line = bumped_flake->flake->pos.line;
    int previous_col = bumped_flake->flake->pos.col;
    int new_line = previous_line;

    /*Assignation d'une vitesse au glaçon bougé (ou 'bumpé'...)*/
    bumped_flake->flake->vect.d_line = +1;

    /*Déplacement du glaçon tant qu'il peut se déplacer*/
    while(IsInbound(BANQUISE_SIZE, new_line + 1, previous_col))
    {
        if(IsCaseAvailable(banquise->grid[new_line + 1][previous_col]))
            new_line += bumped_flake->flake->vect.d_line;
        else
            break;
    }


    /*Lorsque le glaçon s'arrête on vérifie si l'arrêt a été provoqué par un voisin avec lequel il peut interagir...*/
    if(IsFlakeIN(BANQUISE_SIZE, banquise, new_line + 1, previous_col))
    {
        //Le glaçon s'arrête avant l'objet avec lequel interagir
        bumped_flake->flake->vect.d_line = 0;
        bumped_flake->flake->pos.line = new_line;

        //L'interaction est réalisée
        FlakeInteraction(bumped_flake, new_line + 1, previous_col, banquise);

        //Le glaçon est détruit de la banquise puis de la mémoire
        banquise->grid[previous_line][previous_col].object = NULL;
        free(bumped_flake);
    }


    /* ... Ou pas */
    else
    {
        //Annulation de la vitesse du glaçon
        bumped_flake->flake->vect.d_line = 0;

        //Modification de la ligne où se trouve à présent le glaçon
        bumped_flake->flake->pos.line = new_line;

        //Suppression du glaçon sur la banquise, à sa position précédente
        banquise->grid[previous_line][previous_col].object = NULL;

        //Ajout du glaçon sur la banquise, sur sa nouvelle position calculée
        banquise->grid[new_line][previous_col].object = bumped_flake;
    }
}

void moveFlakeRight(T_object *bumped_flake, T_banquise *banquise)
{
    /*Récupération des coordonnées de la case où se trouve initialement le glaçon*/
    int previous_line = bumped_flake->flake->pos.line;
    int previous_col = bumped_flake->flake->pos.col;
    int new_col = previous_col;

    /*Assignation d'une vitesse au glaçon bougé (ou 'bumpé'...)*/
    bumped_flake->flake->vect.d_col = +1;

    /*Déplacement du glaçon tant qu'il peut se déplacer*/
    while(IsInbound(BANQUISE_SIZE, previous_line, new_col + 1))
    {
        if(IsCaseAvailable(banquise->grid[previous_line][new_col + 1]))
            new_col += bumped_flake->flake->vect.d_col;
        else
            break;
    }


    /*Lorsque le glaçon s'arrête on vérifie si l'arrêt a été provoqué par un voisin avec lequel il peut interagir...*/
    if(IsFlakeIN(BANQUISE_SIZE, banquise, previous_line, new_col + 1))
    {
        //Le glaçon s'arrête avant l'objet avec lequel interagir
        bumped_flake->flake->vect.d_line = 0;
        bumped_flake->flake->pos.col = new_col;

        //L'interaction est réalisée
        FlakeInteraction(bumped_flake, previous_line, new_col + 1, banquise);

        //Le glaçon est détruit de la banquise puis de la mémoire
        banquise->grid[previous_line][previous_col].object = NULL;
        free(bumped_flake);
    }


    /* ... Ou pas */
    else
    {
        //Annulation de la vitesse du glaçon
        bumped_flake->flake->vect.d_col = 0;

        //Modification de la ligne où se trouve à présent le glaçon
        bumped_flake->flake->pos.col = new_col;

        //Suppression du glaçon sur la banquise, à sa position précédente
        banquise->grid[previous_line][previous_col].object = NULL;

        //Ajout du glaçon sur la banquise, sur sa nouvelle position calculée
        banquise->grid[previous_line][new_col].object = bumped_flake;
    }
}
