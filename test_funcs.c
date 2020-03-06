#include "test_funcs.h"

/* ============================================ */
/* =========== INTERACTIONS GLACONS =========== */
/* ============================================ */

void moveFlakeUp(T_object *bumped_flake, T_banquise *banquise)
{
    /*R�cup�ration des coordonn�es de la case o� se trouve initialement le gla�on*/
    int previous_line = bumped_flake->flake->pos.line;
    int previous_col = bumped_flake->flake->pos.col;
    int new_line = previous_line;

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


    /*Lorsque le gla�on s'arr�te on v�rifie si l'arr�t a �t� provoqu� par un voisin avec lequel il peut interagir...*/
    if(IsFlakeIN(BANQUISE_SIZE, banquise, new_line - 1, previous_col))
    {
        //Le gla�on s'arr�te avant l'objet avec lequel interagir
        bumped_flake->flake->vect.d_line = 0;
        bumped_flake->flake->pos.line = new_line;

        //L'interaction est r�alis�e
        FlakeInteraction(bumped_flake, new_line - 1, previous_col, banquise);

        //Le gla�on est d�truit de la banquise puis de la m�moire
        banquise->grid[previous_line][previous_col].object = NULL;
        free(bumped_flake);
    }


    /* ... Ou pas */
    else
    {
        //Annulation de la vitesse du gla�on
        bumped_flake->flake->vect.d_line = 0;

        //Modification de la ligne o� se trouve � pr�sent le gla�on
        bumped_flake->flake->pos.line = new_line;

        //Suppression du gla�on sur la banquise, � sa position pr�c�dente
        banquise->grid[previous_line][previous_col].object = NULL;

        //Ajout du gla�on sur la banquise, sur sa nouvelle position calcul�e
        banquise->grid[new_line][previous_col].object = bumped_flake;
    }
}

void moveFlakeLeft(T_object *bumped_flake, T_banquise *banquise)
{
    /*R�cup�ration des coordonn�es de la case o� se trouve initialement le gla�on*/
    int previous_line = bumped_flake->flake->pos.line;
    int previous_col = bumped_flake->flake->pos.col;
    int new_col = previous_col;

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


    /*Lorsque le gla�on s'arr�te on v�rifie si l'arr�t a �t� provoqu� par un voisin avec lequel il peut interagir...*/
    if(IsFlakeIN(BANQUISE_SIZE, banquise, previous_line, new_col - 1))
    {
        //Le gla�on s'arr�te avant l'objet avec lequel interagir
        bumped_flake->flake->vect.d_line = 0;
        bumped_flake->flake->pos.col = new_col;

        //L'interaction est r�alis�e
        FlakeInteraction(bumped_flake, previous_line, new_col - 1, banquise);

        //Le gla�on est d�truit de la banquise puis de la m�moire
        banquise->grid[previous_line][previous_col].object = NULL;
        free(bumped_flake);
    }


    /* ... Ou pas */
    else
    {
        //Annulation de la vitesse du gla�on
        bumped_flake->flake->vect.d_col = 0;

        //Modification de la ligne o� se trouve � pr�sent le gla�on
        bumped_flake->flake->pos.col = new_col;

        //Suppression du gla�on sur la banquise, � sa position pr�c�dente
        banquise->grid[previous_line][previous_col].object = NULL;

        //Ajout du gla�on sur la banquise, sur sa nouvelle position calcul�e
        banquise->grid[previous_line][new_col].object = bumped_flake;
    }
}

void moveFlakeDown(T_object *bumped_flake, T_banquise *banquise)
{
    /*R�cup�ration des coordonn�es de la case o� se trouve initialement le gla�on*/
    int previous_line = bumped_flake->flake->pos.line;
    int previous_col = bumped_flake->flake->pos.col;
    int new_line = previous_line;

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


    /*Lorsque le gla�on s'arr�te on v�rifie si l'arr�t a �t� provoqu� par un voisin avec lequel il peut interagir...*/
    if(IsFlakeIN(BANQUISE_SIZE, banquise, new_line + 1, previous_col))
    {
        //Le gla�on s'arr�te avant l'objet avec lequel interagir
        bumped_flake->flake->vect.d_line = 0;
        bumped_flake->flake->pos.line = new_line;

        //L'interaction est r�alis�e
        FlakeInteraction(bumped_flake, new_line + 1, previous_col, banquise);

        //Le gla�on est d�truit de la banquise puis de la m�moire
        banquise->grid[previous_line][previous_col].object = NULL;
        free(bumped_flake);
    }


    /* ... Ou pas */
    else
    {
        //Annulation de la vitesse du gla�on
        bumped_flake->flake->vect.d_line = 0;

        //Modification de la ligne o� se trouve � pr�sent le gla�on
        bumped_flake->flake->pos.line = new_line;

        //Suppression du gla�on sur la banquise, � sa position pr�c�dente
        banquise->grid[previous_line][previous_col].object = NULL;

        //Ajout du gla�on sur la banquise, sur sa nouvelle position calcul�e
        banquise->grid[new_line][previous_col].object = bumped_flake;
    }
}

void moveFlakeRight(T_object *bumped_flake, T_banquise *banquise)
{
    /*R�cup�ration des coordonn�es de la case o� se trouve initialement le gla�on*/
    int previous_line = bumped_flake->flake->pos.line;
    int previous_col = bumped_flake->flake->pos.col;
    int new_col = previous_col;

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


    /*Lorsque le gla�on s'arr�te on v�rifie si l'arr�t a �t� provoqu� par un voisin avec lequel il peut interagir...*/
    if(IsFlakeIN(BANQUISE_SIZE, banquise, previous_line, new_col + 1))
    {
        //Le gla�on s'arr�te avant l'objet avec lequel interagir
        bumped_flake->flake->vect.d_line = 0;
        bumped_flake->flake->pos.col = new_col;

        //L'interaction est r�alis�e
        FlakeInteraction(bumped_flake, previous_line, new_col + 1, banquise);

        //Le gla�on est d�truit de la banquise puis de la m�moire
        banquise->grid[previous_line][previous_col].object = NULL;
        free(bumped_flake);
    }


    /* ... Ou pas */
    else
    {
        //Annulation de la vitesse du gla�on
        bumped_flake->flake->vect.d_col = 0;

        //Modification de la ligne o� se trouve � pr�sent le gla�on
        bumped_flake->flake->pos.col = new_col;

        //Suppression du gla�on sur la banquise, � sa position pr�c�dente
        banquise->grid[previous_line][previous_col].object = NULL;

        //Ajout du gla�on sur la banquise, sur sa nouvelle position calcul�e
        banquise->grid[previous_line][new_col].object = bumped_flake;
    }
}
