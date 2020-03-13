//
// Created by florian on 13/02/20.
//

#include "banquise.h"


/* ============================================ */
/* ========= INITIALISATION BANQUISE ========== */
/* ============================================ */

//Cr�e un simple tableau de glace uniquement
T_banquise *initRawBanquise(int size)
{
    T_banquise *res;

    res = ( T_banquise* ) malloc(sizeof(T_banquise));

    res->sizeB = size;
    res->grid = (T_case **) malloc(sizeof(T_case *) * size);

    for(int i = 0; i < size;  i++)
        res->grid[i] = (T_case *) malloc(sizeof(T_case) * size);

    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            res->grid[i][j].ground = ice;
            res->grid[i][j].player = NULL;
            res->grid[i][j].object = NULL;
            res->grid[i][j].flag = no_flag;
        }

    }

    return res;
}

//Cr�e une banquise avec un terrain g�n�r� al�atoirement
T_banquise *initBanquise(int size)
{
    //Initialisation de la banquise (que de la glace)
    T_banquise *myBanquise = initRawBanquise(size);

    //G�n�ration al�atoire des �l�ments du terrain
    addWater(myBanquise, NB_WATER);
    addRocks(myBanquise, NB_ROCKS);
    addFlags(myBanquise);

    return myBanquise;
}


/* ============================================ */
/* ========== MODIFICATIONS BANQUISE ========== */
/* ============================================ */

//Ajoute de l'eau al�atoirement dans la banquise, remplace la glace
void addWater(T_banquise *banquise, int nb_water)
{
    int counter = nb_water;

    while(counter > 0)
    {
        for(int i = 0; i < banquise->sizeB; i++)
        {
            for(int j = 0; j < banquise->sizeB; j++)
            {
                int loto = rand() % PERCENT;

                if(counter == 0)
                    return;

                else if(loto < 10 && IsPlacementAvailable(banquise->grid[i][j]))
                {
                    banquise->grid[i][j].ground = water;
                    counter --;
                }

            }
        }
    }
}

//Ajoute des rochers al�atoirement sur la banquise, remplace la glace
//Mais ne remplace pas l'eau! Code pour rocher: object = 1
void addRocks(T_banquise *banquise, int nb_rocks)
{
    int counter = nb_rocks;

    while(counter > 0)
    {
        for(int i = 0; i < banquise->sizeB; i++)
        {
            for(int j = 0; j < banquise->sizeB; j++)
            {
                int loto = rand() % PERCENT;

                if(counter == 0)
                    return;

                else if(loto < 10 && IsPlacementAvailable(banquise->grid[i][j]))
                {
                    banquise->grid[i][j].ground = rock;
                    counter --;
                }

            }
        }
    }

}


//Ajoute les points de d�part et d'arriv�e
//Le point d'arriv� ne peut �tre qu'� trois lignes du haut du d�part
//Le point de d�part dans les trois lignes en bas du tableau

void addFlags(T_banquise *banquise)
{
    int Xa = BANQUISE_SIZE - 1 - (rand() % 3);
    int Ya = rand() % BANQUISE_SIZE;

    int Xb = 0 + (rand() % 3);
    int Yb = rand() % BANQUISE_SIZE;

    //Si la position de A d�finie ci-dessus comporte un objet ou de l'eau on red�finit jusqu'�
    //ce que ce soit bon!
    while(!(IsPlacementAvailable(banquise->grid[Xa][Ya])))
    {
        Xa = BANQUISE_SIZE - 1 - (rand() % 3);
        Ya = rand() % BANQUISE_SIZE;
    }

    banquise->grid[Xa][Ya].flag = A;


    //M�me chose pour B
    while(!(IsPlacementAvailable(banquise->grid[Xb][Yb])))
    {
        Xb = 0 + (rand() % 3);
        Yb = rand() % BANQUISE_SIZE;
    }
    banquise->grid[Xb][Yb].flag = B;
}


/* ============================================ */
/* ============ CHEMIN DE A VERS B ============ */
/* ============================================ */

//initialise la variable � 1 pour tester la fonction isARoad ult�rieurement
// 1 signifiera que la case n'est pas disponible pour effectuer un d�placement
T_test initTest(int size)
{
    T_test T;

    T.B_find = 0;

    T.posA.col = 0;
    T.posA.line = 0;

    T.grid = (int **) malloc(sizeof(int *) * size);

    for (int i = 0 ; i < size ; i++)
    {
        T.grid[i] = (int *) malloc(sizeof(int) * size);

        for (int j = 0 ; j < size ; j++)
        {
            T.grid[i][j] = 1;
        }
    }
    return T;
}

