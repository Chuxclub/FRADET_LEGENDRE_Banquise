#ifndef JOUEUR_H_INCLUDED
#define JOUEUR_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "user_types.h"
/*#include "glacon.h"
#include "banquise.h"
#include "glacon.h"*/
#include "utils.h"
#include "constants.h"




/* ============================================ */
/* ========== Nombre de joueurs================ */
/* ============================================ */

int HowManyPlayers();



/* ============================================ */
/* ========= INITIALISATION JOUEURS =========== */
/* ============================================ */

T_player **initPlayers(int nb_players);
int *searchAvailablePos(T_banquise *banquise, int Ligne_a, int Col_a);
void addPlayers(T_banquise *banquise, T_player **players, int nb_players);


/* ============================================ */
/* =========== DEPLACEMENT JOUEURS ============ */
/* ============================================ */

void moveUp(T_player *player, T_banquise *banquise);
void moveLeft(T_player *player, T_banquise *banquise);
void moveDown(T_player *player, T_banquise *banquise);
void moveRight(T_player *player, T_banquise *banquise);
void updatePlayer(T_player *myPlayer, int previous_line, int previous_col, T_banquise *banquise);


#endif // JOUEUR_H_INCLUDED
