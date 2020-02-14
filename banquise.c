//
// Created by florian on 13/02/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "banquise.h"
//#include "termcolor.h"

#define RAND_MAX 101

#define BANQUISE_SIZE 10
#define NB_OF_COORDINATES 2

//Crée un simple tableau de glace uniquement
T_banquise *initBanquise(int size)
{
    T_banquise *res;
    res = ( T_banquise* ) malloc(sizeof(T_banquise));

    res->size = size;
    res->grid = (T_case **) malloc(sizeof(T_case *) * size);

    for(int i = 0; i < size;  i++)
        res->grid[i] = (T_case *) malloc(sizeof(T_case) * size);

    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            res->grid[i][j].ice = 1;
            res->grid[i][j].player = 0;
            res->grid[i][j].object = 0;

            //res.grid[i][j].glacon.pos.px = 0;
            //res.grid[i][j].glacon.pos.py = 0;
            //res.grid[i][j].glacon.vectordx = 0;
            //res.grid[i][j].glacon.vectordy = 0;

            res->grid[i][j].A = 0;
            res->grid[i][j].B = 0;
        }

    }

    return res;
}

//Ajoute de l'eau aléatoirement dans la banquise, remplace la glace
void addWater(T_banquise *banquise)
{
    srand(time(NULL));

    for(int i = 0; i < banquise->size; i++)
    {
        for(int j = 0; j < banquise->size; j++)
        {
            int loto = rand() % RAND_MAX;

            if(loto < 15)
                banquise->grid[i][j].ice = 0;
        }
    }
}

//Ajoute des rochers aléatoirement sur la banquise, remplace la glace
//Mais ne remplace pas l'eau! Code pour rocher: object = 1
void addRocks(T_banquise *banquise)
{
    srand(time(NULL));

    for(int i = 0; i < banquise->size; i++)
    {
        for(int j = 0; j < banquise->size; j++)
        {
            int loto = rand() % RAND_MAX;

            if(loto < 25 && banquise->grid[i][j].ice == 1)
                banquise->grid[i][j].object = 2;
        }
    }
}


//ajoute un glacon alÃ©atoirement sur la banquise
//object = 0 pour un glacon
void addFlakes(T_banquise *banquise)
{
    srand(time(NULL));

    for(int i = 0; i < banquise->size; i++)
    {
        for(int j = 0; j < banquise->size; j++)
        {
            int snow = rand() % RAND_MAX;

            if(snow < 21 && banquise->grid[i][j].ice == 1)
                banquise->grid[i][j].object = 1;
        }
    }
}

/*
//ajoute un ressort alÃ©atoirement sur la banquise
//object = 3 pour un glacon
void addSprings(T_banquise *banquise)
{
    srand(time(NULL));

    for(int i = 0; i < banquise->size; i++)
    {
        for(int j = 0; j < banquise->size; j++)
        {
            int spring = rand() % RAND_MAX;

            if(spring < 21 && banquise->grid[i][j].ice == 1)
                banquise->grid[i][j].object = 3;
        }
    }
}

//ajoute un piege alÃ©atoirement sur la banquise
//object = 4 pour un piege
void addTraps(T_banquise *banquise)
{
    srand(time(NULL));

    for(int i = 0; i < banquise->size; i++)
    {
        for(int j = 0; j < banquise->size; j++)
        {
            int trap = rand() % RAND_MAX;

            if(trap < 21 && banquise->grid[i][j].ice == 1)
                banquise->grid[i][j].object = 5;
        }
    }
}
*/

