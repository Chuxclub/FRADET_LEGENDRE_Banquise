//
// Created by florian on 13/02/20.
//

#include "banquise.h"


/* ============================================ */
/* ========= INITIALISATION BANQUISE ========== */
/* ============================================ */


/*
    Auteur(e)(s): Florian Legendre
    Utilité: Cree un tableau de glace uniquement
    Fonctionnement: Cree une banquise (la structure sur laquelle évolueront les éléments du jeu) par allocation dynamique
                    -> Puis cree une matrice deux dimensions de cases par double allocation dynamique, cette matrice deux dimensions est assignée à la banquise
                    -> Et enfinn, chaque case de cette grille dorénavant assignée à la banquise est initialisée.
    Complexité en temps (au pire): En supposant que toutes les allocations dynamiques se font en temps constant, on a O(banquise_size²), size étant la taille de la banquise
                                   car l'initialisation de chaque case de la banquise demande à ce qu'on parcourt chaque case
    Hypothèse d'amélioration possible: /
*/
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


/*
    Auteur(e)(s): Florian Legendre
    Utilité: Cree une banquise avec un terrain genere aleatoirement
    Fonctionnement: Cree une banquise de glace, puis ajoute les variations de terrain et les drapeaux (point de départ et d'arrivée du jeu)
    Complexité en temps (au pire): Egale à la somme des complexités de addWater(), addRocks() et addFlags()
    Hypothèse d'amélioration possible: /
*/
T_banquise *initBanquise(int size)
{
    //Initialisation de la banquise (que de la glace)
    T_banquise *myBanquise = initRawBanquise(size);

    //Generation aleatoire des elements du terrain
    addWater(myBanquise, NB_WATER);
    addRocks(myBanquise, NB_ROCKS);
    addFlags(myBanquise);

    return myBanquise;
}


/* ============================================ */
/* ========== MODIFICATIONS BANQUISE ========== */
/* ============================================ */


/*
    Auteur(e)(s): Florian Legendre
    Utilité: Ajoute de l'eau aleatoirement dans la banquise, ne peut que remplacer de la glace grâce à la fonction IsPlacementAvailable()
    Fonctionnement: Un compteur, nb_water, nous donne le nombre d'eau qu'on souhaite ajouter sur la banquise et est décrémenté au fur et à mesure que l'eau est
                    ajoutée. Un nombre aléatoire (cf. la variable "loto") compris entre 0 et 100 est généré, la glace ne peut être ajoutée que si "loto" est compris
                    entre 0 et 10 (ce qui correspond donc à 10% de chance d'avoir une case qui devient de l'eau). Chaque case de la banquise est parcourue avec ce
                    pourcentage. Si arrivée à la fin de la banquise on n'a toujours pas ajouté l'ensemble des cases d'eau souhaité, on reparcourt de nouveau toutes
                    les cases de la banquise (d'où le while(counter > 0)...) Si ce compteur arrive à 0 pendant le parcourt des cases de la banquise, on arrête la fonction
                    par un return (cf. if(counter == 0)). Ainsi on s'assure que le bon nombre de cases d'eau est ajoutée de manière aléatoire.
    Complexité en temps (au pire): Dans le pire des cas, si la variable "loto" n'est jamais dans les 10% on obtient une boucle infinie. Cependant ce cas, bien que
                                   possible dans notre imagination, n'est ni théoriquement possible (la limite de cette probabilité tend vite vers 0) et donc ne l'est pas
                                   en pratique. Plus probable, on a au pire: O(banquise_size²) pour le parcours de chaque case de la banquise
    Hypothèse d'amélioration possible: Créer un tableau de position dont la taille (du tableau) correspond au nombre de cases d'eau voulues
                                       puis générer pour chaque case du tableau une position aléatoire. La difficulté sera alors de vérifier qu'il n'y a pas de doublons
                                       avec une fonction d'une complexité inférieure à la nôtre...
*/
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


/*
    Auteur(e)(s): Florian Legendre
    Utilité: Ajoute des rochers aleatoirement sur la banquise, remplace la glace, ne peut que remplacer de la glace grâce à la fonction IsPlacementAvailable()
    Fonctionnement: Le fonctionnement est identique à la fonction addWater().
    Complexité en temps (au pire): O(banquise_size²)
    Hypothèse d'amélioration possible: Même suggestion que pour addWater()
*/
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

                else if(loto < 5 && IsPlacementAvailable(banquise->grid[i][j]))
                {
                    banquise->grid[i][j].ground = rock;
                    counter --;
                }

            }
        }
    }

}



