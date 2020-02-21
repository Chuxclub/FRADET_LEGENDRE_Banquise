#ifndef GLACON_H_INCLUDED
#define GLACON_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "banquise.h"
#include "joueur.h"
#include "user_types.h"
#include "utils.h"
#include "constants.h"

/* ============================================ */
/* ========== INITIALISATION GLACONS ========== */
/* ============================================ */

T_object **initFlakes(int nb_flakes);
void addFlakes(T_banquise *banquise, T_object **flakes, int nb_flakes);
//rencontre entre un glacon et un joueur
/*T_banquise MeetAFlake(T_player *banquise.grid.player, T_flake *banquise.grid.glacon);

//déplace les glacon après rencontre avec un joueur
T_banquise MoveAFlake(T_player *banquise.grid.player, T_flake *banquise.grid.glacon);

//tue un jour après une rencontre avec un glaçon
T_banquise KillPlayer(T_player *banquise.grid.player, T_flake *banquise.grid.glacon);
*/

#endif // GLACON_H_INCLUDED