//Ajoute les points de départ et d'arrivé
//Le point d'arrivé ne peut être qu'à trois lignes du haut du départ
//Le point de départ dans les trois lignes en bas du tableau
void addFlags(T_banquise *banquise)
{
    srand(time(NULL));

    int Xa = BANQUISE_SIZE - 1 - (rand() % 3);
    int Ya = rand() % BANQUISE_SIZE;

    int Xb = 0 + (rand() % 3);
    int Yb = rand() % BANQUISE_SIZE;

    //Si la position de A définie ci-dessus comporte un objet ou de l'eau on redéfinit jusqu'à
    //ce que ce soit bon!
    while(banquise->grid[Xa][Ya].object > 0 || banquise->grid[Xa][Ya].ice == 0)
    {
        Xa = BANQUISE_SIZE - 1 - (rand() % 3);
        Ya = rand() % BANQUISE_SIZE;
    }
    banquise->grid[Xa][Ya].A = 1;


    //Même chose pour B
    while((banquise->grid[Xb][Yb].object > 0 || banquise->grid[Xb][Yb].ice == 0))
    {
        Xb = 0 + (rand() % 3);
        Yb = rand() % BANQUISE_SIZE;
    }
    banquise->grid[Xb][Yb].B = 1;
}

//Teste si une case est disponible pour y placer un objet interactif ou un joueur
//Renvoit 1 si la case est disponible, 0 sinon
int IsCaseAvailable(T_case banquise_case)
{
    if((banquise_case.ice == 1 && banquise_case.object == 0) && (banquise_case.A != 1 && banquise_case.B != 1) && banquise_case.player == 0)
        return 1;

    else
        return 0;
}

//Recherche toutes les positions disponibles dans une zone de recherche
//que la fonction incrémente au fur et à mesure, s'arrête quand toute la banquise a été explorée ou que le tableau est non vide.
//N'est utilisée que pour les tests
int **searchInboundPos(T_banquise *banquise, int Ligne_a, int Col_a, int *size_pos_tab)
{
    /*Initialisation des constantes et variables nécessaires*/
    //pos_tab correspond aux positions qui seront
    //dans les limites du tableau
    int dist_A = 0;
    int size = 0;
    int found = 0;
    int ligne_max = BANQUISE_SIZE - 1;
    int col_max  = BANQUISE_SIZE - 1;
    int col_min  = 0;
    int **pos_tab = (int **) malloc(sizeof(int *) * BANQUISE_SIZE);

    for(int i = 0; i < BANQUISE_SIZE;  i++)
        pos_tab[i] = (int *) malloc(sizeof(int) * 2);

    do
    {
        /*Définition des limites de la zone de recherche*/
        //Positionnement des indices lignes/colonnes par rapport à A (Col_a, Ligne_a)
        //Et par rapport au rayon de recherche autour de A (dist_A)
        //Rectification de ces indices si ces-derniers dépassent le plateau de jeu
        int col_begin = Col_a - dist_A;
        int col_end = Col_a + dist_A;
        int ligne_begin = Ligne_a - dist_A;
        int ligne_end = Ligne_a + dist_A;

        while(ligne_end > ligne_max)
            ligne_end--;

        while(col_begin < col_min)
                col_begin++;

        while(col_end > col_max)
            col_end--;

        /*Balayage de la zone de recherche du haut en bas, de la gauche vers la droite*/
        //Sauvegarde des positions dans le tableau au fur et à mesure de la recherche et que celles-ci sont libres
        //Sauvegarde de la taille du tableau avec la variable size au fur et à mesure qu'on rajoute des positions
        size = 0;
        for(int ligne_index = ligne_begin; ligne_index <= ligne_end; ligne_index++)
        {
            for(int col_index = col_begin; col_index <= col_end; col_index++)
            {
                if(IsCaseAvailable(banquise->grid[ligne_index][col_index]))
                {
                    pos_tab[size][0] = ligne_index;
                    pos_tab[size][1] = col_index;
                    size++;
                }
            }
        }

        /*Extension de la zone et boucle aucune position de trouvée*/
        if(size > 0)
            found = 1;

        else
            dist_A++;

        /*Arrêt de la recherche si on est parti du coin haut gauche du plateau et qu'on n'a rien trouvé par balayage*/
        if((col_begin == 0 && ligne_begin == 0) && found == 0)
        {
            fprintf(stderr, "No available position to place player\(s\)\n");
            exit(EXIT_FAILURE);
        }

    }while(found == 0);

    *size_pos_tab = size;
    return pos_tab;
}

