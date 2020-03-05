//
// Created by florian on 13/02/20.
//
#include "banquise.h"
//

//Initialise un tableau pour sauvegarder la position ultÈrieure des drapeaux
T_flag_test *initTabFlag()
{
    T_flag_test *test;
    test = ( T_flag_test* ) malloc(sizeof(T_flag_test));

    test->find = 0;
    test->ptr_pos = (T_pos *) malloc(sizeof(T_pos ) * 2);

    for(int i = 0; i < 2; i++)
    {
        test->ptr_pos[i].col = 0;
        test->ptr_pos[i].line = 0;
    }

    return test;
}


//Cherche l'existence d'un chemin allant d'un point de dÈpart A ‡ un point d'arrivÈe B
//Renvoit 1 si il existe un chemin, 0 sinon
int isARoad(T_banquise banquise, T_flag_test *flag_test)
{
    int Xa = flag_test->ptr_pos[0].col;
    int Ya = flag_test->ptr_pos[0].line;
    int Xb = flag_test->ptr_pos[1].col;
    int Yb = flag_test->ptr_pos[1].line;

    if (( Xa == Xb) && (Ya == Yb))
    {
        flag_test->find = 1;
        return flag_test->find;
    }

    else
    {

    if((banquise.grid[Xa][Ya - 1].ground == ice) && (banquise.grid[Xa][Ya - 1].object == NULL) && (banquise.grid[Xa][Ya - 1].flag == 0))
    {
        banquise.grid[Xa][Ya - 1].flag = 1;
        flag_test->ptr_pos[0].line = Ya - 1;
        isARoad(banquise, flag_test);
    }
    if((banquise.grid[Xa][Ya + 1].ground == ice) && (banquise.grid[Xa][Ya + 1].object == NULL) && (banquise.grid[Xa][Ya + 1].player == 0))
    {
        banquise.grid[Xa][Ya + 1].flag = 1;
        flag_test->ptr_pos[0].line = Ya + 1;
        isARoad(banquise, flag_test);
    }
    if((banquise.grid[Xa - 1][Ya].ground == ice) && (banquise.grid[Xa - 1][Ya].object == NULL) && (banquise.grid[Xa - 1][Ya].player == 0))
    {
        banquise.grid[Xa - 1][Ya].flag = 1;
        flag_test->ptr_pos[0].col = Xa - 1;
        isARoad(banquise, flag_test);
    }
    if((banquise.grid[Xa + 1][Ya].ground == ice) && (banquise.grid[Xa + 1][Ya].object == NULL) && (banquise.grid[Xa + 1][Ya].player == 0))
    {
        banquise.grid[Xa + 1][Ya].flag = 1;
        flag_test->ptr_pos[0].col = Xa + 1;
        isARoad(banquise, flag_test);
    }

    return flag_test->find;
    }
}

void a_test(int findme)
{
    printf("%d\n", findme);
}

/* ============================================ */
/* ========= INITIALISATION BANQUISE ========== */
/* ============================================ */

//CrÈe un simple tableau de glace uniquement
T_banquise *initRawBanquise(int size)
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
            res->grid[i][j].ground = ice;
            res->grid[i][j].player = NULL;
            res->grid[i][j].object = NULL;
            res->grid[i][j].flag = no_flag;
        }

    }

    return res;
}

//CrÈe une banquise avec un terrain gÈnÈrÈ alÈatoirement
T_banquise *initBanquise(int size, T_flag_test *flag)
{
    //Initialisation de la banquise (que de la glace)
    T_banquise *myBanquise = initRawBanquise(size);

    //GÈnÈration alÈatoire des ÈlÈments du terrain
    addWater(myBanquise, NB_WATER);
    addRocks(myBanquise, NB_ROCKS);
    addFlags(myBanquise, flag);

    return myBanquise;
}


/* ============================================ */
/* ========== MODIFICATIONS BANQUISE ========== */
/* ============================================ */

