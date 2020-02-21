#include "glacon.h"

T_object **initFlakes(int nb_flakes)
{
    T_object **flakes = (T_object **) malloc(sizeof(T_object *) * nb_flakes);

    for(int i = 0; i < nb_flakes; i++)
        flakes[i] = (T_object *) malloc(sizeof(T_object));


    for(int i = 0; i < nb_flakes; i++)
    {
        flakes[i]->object_type = flake;

        flakes[i]->flake.pos.line = 0;
        flakes[i]->flake.pos.col = 0;
        flakes[i]->flake.vect.d_line = 0;
        flakes[i]->flake.vect.d_col = 0;

        flakes[i]->spring.pos.line = 0;
        flakes[i]->spring.pos.col = 0;

        flakes[i]->trap.pos.line = 0;
        flakes[i]->trap.pos.col = 0;
    }

    return flakes;
}

void addFlakes(T_banquise *banquise, T_object **flakes, int nb_flakes)
{
    int counter = nb_flakes - 1;

    while(counter >= 0)
    {
        for(int i = 0; i < banquise->size; i++)
        {
            for(int j = 0; j < banquise->size; j++)
            {
                int loto_flake = rand() % PERCENT;

                if(counter < 0)
                    return;

                else if(loto_flake < 5 && IsCaseAvailable(banquise->grid[i][j]))
                {
                    flakes[counter]->flake.pos.line = i;
                    flakes[counter]->flake.pos.col = j;
                    banquise->grid[i][j].object = flakes[counter];
                    counter--;
                }
            }
        }
    }
}

/*
//Lorsque le joueur se trouve à un endroit où se situe un glacon
//Effectue le deplacement du glacon
T_banquise MeetAFlake(T_player *banquise.grid.player, T_flake *banquise.grid.glacon)
{
    if ((banquise.grid.player.pos.px == banquise.grid.glacon.pos.px)
       && (banquise.grid.player.pos.py == banquise.grid.glacon.pos.py))
    {
        if ((banquise.grid.glacon.vector.dx == 0)
            && (banquise.grid.glacon.vector.dy == 0))
        {
            MoveAFlake(banquise.grid.player, banquise.grid.glacon);
        }
        else
        {
            KillPlayer(banquise.grid.player, banquise.grid.glacon);
        }
    }
}


//effectue le deplacement du glacon par le joueur
//s'effectue unqiuement si les vecteurs du glacon sont a 0
T_banquise MoveAFlake(T_player *banquise.grid.player, T_flake *banquise.grid.glacon)
{
    banquise->grid.glacon.vector.dx = banquise.grid.player.vector.dx;
    banquise->grid.glacon.vector.dy = banquise.grid.player.vector.dy;

    return *banquise
}



//tue un joueur
//s'effectue uniquement si les vecteurs du glacon sont differents de 0
T_banquise KillPlayer(T_player *banquise.grid.player, T_flake *banquise.grid.glacon)
{
    //le joueur est mort
    banquise.grid.player.ak = 2;
    how_many -= 1;

    //le glacon redevient immobile
    banquise->grid.glacon.vector.dx = 0;
    banquise->grid.glacon.vector.dy = 0;

    return *banquise;
}
*/