//Recherche une position disponible dans une zone de recherche
//Que la fonction incrémente au fur et à mesure, s'arrête quand toute la banquise a été explorée ou qu'une position a été trouvée
int *searchAvailablePos(T_banquise *banquise, int Ligne_a, int Col_a)
{
    /*Initialisation des constantes et variables nécessaires*/
    //pos_tab correspond aux positions qui seront
    //dans les limites du tableau
    int dist_A = 0;
    int found = 0;
    int ligne_max = BANQUISE_SIZE - 1;
    int col_max  = BANQUISE_SIZE - 1;
    int col_min  = 0;
    int *pos_tab = (int *) malloc(sizeof(int) * NB_OF_COORDINATES);

    do
    {
        /*Définition des limites de la zone de recherche*/
        //Positionnement des indices lignes/colonnes par rapport à A (Col_a, Ligne_a)
        //Et par rapport au rayon de recherche autour de A (dist_A)
        //Rectification de ces indices si ces-derniers dépassent le plateau de jeu
        int col_begin = Col_a - dist_A;
        int col_end = Col_a + dist_A;
        int ligne_begin = Ligne_a - dist_A;
        int ligne_end = Ligne_a + dist_A;

        while(ligne_end > ligne_max)
            ligne_end--;

        while(col_begin < col_min)
            col_begin++;

        while(col_end > col_max)
            col_end--;


        /*Balayage de la zone de recherche du haut en bas, de la gauche vers la droite*/
        //Sauvegarde des positions dans le tableau au fur et à mesure de la recherche et que celles-ci sont libres
        //Sauvegarde de la taille du tableau avec la variable size au fur et à mesure qu'on rajoute des positions
        for(int ligne_index = ligne_begin; ligne_index <= ligne_end; ligne_index++)
        {
            for(int col_index = col_begin; col_index <= col_end; col_index++)
            {
                if(IsCaseAvailable(banquise->grid[ligne_index][col_index]))
                {
                    pos_tab[0] = ligne_index;
                    pos_tab[1] = col_index;
                    found = 1;
                    break;
                }
            }

            if(found == 1)
                break;
        }

        /*Extension de la zone et boucle si aucune position de trouvée*/
        if(found == 0)
            dist_A++;

        /*Arrêt de la recherche si on est parti du coin haut gauche du plateau et qu'on n'a rien trouvé par balayage*/
        if((col_begin == 0 && ligne_begin == 0) && found == 0)
        {
            fprintf(stderr, "No available position to place player\(s\)\n");
            exit(EXIT_FAILURE);
        }
    }while(found == 0);

    return pos_tab;
}

//Ajoute les joueurs au plus près du point A sur la banquise
void addPlayers(T_banquise *banquise, int nb_players)
{
    /*Recherche point A (rappel: ne peut être que dans les trois dernières lignes)*/
    int Col_a = 0;
    int Ligne_a = 0;

    for(int ligne = BANQUISE_SIZE - 1; ligne > BANQUISE_SIZE - 4; ligne--)
    {
        for(int col = 0; col < BANQUISE_SIZE; col++)
        {
            if(banquise->grid[ligne][col].A == 1)
            {
                Col_a = col;
                Ligne_a = ligne;
            }
        }
    }

    /*Positionnement des joueurs*/
    //Boucle -> recherche d'une position la plus proche, ajout joueur, etc.
    for(int i = 1; i <= nb_players; i++)
    {
        int *found_pos = searchAvailablePos(banquise, Ligne_a, Col_a);
        int available_line = found_pos[0];
        int available_col = found_pos[1];
        banquise->grid[available_line][available_col].player = i;
        free(found_pos);
    }
}

//Affiche un code d'une case avec le symbole correspondant
//Suit ordre logique: A/B > object > player > ice

