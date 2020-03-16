#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include "user_types.h"
#include "constants.h"


//D�termine si une case est disponible (c'est-�-dire sans objet, ni eau, ni roche, que de la glace)
int IsCaseAvailable(T_case banquise_case);

int AreSurroundingsAvailable(T_banquise *banquise, int line, int col);

void PlaceReservedZone(T_banquise *banquise, int line, int col);

//D�termine s'il y a un gla�on � la case indiqu�e ou non
int IsFlake(T_case banquise_case);

int IsFlagB(T_case banquise_case);

//D�termine si le voisin peut interagir avec un gla�on ou non
int IsFlakeIN(int banquise_size, T_banquise *banquise, int neighbour_line, int neighbour_col);

//D�termine si une position est dans le plateau ou non
int IsInbound(int banquise_size, int line, int col);

//D�termine s'il y a un T_object pr�sent � la case indiqu�e
int IsObject(T_case banquise_case);

//D�termine s'il y a un ressort pr�sent � la case indiqu�e
int IsSpring(T_case banquise_case);

//D�termine s'il y a de l'eau � la case indiqu�e
int IsWater(T_case banquise_case);


/* =================================== */
/* ============== MATHS ============== */
/* =================================== */

int scalar_product(T_vector A, T_vector B);
T_vector multiply_vector(int k, T_vector A);
void M_multiply_vector(int k, T_vector *A);
T_pos translate_point(T_pos a, T_vector A);
void M_translate_point(T_pos *a, T_vector A);
int enum_cycle_right(int n, int max, int right);
int enum_cycle_left(int n, int max, int left);

#endif // UTILS_H_INCLUDED