/*
    Auteur(e)(s): Florian Legendre
    Utilité: Ajoute les points de depart et d'arrivee. Note importante: Le point d'arrive ne peut etre qu'a trois lignes du haut du depart.
                                                                        Le point de depart dans les trois lignes en bas du tableau.
    Fonctionnement: Les variables (Xa, Ya) et (Xb, Yb) représentent respectivement qu'on cherche à générer des points A et B.
                    On les génère une première fois. Puis on vérifie avec "while(!(IsPlacementAvailable(...)))" si leur placement est bon.
                    Si le placement est valide (voir IsPlacementAvailable pour une définition d'un placement valide) alors on ne rentre
                    pas dans la boucle et on assigne directement le drapeau à la position générée (cf. "banquise->grid[Xa][Ya].flag = A;", même
                    chose pour B...) Sinon on rentre dans la boucle et on génère jusqu'à trouver une position correcte.
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: /
*/
void addFlags(T_banquise *banquise)
{
    int Xa = BANQUISE_SIZE - 1 - (rand() % 3);
    int Ya = rand() % BANQUISE_SIZE;

    int Xb = 0 + (rand() % 3);
    int Yb = rand() % BANQUISE_SIZE;

    //Si la position de A definie ci-dessus comporte un objet ou de l'eau on redefinit jusqu'a
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

/*
    Auteur(e)(s): Florian Legendre
    Utilité: S'occupe de mettre à jour un objet passé en paramètre sur la banquise. Par mise à jour on signifie -> débranchement
             sur la banquise de l'objet de son ancienne position, mise à jour de la position de l'objet et rebranchement de l'objet
             sur la banquise à ses nouvelles positions.
    Fonctionnement: Un switch() permet de gérer les différents types d'objets qui n'ont pas les mêmes structures
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: /
*/
void updateObjectOnBanquise(T_object *myObject, int new_line, int new_col, T_banquise *banquise)
{
    switch(myObject->object_type)
    {
        case flake:
            banquise->grid[myObject->flake->pos.line][myObject->flake->pos.col].object = NULL;
            myObject->flake->pos.line = new_line;
            myObject->flake->pos.col = new_col;
            banquise->grid[new_line][new_col].object = myObject;
            break;

        case hammer_head:
            banquise->grid[myObject->hammer_head->pos.line][myObject->hammer_head->pos.col].object = NULL;
            myObject->hammer_head->pos.line = new_line;
            myObject->hammer_head->pos.col = new_col;
            banquise->grid[new_line][new_col].object = myObject;
            break;

        default:
            printf("Object unknown in updateObjectOnBanquise() in banquise.c\n");
            break;
    }
}


/* ============================================ */
/* ============ CHEMIN DE A VERS B ============ */
/* ============================================ */

/*
    Auteur(e)(s): Amandine Fradet
    Utilité: Initialise la variable a 1 pour tester la fonction isARoad ulterieurement, 1 signifiera que la case n'est pas disponible pour effectuer un deplacement
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
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


/*
    Auteur(e)(s): Amandine Fradet
    Utilité: Transforme la banquise en 0, 1 ou 2 dans la variable T pour simplifier la recherche de la fonction isARoad
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
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


/*
    Auteur(e)(s): Amandine Fradet
    Utilité: Intialisation du tableau qui sauvegardera les coordonnées des cases déjà visitées
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
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


/*
    Auteur(e)(s): Amandine Fradet
    Utilité: Recursive cherchant un chemin de A vers B
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
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
    Auteur(e)(s): Amandine Fradet
    Utilité: Fonction qui appellent toutes celles aidant a verifier l'existence d'un chemin de A vers B
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
void road(T_game_parts theGame, int players)
{
    T_test T = collectInfos(theGame.banquise, initTest(BANQUISE_SIZE));
    T_pos *tab = initTab();
    isRoad(T, T.posA.line, T.posA.col, tab, 0);

    //n'a pas trouve de chemin
    if (T.B_find == 0)
    {
        char answer;
        printf("Il n'y a plus de chemin possible.\nVoulez-vous relancer une nouvelle partie ?\nPour oui : y\nPour non : n\n");
        getchar();
        scanf("%c", &answer);

        switch (answer)
        {
        case 'y':
            players = main_menu();
          //  theGame = initGame(players);
            break;

        case 'n':
            printf("Merci d'avoir joué");
            theGame.game_on = false;
            break;

        default:
            printf ("wrong input\n");
            break;
        }
    }
}

/* ============================================ */
/* ================ AFFICHAGES ================ */
/* ============================================ */

/*
    Auteur(e)(s): Florian Legendre
    Utilité: Affiche un code d'une case avec le symbole et/ou la couleur correspondante
    Fonctionnement: Des if/then/else contrôle d'abord la nature du terrain à la case passée en paramètre (si c'est de l'eau ou de la
                    roche rien ne peut être dessus donc on peut les afficher en premier). Puis s'il n'y a pas d'élément de terrain on
                    vérifie alors s'il y a des objets (car le joueur ne peut pas marcher dessus sauf pour les pièges).  Pour cette
                    même raison on contrôle ensuite les drapeaux (occurence plus rare que les objets donc on s'épargne dans la plupart
                    des cas un test inutile). Enfin la présence d'un joueur (ou de son corps). Finalement s'il n'y a rien d'indiqué
                    ci-dessus alors c'est qu'il n'y a rien d'autre que de la glace sur la case, on affiche donc, le cas échéant, de la glace.
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: Si un joueur tombe dans un piège, ce-dernier est affiché en priorité et donc le joueur en question n'est plus affiché.
                                       Il faudrait pouvoir faire clignoter la case où le joueur et le piège se superposent. Ou bien afficher le joueur en priorité
                                       et modifier la couleur du joueur pour faire comprendre qu'il est prisonnier d'un piège.
*/
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


/*
    Auteur(e)(s): Florian Legendre
    Utilité: Affiche la banquise entiere et donc revele son etat au joueur a l'instant de son appel
    Fonctionnement: Parcourt chaque case de la banquise et affiche le contenu du la case avec printCase(). La fonction affiche également
                    des separateurs (--- et |) pour delimiter les cases de la banquise et centre l'affichage de la banquise verticalement
                    (center_printf_row()) et horizontalement (center_printf_col()). Pour centrer, les informations de la taille de console
                    (grâce à windows.h) sont récupérées au moment de l'appel et passées en paramètre des fonctions énoncées précédemment.
                    Ce qui signifie que si le joueur redimensionne sa console en cours de jeu alors il doit jouer un tour pour voir un
                    rendimensionnement de l'affichage de la banquise.
    Complexité en temps (au pire): O(banquise_size²) car on parcourt chaque case de la banquise.
    Hypothèse d'amélioration possible: /
*/
void printBanquise(T_banquise *banquise)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    //Upper Border
    center_printf_row(rows / 6);
    center_printf_col(columns / 2 - (banquise->sizeB * 2 + 4));

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
                center_printf_col(columns / 2 - (banquise->sizeB * 2 + 4));
                printf("| ");
                counter = 0;
            }

            printCase(banquise->grid[i][j]);

            counter++;
        }

        //Mid-lines + Bottom border
        printf("\n");
        center_printf_col(columns / 2 - (banquise->sizeB * 2 + 4));

        for(int i = 0; i < banquise->sizeB * 4 + 1; i++)
            printf("-");
    }

    //Newline
    printf("\n");
}



