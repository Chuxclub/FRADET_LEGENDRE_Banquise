#ifndef JOUEUR_H_INCLUDED
#define JOUEUR_H_INCLUDED

typedef enum {rouge = 1, bleu, vert, jaune} T_couleur;

typedef struct{
    int pos_x;
    int pos_y;
} T_position;

typedef struct{
    int dx;
    int dy;
} T_vecteur;

typedef struct {
    int distance;
    int nb_ice;
    int kill;
} T_score;

typedef struct{
    char nom[50];
    T_couleur color;
    int id;
    T_position pos;
    T_vecteur vector;
    T_score score_j;
} T_player;

#endif // JOUEUR_H_INCLUDED
