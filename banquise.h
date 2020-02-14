//
// Created by florian on 13/02/20.
//

#ifndef BANQUISE_BANQUISE_H
#define BANQUISE_BANQUISE_H

/*
 * Dans Ice: code 0 = eau, code 1 = glace
 * Dans Player: code 0= il n'y a pas de joueurs, code 1 = il y a un joueur
 * Dans Object: code 0= glaçon, code 1=rocher, code 2=ressort, code 3=marteau, code 4=piege
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


T_banquise initBanquise(int size);
void addWater(T_banquise *banquise);
void addRocks(T_banquise *banquise);
void printBanquise(T_banquise *banquise);

#endif //BANQUISE_BANQUISE_H
