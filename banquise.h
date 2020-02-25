//
// Created by florian on 13/02/20.
//

#ifndef BANQUISE_BANQUISE_H
#define BANQUISE_BANQUISE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "user_types.h"
#include "windows_colors.h" //Ajoute des couleurs au terminal Windows
#include "constants.h"

T_flag_test *initTabFlag();



/* ============================================ */
/* ========= INITIALISATION BANQUISE ========== */
/* ============================================ */
T_banquise *initRawBanquise(int size);
T_banquise *initBanquise(int size, T_flag_test *flag);


/* ============================================ */
/* ========== MODIFICATIONS BANQUISE ========== */
/* ============================================ */
void addWater(T_banquise *banquise, int nb_water);
void addRocks(T_banquise *banquise, int nb_rocks);
void addFlags(T_banquise *banquise, T_flag_test *flag);


int isARoad(T_banquise banquise, T_flag_test *flag_test);
void a_test(int findme);

/* ============================================ */
/* ================ AFFICHAGES ================ */
/* ============================================ */
void printBanquise(T_banquise *banquise);
void printCase(T_case banquise_case);


/* ============================================ */
/* =================== FONTE ================== */
/* ============================================ */
//T_banquise Fontebanquise (T_banquise *B);



#endif //BANQUISE_BANQUISE_H