//Affiche un code d'une case avec le symbole correspondant
//Suit ordre logique: A/B > object > player > ice
void printCase(T_case banquise_case)
{
    //Si c'est de l'eau inutile de chercher plus loins, on l'affiche
    if(banquise_case.ice == 0)
    {
        printf("~"); //text_blue(stdout)
        printf(" | "); //text_white(stdout)
    }

    //S'il y a un objet, un drapeau ou un joueur on va l'afficher
    else if(banquise_case.object > 0 || banquise_case.A > 0 || banquise_case.B > 0 || banquise_case.player > 0)
    {
        //C'est soit un objet...
        if(banquise_case.object > 0)
        {
            switch(banquise_case.object)
            {
                case 1:
                    printf("*"); //text_yellow(stdout)
                    printf(" | "); //text_white(stdout)
                    break;

                case 2:
                    printf("o"); //text_yellow(stdout)
                    printf(" | "); //text_white(stdout)
                    break;

                case 3:
                    printf("@"); //text_blue(stdout)
                    printf(" | "); //text_white(stdout)
                    break;

                case 4:
                    printf("m"); //text_blue(stdout)
                    printf(" | "); //text_white(stdout)
                    break;

                case 5:
                    printf("!"); //text_blue(stdout)
                    printf(" | "); //text_white(stdout)
                    break;

                default:
                    perror("Wrong object value in printCase() in banquise.c");
                    exit(EXIT_FAILURE);
                    break;
            }
        }

        //Soit un drapeau
        else if(banquise_case.A > 0 || banquise_case.B > 0)
            {
               if(banquise_case.A == 1)
                {
                    printf("A"); //text_purple(stdout)
                    printf(" | "); //text_white(stdout)
                }

                else
                {
                    printf("B"); //text_purple(stdout)
                    printf(" | "); //text_white(stdout)
                }
            }

        //Soit un joueur
        else if(banquise_case.player > 0)
        {
            switch(banquise_case.player)
            {
                case 1:
                    printf("1"); //text_purple(stdout)
                    printf(" | "); //text_white(stdout)
                    break;

                case 2:
                    printf("2"); //text_purple(stdout)
                    printf(" | "); //text_white(stdout)
                    break;

                case 3:
                    printf("3"); //text_purple(stdout)
                    printf(" | "); //text_white(stdout)
                    break;

                case 4:
                    printf("4"); //text_purple(stdout)
                    printf(" | "); //text_white(stdout)
                    break;

                default:
                    perror("Wrong player value in printCase() in banquise.c");
                    exit(EXIT_FAILURE);
                    break;
                }
        }
    }

    //S'il n'y a ni eau, ni objet/drapeau/joueur, c'est de la glace
    else
        {
            printf("#"); //text_bold(stdout)
            printf(" | "); //text_white(stdout)
        }


}


/*
//calcule la fonte de la banquise
T_banquise Fontebanquise (T_banquise *banquise){
    srand(time (NULL));
for (int i = 0; i < banquise->size; i++){
    for (int j = 0; j < banquise->size; j++){
        if ((banquise->grid[i][j].ice = 0) && (rand()%(16) < 16)){   //fond avec un pourcentage de 15%
            banquise->grid[i][j-1].ice = 0;  // toutes les cases autour de ce point d'eau
            banquise->grid[i][j+1].ice = 0;
            banquise->grid[i-1][j].ice = 0;
            banquise->grid[i+1][j].ice = 0;
            j += 2; //ajoute plus 2 Ã  j pour ne pas recommencer avec le point d'eau nouvellement crÃ©Ã©
        }
    }
}
return *banquise;
}*/

//Affiche l'état de la banquise à l'instant de son appel
void printBanquise(T_banquise *banquise)
{
    //Upper Border
    printf("\n");
    for(int i = 0; i < banquise->size * 4 + 1; i++)
        printf("-"); //, text_white(stdout)

    //Grid
    int counter = banquise->size;

    for(int i = 0; i < banquise->size; i++)
    {
        for(int j = 0; j < banquise->size; j++)
        {
            if(counter == banquise->size)
            {
                printf("\n");

                //Right border of grid
                printf("| "); //, text_white(stdout)
                counter = 0;
            }

            printCase(banquise->grid[i][j]);

            counter++;
        }

        //Mid-lines + Bottom border
        printf("\n");
        for(int i = 0; i < banquise->size * 4 + 1; i++)
            printf("-");
    }

    //Newline
    printf("\n");
}