//transforme la banquise en 0, 1 ou 2 dans la variable T pour simplifier la recherche de la fonction isARoad
T_test collectInfos(T_banquise *banquise, T_test T)
{
    for (int i = 0; i < BANQUISE_SIZE; i++)
    {

        for (int j = 0; j < BANQUISE_SIZE; j++)
        {
            //uniquement de la glace ou un joueur sur la case
            if ( (IsCaseAvailable(banquise->grid[i][j]) == 1) || (banquise->grid[i][j].player != NULL) )
            {
                T.grid[i][j] = 0;
            }

            //si la case contient le point d'arrivée B
            else if (banquise->grid[i][j].flag == 2)
            {
                T.grid[i][j] = 2;
            }

            //le point de d�part A
            else if (banquise->grid[i][j].flag == 1)
            {
                T.grid[i][j] = 0;
                T.posA.line = i;
                T.posA.col = j;
            }
        }
    }

    return T;
}

//intialisation du tableau qui sauvegardera les coordonnées des cases déjà visitées
T_pos *initTab()
{
    int s = BANQUISE_SIZE * BANQUISE_SIZE;
    T_pos *tab = (T_pos *)malloc(sizeof(T_pos)*s);
    for (int i = 0; i < BANQUISE_SIZE; i++)
    {
        tab[i].line = 0;
        tab[i].col = 0;
    }
    return tab;
}

//r�cursive cherchant un chemin de A vers B
void isRoad(T_test T, int line, int col, T_pos *tab, int i)
{

        if (T.grid[line][col] == 2)
        {
            T.B_find = 1;
        }
        //si la case n'a jamais été visitée
        //lancement des récursives

        //la case de gauche
        else if ( ((-1) < (col - 1)) && (T.grid[line][col - 1] != 1) )
        {
            T.grid[line][col] = 1;
            tab[i].col = col;
            tab[i].line = line;
            isRoad(T, line, col - 1, tab, i + 1);
        }
        //la case du haut
        else if ( ((-1) < (line - 1)) && (T.grid[line - 1][col] != 1) )
        {
            T.grid[line][col] = 1;
            tab[i].col = col;
            tab[i].line = line;
            isRoad(T, line - 1, col, tab, i + 1);
        }

        //la case de droite
        else if ( ((col + 1) < BANQUISE_SIZE) && (T.grid[line][col + 1] != 1) )
        {
            T.grid[line][col] = 1;
            tab[i].col = col;
            tab[i].line = line;
            isRoad(T, line, col + 1, tab, i + 1);
        }
        //la case du bas
        else if ( ((line + 1) < BANQUISE_SIZE) && (T.grid[line + 1][col] != 1) )
        {
            T.grid[line][col] = 1;
            tab[i].col = col;
            tab[i].line = line;
            isRoad(T, line + 1, col, tab, i + 1);
        }
        //cas où aucune de ces 4 possibilités ne fonctionne
        //on retourne sur la case précédente, enregistrée dans le tableau
        else if (i > 0)
        {
            T.grid[line][col] = 1;
            i -= 1;
            int  ncol = tab[i].col;
            int  nline = tab[i].line;
            isRoad(T, nline, ncol, tab, i);
        }
}

/*
void withFlakes(T_test T, int col, int line)
{

}

void aRoad(T_test T, int col, int line)
{
    isARoad(T, T.posA.col, T.posA.line);
    if (T.find == 0)
    {
        withFlakes(T, T.posA.col, T.posA.line);
    }

}*/

/* ============================================ */
/* ================ AFFICHAGES ================ */
/* ============================================ */

