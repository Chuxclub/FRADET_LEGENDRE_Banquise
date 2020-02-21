#ifndef USER_TYPES_H_INCLUDED
#define USER_TYPES_H_INCLUDED


/* ============================================ */
/* =============== TYPES JOUEURS ============== */
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
} T_player_infos;

typedef struct
{
    T_player_infos details;
    int id;
}T_player;



/* ============================================ */
/* ============== TYPES BANQUISE ============== */
/* ============================================ */
typedef enum{water = 0, ice} T_ice;
typedef enum{no_object = 0, flake, rock, spring, hammer, trap} T_object;

typedef struct
{
    T_ice ice;
    T_player *player;
    T_object object;
    int A;
    int B;
} T_case;

typedef struct
{
    T_case **grid;
    int size;
}T_banquise;

#endif // USER_TYPES_H_INCLUDED
