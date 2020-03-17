#include "marteau.h"


/* ============================================ */
/* ========== INITIALISATION MARTEAU ========== */
/* ============================================ */


T_hammer *initHammer()
{
    T_hammer_handle *res_handle = (T_hammer_handle *) malloc(sizeof(T_hammer_handle));
    T_hammer_head *res_head = (T_hammer_head *) malloc(sizeof(T_hammer_head));
    T_hammer *res_hammer = (T_hammer *) malloc(sizeof(T_hammer));

    res_handle->id = 0;
    res_handle->pos.line = 0;
    res_handle->pos.col = 0;
    res_handle->aspect = horizontal;

    //Le marteau est initialisé dans cette position: o-
    res_head->id = 0;
    res_head->up_face.d_line = 0;
    res_head->up_face.d_col = -1;

    res_head->left_face.d_line = +1;
    res_head->left_face.d_col = 0;


    //Initilisation des autres paramètres de la tête du marteau
    res_head->pos.line = 0;
    res_head->pos.col = 0;

    res_head->momentum = no_momentum;
    res_head->rot_dir = no_rotation;
    res_head->state = left;

    res_hammer->handle = res_handle;
    res_hammer->head = res_head;

    return res_hammer;
}

T_object **initHammers(int nb_hammers)
{
    T_object **hammers = (T_object **) malloc(sizeof(T_object *) * 2* nb_hammers);

    for(int i = 0; i < 2 * nb_hammers; i++)
        hammers[i] = (T_object *) malloc(sizeof(T_object));

    for(int i = 0; i < 2 * nb_hammers; i+=2)
    {
        T_hammer *myHammer = initHammer();

        hammers[i]->object_type = hammer_head;
        hammers[i + 1]->object_type = hammer_handle;

        hammers[i]->hammer_head = myHammer->head;
        hammers[i]->hammer_handle = NULL;

        hammers[i + 1]->hammer_head = NULL;
        hammers[i + 1]->hammer_handle = myHammer->handle;


        hammers[i]->flake = NULL;
        hammers[i + 1]->flake = NULL;

        hammers[i]->spring = NULL;
        hammers[i + 1]->spring = NULL;

        hammers[i]->trap = NULL;
        hammers[i + 1]->trap = NULL;
    }

    return hammers;
}

void addHammers(T_banquise *banquise, T_object **hammers, int nb_hammers)
{
    int counter = 2*nb_hammers - 1;

    while(counter >= 0)
    {
        for(int i = 0; i < banquise->sizeB; i++)
        {
            for(int j = 0; j < banquise->sizeB; j++)
            {
                int loto_hammer = rand() % PERCENT;

                if(counter < 0)
                    return;

                else if(loto_hammer < 5 && AreSurroundingsAvailable(banquise, i, j))
                {
                    int hammer_head_line = i + hammers[counter - 1]->hammer_head->up_face.d_line;
                    int hammer_head_col = j + hammers[counter - 1]->hammer_head->up_face.d_col;

                    //On calcule d'abord la position de la tête du marteau puis celle de sa poignée
                    hammers[counter - 1]->hammer_head->pos.line = hammer_head_line;
                    hammers[counter - 1]->hammer_head->pos.col = hammer_head_col;

                    hammers[counter]->hammer_handle->pos.line = i;
                    hammers[counter]->hammer_handle->pos.col = j;

                    //Branchement sur la banquise
                    PlaceReservedZone(banquise, i, j);
                    banquise->grid[i][j].object = hammers[counter];
                    banquise->grid[hammer_head_line][hammer_head_col].object = hammers[counter - 1];
                    counter -= 2;
                }
            }
        }
    }
}