//Affiche un code d'une case avec le symbole correspondant
//Suit ordre logique: A/B > object > player > ice
void printCase(T_case banquise_case)
{
    //Si c'est de l'eau ou un rocher inutile de chercher plus loin, on l'affiche
    if(banquise_case.ground == water)
    {
        color(9, 9); //En bleu, fond bleu
        printf("~"); //text_blue(stdout)
        color(15, 0); //Couleur d'origine de la console = reset de la couleur
        printf(" | "); //text_white(stdout)
    }

    else if(banquise_case.ground == rock)
    {
        color(6, 6); //En kaki, fond kaki
        printf("o");
        color(15, 0);
        printf(" | ");
    }

    //S'il y a un objet, un drapeau ou un joueur on va l'afficher
    else if((banquise_case.object != NULL && banquise_case.object->object_type != reserved) || banquise_case.flag != no_flag || banquise_case.player != NULL)
    {
        //C'est soit un objet...
        if(banquise_case.object != NULL && banquise_case.object->object_type != reserved)
        {
            switch(banquise_case.object->object_type)
            {
                case flake:
                    color(11, 0); //En turquoise
                    printf("*"); //text_yellow(stdout)
                    color(15, 0);
                    printf(" | "); //text_white(stdout)
                    break;

                case spring:
                    color(2, 0); //En vert
                    printf("@"); //text_blue(stdout)
                    color(15, 0);
                    printf(" | "); //text_white(stdout)
                    break;

                case hammer_head:
                    color(2, 0); //En vert
                    printf("o"); //text_blue(stdout)
                    color(15, 0);
                    printf(" | "); //text_white(stdout)
                    break;

                case hammer_handle:
                    color(2, 0); //En vert
                    printf("-"); //text_blue(stdout)
                    color(15, 0);
                    printf(" | "); //text_white(stdout)
                    break;

                case trap:
                    color(4, 0); //En marron
                    printf("!"); //text_blue(stdout)
                    color(15, 0);
                    printf(" | "); //text_white(stdout)
                    break;

                default:
                    perror("Wrong object value in printCase() in banquise.c");
                    exit(EXIT_FAILURE);
                    break;
            }
        }

        //Soit un drapeau
        else if(banquise_case.flag != no_flag)
            {
               if(banquise_case.flag == A)
                {
                    color(5, 0);
                    printf("A"); //text_purple(stdout)
                    color(15, 0);
                    printf(" | "); //text_white(stdout)
                }

                else
                {
                    color(5, 0); //En violet
                    printf("B"); //text_purple(stdout)
                    color(15, 0);
                    printf(" | "); //text_white(stdout)
                }
            }

        //Soit un joueur
        else if(banquise_case.player != NULL)
        {
            if(banquise_case.player->details.health != dead)
            {
                switch(banquise_case.player->id)
                {
                    case 1:
                        color(12, 0); //En rouge
                        printf("1"); //text_purple(stdout)
                        color(15, 0);
                        printf(" | "); //text_white(stdout)
                        break;

                    case 2:
                        color(12, 0);
                        printf("2"); //text_purple(stdout)
                        color(15, 0);
                        printf(" | "); //text_white(stdout)
                        break;

                    case 3:
                        color(12, 0);
                        printf("3"); //text_purple(stdout)
                        color(15, 0);
                        printf(" | "); //text_white(stdout)
                        break;

                    case 4:
                        color(12, 0);
                        printf("4"); //text_purple(stdout)
                        color(15, 0);
                        printf(" | "); //text_white(stdout)
                        break;

                    default:
                        perror("Wrong player value in printCase() in banquise.c");
                        exit(EXIT_FAILURE);
                        break;
                }
            }

            else
                {
                    color(12, 0); //En rouge
                    printf("X"); //text_purple(stdout)
                    color(15, 0);
                    printf(" | ");
                }

        }
    }

    //S'il n'y a ni eau, ni objet/drapeau/joueur, c'est de la glace
    else
        {
            color(15, 15); //En blanc, fond blanc
            printf("#"); //text_bold(stdout)
            color(15, 0);
            printf(" | "); //text_white(stdout)
        }


}


//Affiche l'�tat de la banquise � l'instant de son appel
void printBanquise(T_banquise *banquise)
{
    //Upper Border
    printf("\n");
    for(int i = 0; i < banquise->sizeB * 4 + 1; i++)
        printf("-"); //, text_white(stdout)

    //Grid
    int counter = banquise->sizeB;

    for(int i = 0; i < banquise->sizeB; i++)
    {
        for(int j = 0; j < banquise->sizeB; j++)
        {
            if(counter == banquise->sizeB)
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
        for(int i = 0; i < banquise->sizeB * 4 + 1; i++)
            printf("-");
    }

    //Newline
    printf("\n");
}



/* ============================================ */
/* =================== FONTE ================== */
/* ============================================ */

//Calcule la fonte de la banquise => Pour le besoin des tests, ne pas y faire appel maintenant
T_banquise Fontebanquise (T_banquise *banquise)
{
    srand(time (NULL));

    //calcule le pourcentage de chance pour que la banquise fonde
    int fonte = rand()%100;

    T_test T = initTest(BANQUISE_SIZE);

    //enregistre les points d'eau de la banquise dans un tableau d'entiers
    for (int i = 0; i < BANQUISE_SIZE; i++)
    {
        for (int j = 0; j < BANQUISE_SIZE; j++)
        {
            if (banquise->grid[i][j].ground == 0)
            {
                T.grid[i][j] = 0;
            }
        }
    }

    //si un point d'eau est détecté dans le tableau, cela impactera la banquise
    //fonte doit être inférieure à 5 pour que la fonte opère
    for (int k = 0; k < BANQUISE_SIZE; k++)
    {
        for (int l = 0; l < BANQUISE_SIZE; l++)
        {
            if ( (T.grid[k][l] == 0) && (fonte < 100) )
            {
                if ( (banquise->grid[k][l - 1].flag == 0) && (banquise->grid[k][l - 1].player == NULL) )
                {
                    banquise->grid[k][l - 1].ground = 0;
                    banquise->grid[k][l - 1].object = NULL;
                }

                if ( (banquise->grid[k - 1][l].flag == 0) && (banquise->grid[k - 1][l].player == NULL) )
                {
                    banquise->grid[k - 1][l].ground = 0;
                    banquise->grid[k - 1][l].object = NULL;
                }

                if ( (banquise->grid[k][l + 1].flag == 0) && (banquise->grid[k][l + 1].player == NULL) )
                {
                    banquise->grid[k][l + 1].ground = 0;
                    banquise->grid[k][l + 1].object = NULL;
                }

                if ( (banquise->grid[k + 1][l].flag == 0) && (banquise->grid[k + 1][l].player == NULL) )
                {
                    banquise->grid[k + 1][l].ground = 0;
                    banquise->grid[k + 1][l].object = NULL;
                }

            }
        }
    }

    return *banquise;
}

