#include "maths.h"

/* ===================================== */
/* ============== VECTORS ============== */
/* ===================================== */

//--------> Return origin vector
T_vector null_vect()
{
    T_vector null_vect = {0, 0};
    return null_vect;
}

//--------> Vectors Operations
T_vector add_vectors(T_vector A, T_vector B)
{
    T_vector C = {0, 0};
    C.d_line = A.d_line + B.d_line;
    C.d_col = A.d_col + B.d_col;

    return C;
}

void M_add_vectors(T_vector A, T_vector B, T_vector *C)
{
    C->d_line = A.d_line + B.d_line;
    C->d_col = A.d_col + B.d_col;
}

T_vector subtract_vectors(T_vector A, T_vector B)
{
    T_vector C = {0, 0};
    C.d_line = A.d_line - B.d_line;
    C.d_col = A.d_col - B.d_col;

    return C;
}

void M_subtract_vectors(T_vector A, T_vector B, T_vector *C)
{
    C->d_line = A.d_line - B.d_line;
    C->d_col = A.d_col - B.d_col;
}


T_vector multiply_vector(int k, T_vector A)
{
    A.d_line *= k;
    A.d_col *= k;

    return A;
}

void M_multiply_vector(int k, T_vector *A)
{
    A->d_line *= k;
    A->d_col *= k;
}


int scalar_product(T_vector A, T_vector B)
{
    return A.d_line * B.d_line + A.d_col * B.d_col;
}


//--------> Geometric Operations
T_pos translate_point(T_pos a, T_vector A)
{
    a.line += A.d_line;
    a.col += A.d_col;

    return a;
}

void M_translate_point(T_pos *a, T_vector A)
{
    a->line += A.d_line;
    a->col += A.d_col;
}


