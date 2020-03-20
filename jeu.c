#include "jeu.h"

/*
    Auteur(e)(s): Florian Legendre
    Utilit�: Initialise tous les elements du jeu qui sont regroup�s dans la structure T_game_parts.
             Pour l'analogie, T_game_parts est comme la bo�te du jeu de plateau qui contient le plateau,
             les jetons, le d�s, etc. initGame() initialise cette bo�te (virtuelle) et son contenu
    Fonctionnement: Fait appel aux fonctions d'initialisation dans les modules correspondants et assigne
                    les ensembles d'objets ainsi initialis�s dans la "bo�te" du jeu (game_parts) qui est
                    retourn� en sortie.
    Complexit� en temps (au pire): O(banquise_size�) vu que toutes les fonctions d'initialisation ont cette complexit�
                                   et que la somme de complexit�s quadratiques au pire nous redonne une complexit� quadratique
    Hypoth�se d'am�lioration possible: Limiter le nombre de fonctions � complexit� quadratique
*/
T_game_parts initGame(int nb_players)
{

    T_player **players = initPlayers(nb_players);

    //Initialisation de la banquise (que de la glace)
    T_banquise *myBanquise = initBanquise(BANQUISE_SIZE);


    //Initialisation et ajout des objets sur la banquise
    T_object **hammers = initHammers(NB_HAMMERS);
    addHammers(myBanquise, hammers, NB_HAMMERS);

    T_object **flakes = initFlakes(NB_FLAKES);
    addFlakes(myBanquise, flakes, NB_FLAKES);

    T_object **traps = initTraps(NB_TRAPS);
    addTraps(myBanquise, traps, NB_TRAPS);

    T_object **springs = initSprings(NB_SPRINGS);
    addSprings(myBanquise, springs, NB_SPRINGS);

    //Recueil des quantit�s respectives pour chaque objet sur la banquise (index = type �num�r�)
    //Pour rappel: {no_object = 0, flake, spring, hammer_handle, hammer_head, trap, reserved} /!\ On ne compte pas les cases 'reserved'!
    //On remplace donc le comptage de 'reserved' par le comptage du nombre de joueurs
    int *nb_of_objects_per_object = (int *) malloc(sizeof(int) * 6);
    nb_of_objects_per_object[0] = 0;
    nb_of_objects_per_object[1] = NB_FLAKES;
    nb_of_objects_per_object[2] = NB_SPRINGS;
    nb_of_objects_per_object[3] = NB_HAMMERS;
    nb_of_objects_per_object[4] = NB_TRAPS;
    nb_of_objects_per_object[5] = nb_players;


    //Ajout des joueurs sur la banquise
    addPlayers(myBanquise, players, nb_players);

    //Rassemblement des parties du jeu pour les fonctions admins utiles aux tests
    T_game_parts game_parts;
    game_parts.banquise = myBanquise;
    game_parts.players = players;
    game_parts.flakes = flakes;
    game_parts.springs = springs;
    game_parts.traps = traps;
    game_parts.hammers = hammers;
    game_parts.quantities = nb_of_objects_per_object;
    game_parts.game_on = true;


    return game_parts;
}
