#include "maths.h"

/* ===================================== */
/* ============== VECTORS ============== */
/* ===================================== */

//--------> Return origin vector


/*
    Auteur(e)(s): Florian Legendre
    Utilité: Ameliorer la lisibilite des fonctions qui ont besoin d'un vecteur nul.
    Fonctionnement: Initialise un vecteur nul ( (0,0) ) et le retourne
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: /
*/
T_vector null_vect()
{
    T_vector null_vect = {0, 0};
    return null_vect;
}

//--------> Vectors Operations

/*
    Auteur(e)(s): Florian Legendre
    Utilité: Permet d'additionner deux vecteurs et de renvoyer le résultat de cette addition
    Fonctionnement: Utilise la definition algébrique de l'addition de vecteurs
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: /
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
    Utilité: Meme chose que precedemment mais le resultat est stocke dans un vecteur C mutable (d'ou l'extension M_ au nom de la fonction)
    Fonctionnement: Meme chose que precedemment
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: /
*/
void M_add_vectors(T_vector A, T_vector B, T_vector *C)
{
    C->d_line = A.d_line + B.d_line;
    C->d_col = A.d_col + B.d_col;
}



/*
    Auteur(e)(s): Florian Legendre
    Utilité: Permet de soustraire deux vecteurs et de renvoyer le resultat de cette soustraction
    Fonctionnement: Utilise la definition algebrique de la soustraction de vecteurs
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: /
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
    Utilité: Meme chose que precedemment mais le resultat est stocke dans un vecteur C mutable (d'ou l'extension M_ au nom de la fonction)
    Fonctionnement: Meme chose que precedemment
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: /
*/
void M_subtract_vectors(T_vector A, T_vector B, T_vector *C)
{
    C->d_line = A.d_line - B.d_line;
    C->d_col = A.d_col - B.d_col;
}



/*
    Auteur(e)(s): Florian Legendre
    Utilité: Permet de multiplier un vecteur par un scalaire et de renvoyer le resultat de cette multiplication
    Fonctionnement: Utilise la definition algebrique de la multiplication de vecteurs par des scalaires
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: /
*/
T_vector multiply_vector(int k, T_vector A)
{
    A.d_line *= k;
    A.d_col *= k;

    return A;
}



/*
    Auteur(e)(s): Florian Legendre
    Utilité: Meme chose que precedemment mais le resultat est stocke dans un vecteur A mutable (d'ou l'extension M_ au nom de la fonction)
    Fonctionnement:  Meme chose que precedemment
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: /
*/
void M_multiply_vector(int k, T_vector *A)
{
    A->d_line *= k;
    A->d_col *= k;
}



/*
    Auteur(e)(s): Florian Legendre
    Utilité: Permet de calculer le produit scalaire de deux vecteurs, renvoit le résultat de ce calcul
    Fonctionnement: Utilise la définition algébrique du produit scalaire
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: /
*/
int scalar_product(T_vector A, T_vector B)
{
    return A.d_line * B.d_line + A.d_col * B.d_col;
}


//--------> Geometric Operations


/*
    Auteur(e)(s): Florian Legendre
    Utilité: Permet de translater les coordonnees d'un point selon une regle donnee par un vecteur
    Fonctionnement: Cf. Definition algebrique
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: /
*/
T_pos translate_point(T_pos a, T_vector A)
{
    a.line += A.d_line;
    a.col += A.d_col;

    return a;
}



/*
    Auteur(e)(s): Florian Legendre
    Utilité:  Meme chose que precedemment mais le resultat est stocke dans un point a mutable (d'ou l'extension M_ au nom de la fonction)
    Fonctionnement: Meme chose que precedemment
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: /
*/
void M_translate_point(T_pos *a, T_vector A)
{
    a->line += A.d_line;
    a->col += A.d_col;
}


