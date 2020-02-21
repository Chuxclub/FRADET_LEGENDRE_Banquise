#ifndef JOUEUR_H_INCLUDED
#define JOUEUR_H_INCLUDED

#include "user_types.h"




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


#endif // JOUEUR_H_INCLUDED