//Ajoute de l'eau alÈatoirement dans la banquise, remplace la glace
void addWater(T_banquise *banquise, int nb_water)
{
    int counter = nb_water;

    while(counter > 0)
    {
        for(int i = 0; i < banquise->size; i++)
        {
            for(int j = 0; j < banquise->size; j++)
            {
                int loto = rand() % PERCENT;

                if(counter == 0)
                    return;

                else if(loto < 10 && IsCaseAvailable(banquise->grid[i][j]))
                {
                    banquise->grid[i][j].ground = water;
                    counter --;
                }

            }
        }
    }
}

//Ajoute des rochers alÈatoirement sur la banquise, remplace la glace
//Mais ne remplace pas l'eau! Code pour rocher: object = 1
void addRocks(T_banquise *banquise, int nb_rocks)
{
    int counter = nb_rocks;

    while(counter > 0)
    {
        for(int i = 0; i < banquise->size; i++)
        {
            for(int j = 0; j < banquise->size; j++)
            {
                int loto = rand() % PERCENT;

                if(counter == 0)
                    return;

                else if(loto < 10 && IsCaseAvailable(banquise->grid[i][j]))
                {
                    banquise->grid[i][j].ground = rock;
                    counter --;
                }

            }
        }
    }

}


//Ajoute les points de dÈpart et d'arrivÈe
//Le point d'arrivÈ ne peut Ítre qu'‡ trois lignes du haut du dÈpart
//Le point de dÈpart dans les trois lignes en bas du tableau

void addFlags(T_banquise *banquise, T_flag_test *flag)
{
    int Xa = BANQUISE_SIZE - 1 - (rand() % 3);
    int Ya = rand() % BANQUISE_SIZE;

    int Xb = 0 + (rand() % 3);
    int Yb = rand() % BANQUISE_SIZE;

    //Si la position de A dÈfinie ci-dessus comporte un objet ou de l'eau on redÈfinit jusqu'‡
    //ce que ce soit bon!
    while(!(IsCaseAvailable(banquise->grid[Xa][Ya])))
    {
        Xa = BANQUISE_SIZE - 1 - (rand() % 3);
        Ya = rand() % BANQUISE_SIZE;
    }
    banquise->grid[Xa][Ya].flag = A;

    //Sauvegarde de la position de A pour le test isARoad
    flag->ptr_pos[0].col = Xa;
    flag->ptr_pos[0].line = Ya;


    //MÍme chose pour B
    while(!(IsCaseAvailable(banquise->grid[Xb][Yb])))
    {
        Xb = 0 + (rand() % 3);
        Yb = rand() % BANQUISE_SIZE;
    }
    banquise->grid[Xb][Yb].flag = B;

    //Sauvegarde de la position de B pour le test isARoad
    flag->ptr_pos[1].col = Xb;
    flag->ptr_pos[1].line = Yb;


}



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
    else if(banquise_case.object != NULL || banquise_case.flag != no_flag || banquise_case.player != NULL)
    {
        //C'est soit un objet...
        if(banquise_case.object != NULL)
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

                case hammer:
                    color(2, 0); //En vert
                    printf("m"); //text_blue(stdout)
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


//Affiche l'Ètat de la banquise ‡ l'instant de son appel
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



/* ============================================ */
/* =================== FONTE ================== */
/* ============================================ */

/*
//Calcule la fonte de la banquise => Pour le besoin des tests, ne pas y faire appel maintenant
T_banquise Fontebanquise (T_banquise *banquise)
{
    srand(time (NULL));
    for (int i = 0; i < banquise->size; i++)
    {
        for (int j = 0; j < banquise->size; j++)
        {
            if ((banquise->grid[i][j].ice = 0) && (rand()%(16) < 16))
            {   //fond avec un pourcentage de 15%
                banquise->grid[i][j-1].ice = 0;  // toutes les cases autour de ce point d'eau
                banquise->grid[i][j+1].ice = 0;
                banquise->grid[i-1][j].ice = 0;
                banquise->grid[i+1][j].ice = 0;
                j += 2; //ajoute plus 2 √† j pour ne pas recommencer avec le point d'eau nouvellement cr√©√©
            }
        }
    }

    return *banquise;
}
*/
