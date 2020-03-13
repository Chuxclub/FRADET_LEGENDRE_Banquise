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


// ------------> D�placements
void accelerateUp(T_object *bumped_flake);
void accelerateLeft(T_object *bumped_flake);
void accelerateDown(T_object *bumped_flake);
void accelerateRight(T_object *bumped_flake);
void accelerateOpposite(T_object *bumped_flake);

// ------------> R�actions aux objets et � l'environnement
void BecomeIce(T_object *bumped_flake, int water_line, int water_col, T_banquise *banquise);
void BumpSpring(T_object *bumped_flake, int neighbour_line, int neighbour_col, T_banquise *banquise);
//void BumpSpring(T_object *bumped_flake);
void FlakeInteraction(T_object *bumped_flake, int neighbour_line, int neighbour_col, T_banquise *banquise);

// ------------> Regroupement d�placements et r�actions aux objets et � l'environnement
void updateFlakes(int nb_flakes, T_object **flakes,  T_banquise *banquise);


//rencontre entre un glacon et un joueur
/*T_banquise MeetAFlake(T_player *banquise.grid.player, T_flake *banquise.grid.glacon);

//d�place les glacon apr�s rencontre avec un joueur
T_banquise MoveAFlake(T_player *banquise.grid.player, T_flake *banquise.grid.glacon);

//tue un jour apr�s une rencontre avec un gla�on
T_banquise KillPlayer(T_player *banquise.grid.player, T_flake *banquise.grid.glacon);
*/

#endif // GLACON_H_INCLUDED
