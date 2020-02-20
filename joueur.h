#ifndef JOUEUR_H_INCLUDED
#define JOUEUR_H_INCLUDED


/* ============================================ */
/* ============ TYPES UTILISATEURS ============ */
/* ============================================ */

typedef enum {rouge = 1, bleu, vert, jaune} T_couleur;

typedef struct{
    int px;
    int py;
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
    T_couleur couleur;
    T_position pos;
    T_vecteur vecteur;
    T_score score;
} T_data;

typedef struct
{
    T_data **grid;
    int id;
}T_joueur;


/* ============================================ */
/* ========== Nombre de joueurs================ */
/* ============================================ */
int HowManyPlayers();



/* ============================================ */
/* ========= INITIALISATION JOUEURS =========== */
/* ============================================ */

int *searchAvailablePos(T_banquise *banquise, int Ligne_a, int Col_a);
void addPlayers(T_banquise *banquise, int nb_players);


#endif // JOUEUR_H_INCLUDED
