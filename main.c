#include <stdio.h>
#include <stdlib.h>
#include "banquise.h"

#define BANQUISE_SIZE 10
#define BANQUISE_CASES (BANQUISE_SIZE * BANQUISE_SIZE)

int main()
{
    //Initialisation de la banquise (que de la glace)
    T_banquise *myBanquise = initBanquise(BANQUISE_SIZE);

    //Génération aléatoire des éléments du terrain
    addWater(myBanquise);
    addRocks(myBanquise);
    addFlags(myBanquise);
    addPlayers(myBanquise);

    //Affichage
    printBanquise(myBanquise);

    return 0;
}
