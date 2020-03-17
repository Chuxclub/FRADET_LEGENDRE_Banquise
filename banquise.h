//
// Created by florian on 13/02/20.
//

#ifndef BANQUISE_BANQUISE_H
#define BANQUISE_BANQUISE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "user_types.h"
#include "utils.h"
#include "windows_colors.h" //Ajoute des couleurs au terminal Windows
#include "constants.h"


/* ============================================ */
/* ========= INITIALISATION BANQUISE ========== */
/* ============================================ */
T_banquise *initRawBanquise(int size);
T_banquise *initBanquise(int size);


/* ============================================ */
/* ========== MODIFICATIONS BANQUISE ========== */
/* ============================================ */
void addWater(T_banquise *banquise, int nb_water);
void addRocks(T_banquise *banquise, int nb_rocks);
void addFlags(T_banquise *banquise);


/* ============================================ */
/* ============ CHEMIN DE A VERS B ============ */
/* ============================================ */
T_test initTest(int size);
T_test collectInfos(T_banquise *banquise, T_test T);
T_pos *initTab();
void isRoad(T_test T, int line, int col, T_pos *tab, int i);
void road(T_banquise *banquise, int endgame);


/* ============================================ */
/* ================ AFFICHAGES ================ */
/* ============================================ */
void printBanquise(T_banquise *banquise);
void printCase(T_case banquise_case);


/* ============================================ */
/* =================== FONTE ================== */
/* ============================================ */
void Fontebanquise (T_banquise *banquise);



#endif //BANQUISE_BANQUISE_H