void updateHammers(int nb_hammers, T_game_parts *game_parts)
{
    //Rappel: ordre = tête puis manche => hammers[0] est une tête de marteau, hammers[1] est un manche de marteau, etc.
    for(int i = 0; i < nb_hammers*2; i+=2)
    {
        if(game_parts->hammers[i]->hammer_head->momentum != no_momentum)
        {
            /* Phase 1 : On verifie qu'un joueur n'est pas a portee du marteau, si c'est le cas, le marteau tue le joueu*/
            int player_id;

            if(IsPlayerInRange(game_parts->hammers[i]->hammer_head, game_parts->banquise, &player_id))
                killPlayer(game_parts->players[player_id - 1]);


            /* Phase 2 : Mise a jour de la tete du marteau en utilisant des matrices d'etats-transitions */
            T_vector clockw[4] = {{-1, 1},{1, 1},{1, -1},{-1, -1}};
            T_vector anticlockw[4] = {{1, 1},{1, -1},{-1, -1},{-1, 1}};
            int head_state = game_parts->hammers[i]->hammer_head->state;

            //----> Sens horaire
            if(game_parts->hammers[i]->hammer_head->rot_dir == clockwise)
            {
                //Calculating new head pos and updating banquise
                T_pos previous_H_pos = {game_parts->hammers[i]->hammer_head->pos.line, game_parts->hammers[i]->hammer_head->pos.col};
                T_pos new_H_pos = {previous_H_pos.line + clockw[head_state].d_line, previous_H_pos.col + clockw[head_state].d_col};
                updateObjectOnBanquise(game_parts->hammers[i], new_H_pos.line, new_H_pos.col, game_parts->banquise);

                //Calculating and updating new upf and leftf vectors  for enabling further interactions with flake (cf. FlakeInteraction() in glacon.c)
                int previous_head_state = enum_cycle_left(head_state, 4, 1);
                M_add_vectors(game_parts->hammers[i]->hammer_head->up_face, clockw[head_state], &(game_parts->hammers[i]->hammer_head->up_face));
                M_add_vectors(game_parts->hammers[i]->hammer_head->left_face, clockw[previous_head_state], &(game_parts->hammers[i]->hammer_head->left_face));

                //Turn by turn calculation and update of vector carrier for further transmission to flake (when momentum reaches 0)
                T_vector H_vector_carrier = game_parts->hammers[i]->hammer_head->vector_carrier;
                M_subtract_vectors(clockw[head_state], H_vector_carrier, &(game_parts->hammers[i]->hammer_head->vector_carrier));

                //Changing state and decrementing momentum
                game_parts->hammers[i]->hammer_head->state = enum_cycle_right(game_parts->hammers[i]->hammer_head->state, 4, 1);
                game_parts->hammers[i]->hammer_head->momentum--;

                //Transferring vector carrier to flake when momentum reaches 0
                if(game_parts->hammers[i]->hammer_head->momentum == no_momentum)
                {
                    T_pos transfer_neighb = translate_point(game_parts->hammers[i]->hammer_head->pos, game_parts->hammers[i]->hammer_head->vector_carrier);

                    if(game_parts->banquise->grid[transfer_neighb.line][transfer_neighb.col].object->object_type == flake)
                    {
                        game_parts->banquise->grid[transfer_neighb.line][transfer_neighb.col].object->flake->vect = game_parts->hammers[i]->hammer_head->vector_carrier;
                        game_parts->hammers[i]->hammer_head->vector_carrier = null_vect();
                    }

                    else
                        game_parts->hammers[i]->hammer_head->vector_carrier = null_vect();
                }
            }

            //----> Sens antihoraire
            else
            {
                //Calculating new head pos and updating banquise
                T_pos previous_H_pos = {game_parts->hammers[i]->hammer_head->pos.line, game_parts->hammers[i]->hammer_head->pos.col};
                T_pos new_H_pos = {previous_H_pos.line + anticlockw[head_state].d_line, previous_H_pos.col + anticlockw[head_state].d_col};
                updateObjectOnBanquise(game_parts->hammers[i], new_H_pos.line, new_H_pos.col, game_parts->banquise);

                //Calculating and updating new upf and leftf vectors for enabling further interactions with flake (cf. FlakeInteraction() in glacon.c)
                int previous_head_state = enum_cycle_left(head_state, 4, 1);
                M_add_vectors(game_parts->hammers[i]->hammer_head->up_face, anticlockw[head_state], &(game_parts->hammers[i]->hammer_head->up_face));
                M_add_vectors(game_parts->hammers[i]->hammer_head->left_face, anticlockw[previous_head_state], &(game_parts->hammers[i]->hammer_head->left_face));

                //Turn by turn calculation and update of vector carrier for further transmission to flake (when momentum reaches 0)
                T_vector H_vector_carrier = game_parts->hammers[i]->hammer_head->vector_carrier;
                M_subtract_vectors(anticlockw[head_state], H_vector_carrier, &(game_parts->hammers[i]->hammer_head->vector_carrier));

                //Changing state and decrementing momentum each turn
                game_parts->hammers[i]->hammer_head->state = enum_cycle_left(game_parts->hammers[i]->hammer_head->state, 4, 1);
                game_parts->hammers[i]->hammer_head->momentum--;

                //Transferring vector carrier to flake when momentum reaches 0
                if(game_parts->hammers[i]->hammer_head->momentum == no_momentum)
                {
                    T_pos transfer_neighb = translate_point(game_parts->hammers[i]->hammer_head->pos, game_parts->hammers[i]->hammer_head->vector_carrier);

                    if(game_parts->banquise->grid[transfer_neighb.line][transfer_neighb.col].object->object_type == flake)
                    {
                        game_parts->banquise->grid[transfer_neighb.line][transfer_neighb.col].object->flake->vect = game_parts->hammers[i]->hammer_head->vector_carrier;
                        game_parts->hammers[i]->hammer_head->vector_carrier = null_vect();
                    }

                    else
                        game_parts->hammers[i]->hammer_head->vector_carrier = null_vect();
                }
            }
        }
    }
}

