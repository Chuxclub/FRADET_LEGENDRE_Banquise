#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "banquise.h"
#include "joueur.h"
#include "glacon.h"
//#include "termcolor.h"

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
