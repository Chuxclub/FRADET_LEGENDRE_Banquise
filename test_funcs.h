#ifndef TEST_FUNCS_H_INCLUDED
#define TEST_FUNCS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "user_types.h"
#include "utils.h"
#include "glacon.h"

// ------> Fonctions de test "en jeu"
/* ============================================ */
/* ========= FONCTIONS D'AJOUT EN JEU ========= */
/* ============================================ */

void adminAddWater(T_game_parts game_parts, int line, int col);

/* ====================================================== */
/* ========= FONCTIONS DE TELEPORTATIONS EN JEU ========= */
/* ====================================================== */

void adminTpPlayer(T_game_parts game_parts, int player_id, int line, int col);
void adminTpFlake(T_game_parts game_parts, int flake_id, int line, int col);
void adminTpSpring(T_game_parts game_parts, int spring_id, int line, int col);


/* ====================================================== */
/* ========= FONCTIONS D'IDENTIFICATION EN JEU ========= */
/* ====================================================== */

void adminInfoCase(T_game_parts game_parts, int line, int col);


/* ============================================ */
/* ============ FONCTION MAITRESSE ============ */
/* ============================================ */

void adminPrivileges(T_game_parts game_parts);



// ------> Fonctions de test "en code"
/* ============================================ */
/* =========== INTERACTIONS GLACONS =========== */
/* ============================================ */

void moveFlakeUp(T_object *bumped_flake, T_banquise *banquise);
void moveFlakeLeft(T_object *bumped_flake, T_banquise *banquise);
void moveFlakeDown(T_object *bumped_flake, T_banquise *banquise);
void moveFlakeRight(T_object *bumped_flake, T_banquise *banquise);

#endif // TEST_FUNCS_H_INCLUDED
