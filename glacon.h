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

//déplace les glacon après rencontre avec un joueur
T_banquise MoveAFlake(T_player *banquise.grid.player, T_flake *banquise.grid.glacon);

//tue un jour après une rencontre avec un glaçon
T_banquise KillPlayer(T_player *banquise.grid.player, T_flake *banquise.grid.glacon);
*/

#endif // GLACON_H_INCLUDED
