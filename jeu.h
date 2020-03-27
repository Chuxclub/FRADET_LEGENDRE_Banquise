#ifndef JEU_H_INCLUDED
#define JEU_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "banquise.h"
#include "glacon.h"
#include "joueur.h"
#include "piege.h"
#include "ressort.h"
#include "menus.h"
#include "marteau.h"



T_game_parts initGame(int nb_players);
void free_game_parts(T_game_parts theGame);

#endif // JEU_H_INCLUDED
