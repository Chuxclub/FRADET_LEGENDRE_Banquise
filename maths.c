#include "maths.h"

/* ===================================== */
/* ============== VECTORS ============== */
/* ===================================== */

//--------> Return origin vector


/*
    Auteur(e)(s):
    Utilité:
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
T_vector null_vect()
{
    T_vector null_vect = {0, 0};
    return null_vect;
}

//--------> Vectors Operations

/*
    Auteur(e)(s):
    Utilité:
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
T_vector add_vectors(T_vector A, T_vector B)
{
    T_vector C = {0, 0};
    C.d_line = A.d_line + B.d_line;
    C.d_col = A.d_col + B.d_col;

    return C;
}



/*
    Auteur(e)(s):
    Utilité:
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
void M_add_vectors(T_vector A, T_vector B, T_vector *C)
{
    C->d_line = A.d_line + B.d_line;
    C->d_col = A.d_col + B.d_col;
}



/*
    Auteur(e)(s):
    Utilité:
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
T_vector subtract_vectors(T_vector A, T_vector B)
{
    T_vector C = {0, 0};
    C.d_line = A.d_line - B.d_line;
    C.d_col = A.d_col - B.d_col;

    return C;
}



/*
    Auteur(e)(s):
    Utilité:
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
void M_subtract_vectors(T_vector A, T_vector B, T_vector *C)
{
    C->d_line = A.d_line - B.d_line;
    C->d_col = A.d_col - B.d_col;
}



/*
    Auteur(e)(s):
    Utilité:
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
T_vector multiply_vector(int k, T_vector A)
{
    A.d_line *= k;
    A.d_col *= k;

    return A;
}



/*
    Auteur(e)(s):
    Utilité:
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
void M_multiply_vector(int k, T_vector *A)
{
    A->d_line *= k;
    A->d_col *= k;
}



/*
    Auteur(e)(s):
    Utilité:
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
int scalar_product(T_vector A, T_vector B)
{
    return A.d_line * B.d_line + A.d_col * B.d_col;
}


//--------> Geometric Operations


/*
    Auteur(e)(s):
    Utilité:
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
T_pos translate_point(T_pos a, T_vector A)
{
    a.line += A.d_line;
    a.col += A.d_col;

    return a;
}



/*
    Auteur(e)(s):
    Utilité:
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
void M_translate_point(T_pos *a, T_vector A)
{
    a->line += A.d_line;
    a->col += A.d_col;
}


