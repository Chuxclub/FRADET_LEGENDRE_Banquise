#ifndef JOUEUR_H_INCLUDED
#define JOUEUR_H_INCLUDED

typedef enum {rouge = 1, bleu, vert, jaune} couleur;

typedef struct{
    int pos_x;
    int pos_y;
} position;

typedef struct{
    int dx;
    int dy;
} vecteur;

typedef struct {
    int distance;
    int nb_ice;
    int kill;
} score;

typedef struct{
    char nom[50];
    couleur color;
    int id;
    position pos;
    vecteur vector;
    score score_j;
} joueur;

#endif // JOUEUR_H_INCLUDED
