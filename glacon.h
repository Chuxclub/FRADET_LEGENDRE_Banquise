#ifndef GLACON_H_INCLUDED
#define GLACON_H_INCLUDED
#include "banquise.h"
#include "joueur.h"


typedef struct
{
    T_position pos;
    T_vecteur vector;
}T_flake;

//rencontre entre un glacon et un joueur
/*T_banquise MeetAFlake(T_player *banquise.grid.player, T_flake *banquise.grid.glacon);

//d�place les glacon apr�s rencontre avec un joueur
T_banquise MoveAFlake(T_player *banquise.grid.player, T_flake *banquise.grid.glacon);

//tue un jour apr�s une rencontre avec un gla�on
T_banquise KillPlayer(T_player *banquise.grid.player, T_flake *banquise.grid.glacon);
*/

#endif // GLACON_H_INCLUDED
