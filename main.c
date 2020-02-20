#include <stdio.h>
#include <stdlib.h>
#include "banquise.h"
#include "glacon.h"
#include "piege.h"
#include "ressort.h"
#include "utils.h"

#define BANQUISE_SIZE 10
#define BANQUISE_CASES (BANQUISE_SIZE * BANQUISE_SIZE)

int main()
{
    srand(time(NULL));

    //Initialisation de la banquise (que de la glace)
    T_banquise *myBanquise = initBanquise(BANQUISE_SIZE);

    //Initialise une variable avec le nombre de joueurs
    int nb_player = HowManyPlayers();


    addPlayers(myBanquise, nb_player);
    //Affichage
    printBanquise(myBanquise);

    return 0;
}
