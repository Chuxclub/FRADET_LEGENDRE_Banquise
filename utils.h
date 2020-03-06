#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include "user_types.h"
#include "constants.h"


int IsCaseAvailable(T_case banquise_case);
int IsObject(T_case banquise_case);
int IsFlake(T_case banquise_case);
int IsFlakeIN(int banquise_size, T_banquise *banquise, int neighbour_line, int neighbour_col);
int IsWater(T_case banquise_case);
int IsInbound(int banquise_size, int line, int col);


#endif // UTILS_H_INCLUDED
