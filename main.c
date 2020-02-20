#include <stdio.h>
#include <stdlib.h>
#include "banquise.h"
#include "joueur.c"

#define BANQUISE_SIZE 10
#define BANQUISE_CASES (BANQUISE_SIZE * BANQUISE_SIZE)

int main()
{
    srand(time(NULL));

    //Initialisation de la banquise (que de la glace)
    T_banquise *myBanquise = initRawBanquise(BANQUISE_SIZE);

    //Initialise une variable avec le nombre de joueurs
    int nb_player = HowManyPlayers();

    //Génération aléatoire des éléments du terrain
    addWater(myBanquise);
    addRocks(myBanquise);
    addFlakes(myBanquise);
    addSprings(myBanquise);
    addTraps(myBanquise);

    addFlags(myBanquise);
    addPlayers(myBanquise, nb_player);


    //Affichage
    printBanquise(myBanquise);

    return 0;
}