/* ============================================ */
/* =================== FONTE ================== */
/* ============================================ */

//Calcule la fonte de la banquise
/*
    Auteur(e)(s):
    Utilité:
    Fonctionnement:
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible:
*/
void Fontebanquise (T_banquise *banquise)
{
    srand(time (NULL));

    //calcule le pourcentage de chance pour que la banquise fonde
    int fonte = rand()%100;

    T_test T = initTest(BANQUISE_SIZE);

    //enregistre les points d'eau de la banquise dans un tableau d'entiers
    //0 pour un point d'eau
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
            if ( (T.grid[k][l] == 0) && (fonte < 4) )
            {
                if ( (-1 < (l - 1)) && (banquise->grid[k][l - 1].flag == 0) && (banquise->grid[k][l - 1].player == NULL) )
                {
                    banquise->grid[k][l - 1].ground = 0;
                    banquise->grid[k][l - 1].object = NULL;
                }

                if ( (-1 < (k - 1)) && (banquise->grid[k - 1][l].flag == 0) && (banquise->grid[k - 1][l].player == NULL) )
                {
                    banquise->grid[k - 1][l].ground = 0;
                    banquise->grid[k - 1][l].object = NULL;
                }

                if ( ((l + 1) < BANQUISE_SIZE) && (banquise->grid[k][l + 1].flag == 0) && (banquise->grid[k][l + 1].player == NULL) )
                {
                    banquise->grid[k][l + 1].ground = 0;
                    banquise->grid[k][l + 1].object = NULL;
                }

                if ( ((k + 1) < BANQUISE_SIZE) && (banquise->grid[k + 1][l].flag == 0) && (banquise->grid[k + 1][l].player == NULL) )
                {
                    banquise->grid[k + 1][l].ground = 0;
                    banquise->grid[k + 1][l].object = NULL;
                }
            }
        }
    }
}

