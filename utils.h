#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include "user_types.h"


int IsCaseAvailable(T_case banquise_case);
int IsFlake(T_case banquise_case);
int IsInbound(int banquise_size, int line, int col);


#endif // UTILS_H_INCLUDED
