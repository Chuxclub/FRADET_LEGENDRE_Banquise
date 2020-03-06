#ifndef TEST_FUNCS_H_INCLUDED
#define TEST_FUNCS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "user_types.h"
#include "utils.h"
#include "glacon.h"

void moveFlakeUp(T_object *bumped_flake, T_banquise *banquise);
void moveFlakeLeft(T_object *bumped_flake, T_banquise *banquise);
void moveFlakeDown(T_object *bumped_flake, T_banquise *banquise);
void moveFlakeRight(T_object *bumped_flake, T_banquise *banquise);

#endif // TEST_FUNCS_H_INCLUDED
