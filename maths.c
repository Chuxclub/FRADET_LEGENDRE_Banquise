#include "maths.h"

/* ===================================== */
/* ============== VECTORS ============== */
/* ===================================== */

//--------> Return origin vector


/*
    Auteur(e)(s): Florian Legendre
    Utilit�: Ameliorer la lisibilite des fonctions qui ont besoin d'un vecteur nul.
    Fonctionnement: Initialise un vecteur nul ( (0,0) ) et le retourne
    Complexit� en temps (au pire): O(1)
    Hypoth�se d'am�lioration possible: /
*/
T_vector null_vect()
{
    T_vector null_vect = {0, 0};
    return null_vect;
}

//--------> Vectors Operations

/*
    Auteur(e)(s): Florian Legendre
    Utilit�: Permet d'additionner deux vecteurs et de renvoyer le r�sultat de cette addition
    Fonctionnement: Utilise la definition alg�brique de l'addition de vecteurs
    Complexit� en temps (au pire): O(1)
    Hypoth�se d'am�lioration possible: /
*/
T_vector add_vectors(T_vector A, T_vector B)
{
    T_vector C = {0, 0};
    C.d_line = A.d_line + B.d_line;
    C.d_col = A.d_col + B.d_col;

    return C;
}



/*
    Auteur(e)(s): Florian Legendre
    Utilit�: Meme chose que precedemment mais le resultat est stocke dans un vecteur C mutable (d'ou l'extension M_ au nom de la fonction)
    Fonctionnement: Meme chose que precedemment
    Complexit� en temps (au pire): O(1)
    Hypoth�se d'am�lioration possible: /
*/
void M_add_vectors(T_vector A, T_vector B, T_vector *C)
{
    C->d_line = A.d_line + B.d_line;
    C->d_col = A.d_col + B.d_col;
}



/*
    Auteur(e)(s): Florian Legendre
    Utilit�: Permet de soustraire deux vecteurs et de renvoyer le resultat de cette soustraction
    Fonctionnement: Utilise la definition algebrique de la soustraction de vecteurs
    Complexit� en temps (au pire): O(1)
    Hypoth�se d'am�lioration possible: /
*/
T_vector subtract_vectors(T_vector A, T_vector B)
{
    T_vector C = {0, 0};
    C.d_line = A.d_line - B.d_line;
    C.d_col = A.d_col - B.d_col;

    return C;
}



/*
    Auteur(e)(s): Florian Legendre
    Utilit�: Meme chose que precedemment mais le resultat est stocke dans un vecteur C mutable (d'ou l'extension M_ au nom de la fonction)
    Fonctionnement: Meme chose que precedemment
    Complexit� en temps (au pire): O(1)
    Hypoth�se d'am�lioration possible: /
*/
void M_subtract_vectors(T_vector A, T_vector B, T_vector *C)
{
    C->d_line = A.d_line - B.d_line;
    C->d_col = A.d_col - B.d_col;
}



/*
    Auteur(e)(s): Florian Legendre
    Utilit�: Permet de multiplier un vecteur par un scalaire et de renvoyer le resultat de cette multiplication
    Fonctionnement: Utilise la definition algebrique de la multiplication de vecteurs par des scalaires
    Complexit� en temps (au pire): O(1)
    Hypoth�se d'am�lioration possible: /
*/
T_vector multiply_vector(int k, T_vector A)
{
    A.d_line *= k;
    A.d_col *= k;

    return A;
}



/*
    Auteur(e)(s): Florian Legendre
    Utilit�: Meme chose que precedemment mais le resultat est stocke dans un vecteur A mutable (d'ou l'extension M_ au nom de la fonction)
    Fonctionnement:  Meme chose que precedemment
    Complexit� en temps (au pire): O(1)
    Hypoth�se d'am�lioration possible: /
*/
void M_multiply_vector(int k, T_vector *A)
{
    A->d_line *= k;
    A->d_col *= k;
}



/*
    Auteur(e)(s): Florian Legendre
    Utilit�: Permet de calculer le produit scalaire de deux vecteurs, renvoit le r�sultat de ce calcul
    Fonctionnement: Utilise la d�finition alg�brique du produit scalaire
    Complexit� en temps (au pire): O(1)
    Hypoth�se d'am�lioration possible: /
*/
int scalar_product(T_vector A, T_vector B)
{
    return A.d_line * B.d_line + A.d_col * B.d_col;
}


//--------> Geometric Operations


/*
    Auteur(e)(s): Florian Legendre
    Utilit�: Permet de translater les coordonnees d'un point selon une regle donnee par un vecteur
    Fonctionnement: Cf. Definition algebrique
    Complexit� en temps (au pire): O(1)
    Hypoth�se d'am�lioration possible: /
*/
T_pos translate_point(T_pos a, T_vector A)
{
    a.line += A.d_line;
    a.col += A.d_col;

    return a;
}



/*
    Auteur(e)(s): Florian Legendre
    Utilit�:  Meme chose que precedemment mais le resultat est stocke dans un point a mutable (d'ou l'extension M_ au nom de la fonction)
    Fonctionnement: Meme chose que precedemment
    Complexit� en temps (au pire): O(1)
    Hypoth�se d'am�lioration possible: /
*/
void M_translate_point(T_pos *a, T_vector A)
{
    a->line += A.d_line;
    a->col += A.d_col;
}


