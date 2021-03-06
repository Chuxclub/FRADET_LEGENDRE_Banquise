#ifndef JOUEUR_H_INCLUDED
#define JOUEUR_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "user_types.h"
#include "utils.h"
#include "constants.h"
#include "console_funcs.h"
#include "glacon.h"




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
/* ============== SCORE JOUEURS =============== */
/* ============================================ */
void score_distance(T_player *player);
void score_flake(T_player *player);
void score_kill(T_player *player);
void print_score(T_player **player, int nb_players);


/* ============================================ */
/* =========== DEPLACEMENT JOUEURS ============ */
/* ============================================ */

//void moveUp(T_player *player, T_banquise *banquise);
void moveUp(T_player *player, T_game_parts *theGame);
void moveLeft(T_player *player, T_game_parts *theGame);
void moveDown(T_player *player, T_game_parts *theGame);
void moveRight(T_player *player, T_game_parts *theGame);
//void moveLeft(T_player *player, T_banquise *banquise);
//void moveDown(T_player *player, T_banquise *banquise);
//void moveRight(T_player *player, T_banquise *banquise);
void updatePlayer(T_player *myPlayer, int previous_line, int previous_col, T_banquise *banquise);


/* ============================================ */
/* ============== ETATS JOUEURS =============== */
/* ============================================ */

void killPlayer(T_player *myPlayer);
#endif // JOUEUR_H_INCLUDED
