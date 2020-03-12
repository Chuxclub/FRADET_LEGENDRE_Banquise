#ifndef USER_TYPES_H_INCLUDED
#define USER_TYPES_H_INCLUDED

/* ============================================ */
/* =============== TYPES UTILES =============== */
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


//------------> Marteau
typedef enum {horizontal = 0, vertical} T_aspect;
typedef enum {no_rotation = 0, clockwise = 90, anticlockwise = -90} T_rot_dir;
typedef enum {no_momentum = 0, momentum_1, momentum_2, full_momentum = 3} T_momentum;

typedef struct
{
    int id;
    T_pos pos;
    T_aspect aspect;
} T_hammer_handle;

typedef struct
{
    int id;
    T_vector up_face;
    T_vector left_face;
    T_vector right_face;
    T_pos pos;
    T_momentum momentum;
    T_rot_dir rot_dir;
} T_hammer_head;

typedef struct
{
    T_hammer_handle *handle;
    T_hammer_head *head;
} T_hammer;
//------------> Fin Marteau


typedef enum{no_object = 0, flake, spring, hammer_handle, hammer_head, trap, reserved} T_object_type;


typedef struct
{
    T_object_type object_type;
    T_flake *flake;
    T_trap *trap;
    T_spring *spring;
    T_hammer_handle *hammer_handle;
    T_hammer_head *hammer_head;
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
    int sizeB;
}T_banquise;


/* ============================================ */
/* =============== AUTRES TYPES =============== */
/* ============================================ */

typedef struct
{
    T_banquise *banquise;
    T_player **players;
    T_object **flakes;
    T_object **springs;
    T_object **traps;
} T_game_parts;

typedef struct
{
    int **grid;
    T_pos posA;
    int B_find;
}T_test;

#endif // USER_TYPES_H_INCLUDED
