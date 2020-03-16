#ifndef MATHS_H_INCLUDED
#define MATHS_H_INCLUDED

#include "user_types.h"

/* ===================================== */
/* ============== VECTORS ============== */
/* ===================================== */

//--------> Return origin vector
T_vector null_vect();

//--------> Vectors Operations
T_vector add_vectors(T_vector A, T_vector B);
void M_add_vectors(T_vector A, T_vector B, T_vector *C);

T_vector subtract_vectors(T_vector A, T_vector B);
void M_subtract_vectors(T_vector A, T_vector B, T_vector *C);

T_vector multiply_vector(int k, T_vector A);
void M_multiply_vector(int k, T_vector *A);

int scalar_product(T_vector A, T_vector B);

//--------> Geometric Operations
T_pos translate_point(T_pos a, T_vector A);
void M_translate_point(T_pos *a, T_vector A);



#endif // MATHS_H_INCLUDED
