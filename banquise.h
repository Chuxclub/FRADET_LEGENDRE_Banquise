//
// Created by florian on 13/02/20.
//

#ifndef BANQUISE_BANQUISE_H
#define BANQUISE_BANQUISE_H

/*
 * Dans Ice: code 0 = eau, code 1 = glace
 * Dans Player: code 0 = il n'y a pas de joueurs, code 1 = il y le joueur 1, code 2 = il y a le joueur 2, etc.
 * Dans Object: code 0 = pas  d'objets, code 1= glaçon, code 2=rocher, code 3=ressort, code 4=marteau, code 5=piege
 * Dans A: code 0=la case n'est pas le point de départ, code 1=la case est le point de départ
 * Dans B: code 0=la case n'est pas le point d'arrivée, code 1=la case est le point d'arrivée
 */
typedef struct
{
    int ice;
    int player;
    int object;
    int A;
    int B;
} T_case;

typedef struct
{
    T_case **grid;
    int size;
}T_banquise;


T_banquise *initBanquise(int size);
void addWater(T_banquise *banquise);
void addRocks(T_banquise *banquise);
//void addFlakes(T_banquise *banquise);
//void addTraps(T_banquise *banquise);
//void addSprings(T_banquise *banquise);
//banquise Fontebanquise (banquise B)

void printBanquise(T_banquise *banquise);
void printCase(T_case banquise_case);

void addPlayers(T_banquise *banquise, int nb_players);
int IsPosAvailable(T_case banquise_case);
int *searchPos(T_banquise *banquise, int Ligne_a, int Col_a, int size_pos_tab[]);
int *searchPos(T_banquise *banquise, int Ligne_a, int Col_a, int size_pos_tab[]);

#endif //BANQUISE_BANQUISE_H
