#ifndef USER_TYPES_H_INCLUDED
#define USER_TYPES_H_INCLUDED

/* ============================================ */
/* ============== TYPES GENERAUX ============== */
/* ============================================ */

typedef struct
{
    int line;
    int col;
} T_pos;

typedef struct
{
    int d_line;
    int d_col;
} T_vector;



/* ============================================ */
/* =============== TYPES OBJETS =============== */
/* ============================================ */
typedef struct
{
    T_pos pos;
    T_vector vect;
}T_flake;

typedef struct
{
    T_pos pos;
}T_trap;

typedef struct
{
    T_pos pos;
}T_spring;


typedef enum{no_object = 0, flake, spring, hammer, trap} T_object_type;


typedef struct
{
    T_object_type object_type;
    T_flake flake;
    T_trap trap;
    T_spring spring;
} T_object;

/* ============================================ */
/* =============== TYPES JOUEURS ============== */
/* ============================================ */
typedef enum {rouge = 1, bleu, vert, jaune} T_couleur;



typedef struct {
    int distance;
    int nb_ice;
    int kill;
} T_score;

typedef struct{
    char nom[50];
    T_couleur couleur;
    T_pos pos;
    T_vector vect;
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
typedef enum{water = 0, ice, rock} T_ground;
typedef enum{no_flag = 0, A, B} T_flag;

typedef struct
{
    T_ground ground;
    T_player *player;
    T_object *object;
    T_flag flag;
} T_case;

typedef struct
{
    T_case **grid;
    int size;
}T_banquise;

#endif // USER_TYPES_H_INCLUDED
