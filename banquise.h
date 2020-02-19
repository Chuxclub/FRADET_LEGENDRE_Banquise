//
// Created by florian on 13/02/20.
//

#ifndef BANQUISE_BANQUISE_H
#define BANQUISE_BANQUISE_H

/* ============================================ */
/* ============ TYPES UTILISATEURS ============ */
/* ============================================ */
typedef enum{water = 0, ice} T_ice;
typedef enum{no_player = 0, player_1, player_2, player_3, player_4} T_player;
typedef enum{no_object = 0, flake, rock, spring, hammer, trap} T_object;

typedef struct
{
    T_ice ice;
    T_player player;
    T_object object;
    int A;
    int B;
} T_case;

typedef struct
{
    T_case **grid;
    int size;
}T_banquise;



/* ============================================ */
/* ========= INITIALISATION BANQUISE ========== */
/* ============================================ */
T_banquise *initRawBanquise(int size);



/* ============================================ */
/* ========== MODIFICATIONS BANQUISE ========== */
/* ============================================ */
void addWater(T_banquise *banquise);
void addRocks(T_banquise *banquise);
void addFlakes(T_banquise *banquise);
void addFlags(T_banquise *banquise);
//void addTraps(T_banquise *banquise);
//void addSprings(T_banquise *banquise);

int IsCaseAvailable(T_case banquise_case);
int *searchPos(T_banquise *banquise, int Ligne_a, int Col_a, int size_pos_tab[]);
void addPlayers(T_banquise *banquise, int nb_players);


/* ============================================ */
/* ================ AFFICHAGES ================ */
/* ============================================ */
void printBanquise(T_banquise *banquise);
void printCase(T_case banquise_case);


/* ============================================ */
/* =================== FONTE ================== */
/* ============================================ */
T_banquise Fontebanquise (T_banquise *B);



#endif //BANQUISE_BANQUISE_H
