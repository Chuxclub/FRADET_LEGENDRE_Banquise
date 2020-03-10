#ifndef MARTEAU_H_INCLUDED
#define MARTEAU_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "user_types.h"
#include "utils.h"
#include "constants.h"

T_hammer *initHammer();
T_object **initHammers(int nb_hammers);
void addHammers(T_banquise *banquise, T_object **hammers, int nb_hammers);

#endif // MARTEAU_H_INCLUDED
