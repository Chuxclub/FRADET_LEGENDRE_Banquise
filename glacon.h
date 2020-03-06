#ifndef GLACON_H_INCLUDED
#define GLACON_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "user_types.h"
#include "utils.h"
#include "constants.h"


/* ============================================ */
/* ========== INITIALISATION GLACONS ========== */
/* ============================================ */

T_flake *initFlake();
T_object **initFlakes(int nb_flakes);
void addFlakes(T_banquise *banquise, T_object **flakes, int nb_flakes);


/* ============================================ */
/* =========== INTERACTIONS GLACONS =========== */
/* ============================================ */

void FlakeInteraction(T_object *bumped_flake, int neighbour_line, int neighbour_col, T_banquise *banquise);
void BecomeIce(T_object *bumped_flake, int water_line, int water_col, T_banquise *banquise);
void moveFlakeUp(T_object *bumped_flake, T_banquise *banquise);
void moveFlakeLeft(T_object *bumped_flake, T_banquise *banquise);
void moveFlakeDown(T_object *bumped_flake, T_banquise *banquise);
void moveFlakeRight(T_object *bumped_flake, T_banquise *banquise);



//rencontre entre un glacon et un joueur
/*T_banquise MeetAFlake(T_player *banquise.grid.player, T_flake *banquise.grid.glacon);

//déplace les glacon après rencontre avec un joueur
T_banquise MoveAFlake(T_player *banquise.grid.player, T_flake *banquise.grid.glacon);

//tue un jour après une rencontre avec un glaçon
T_banquise KillPlayer(T_player *banquise.grid.player, T_flake *banquise.grid.glacon);
*/

#endif // GLACON_H_INCLUDED
