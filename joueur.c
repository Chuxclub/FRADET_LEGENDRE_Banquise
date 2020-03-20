#include "joueur.h"


/* ============================================ */
/* ========== INITIALISATION JOUEURS ========== */
/* ============================================ */


/*
    Auteur(e)(s): Florian Legendre
    Utilité: Meme chose que pour initTraps() dans piege.c ou initSprings() dans ressort.c...
    Fonctionnement: Meme chose que pour initTraps() dans piege.c ou initSprings() dans ressort.c...
    Complexité en temps (au pire): O(nb_player)
    Hypothèse d'amélioration possible: /
*/
T_player **initPlayers(int nb_players)
{
    T_player **players = (T_player **) malloc(sizeof(T_player *) * nb_players);

    for(int i = 0; i < nb_players; i++)
        players[i] = (T_player *) malloc(sizeof(T_player));


    for(int i = 0; i < nb_players; i++)
    {
        players[i]->id = (i + 1);
        players[i]->details.freedom = player_free;
        players[i]->details.health = full_health;
    }

    return players;
}


/*
    Auteur(e)(s): Florian Legendre
    Utilité: Chercher autour du drapeau si une case est disponible pour pouvoir y placer un joueur
    Fonctionnement: On procede par balayage d'une zone qu'on definit par des variables dont on a veille a ce qu'elles
                    definissent des cases qui sont dans le plateau. Le balayage se fait alors ligne par ligne. Si aucune
                    cases correspondant a nos conditions de placement du joueur autour du drapeau ne correspond on etend le
                    rayon de recherche (cf. dist_A). Si on a balaye toutes les lignes possibles de la banquise et qu'on a rien trouve
                    on renvoit un message d'erreur qui sera gere plus tard dans le developpement. Au final, la fonction renvoit un tableau comprenant
                    les positions determinees par l'algorithme.
    Complexité en temps (au pire):
    Hypothèse d'amélioration possible: 1) La fonction reverifie des cases deja verifiees des qu'on etend le rayon de recherche.
                                          On peut eviter cela en ne procedant non plus par balayage mais par bandes de recherche.
                                          Le principe serait alors le suivant: definir la ligne du dessus, la ligne du dessous et les colonnes
                                          sur les cotes entre les lignes dans le rayon souhaite. Si une colonne ou ligne est hors du plateau on peut
                                          ne pas y faire de recherche. On evite alors de retester des cases deja testees.

                                       2) Au lieu de renvoyer un tableau d'entier on pourrait renvoyer un T_pos
                                       3) pos_tab est renvoye. On pourrait aussi bien eviter l'allocation dynamique
*/
int *searchAvailablePos(T_banquise *banquise, int Ligne_a, int Col_a)
{
    /*Initialisation des constantes et variables necessaires*/
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
        /*Definition des limites de la zone de recherche*/
        //Positionnement des indices lignes/colonnes par rapport a A (Col_a, Ligne_a)
        //Et par rapport au rayon de recherche autour de A (dist_A)
        //Rectification de ces indices si ces-derniers d�passent le plateau de jeu
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
        //Sauvegarde des positions dans le tableau au fur et a mesure de la recherche et que celles-ci sont libres
        //Sauvegarde de la taille du tableau avec la variable size au fur et a mesure qu'on rajoute des positions
        for(int ligne_index = ligne_begin; ligne_index <= ligne_end; ligne_index++)
        {
            for(int col_index = col_begin; col_index <= col_end; col_index++)
            {
                if(IsPlacementAvailable(banquise->grid[ligne_index][col_index]))
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

        /*Extension de la zone et boucle si aucune position de trouvee*/
        if(found == 0)
            dist_A++;

        /*Arret de la recherche si on est parti du coin haut gauche du plateau et qu'on n'a rien trouve par balayage*/
        if((col_begin == 0 && ligne_begin == 0) && found == 0)
        {
            fprintf(stderr, "No available position to place player\(s)\n");
            exit(EXIT_FAILURE);
        }
    }while(found == 0);

    return pos_tab;
}



/*
    Auteur(e)(s): Florian Legendre
    Utilité: Ajoute les joueurs au plus pres du point A sur la banquise
    Fonctionnement: La fonction recherche d'abord l'emplacement du drapeau A
    Complexité en temps (au pire): O(banquise_size²) du fait de la recherche de l'emplacement de A
    Hypothèse d'amélioration possible: 1) On pourrait ajouter a T_game_parts les champs flag_A et flag_B qui
                                          contiendrait les positions des drapeaux A et B determines par addFlags()
                                          On eviterait ainsi la recherche de complexite quadratique sur la banquise.
*/
void addPlayers(T_banquise *banquise, T_player **players, int nb_players)
{
    /*Recherche point A (rappel: ne peut �tre que dans les trois derni�res lignes)*/
    int Col_a = 0;
    int Ligne_a = 0;

    for(int line = BANQUISE_SIZE - 1; line > BANQUISE_SIZE - 4; line--)
    {
        for(int col = 0; col < BANQUISE_SIZE; col++)
        {
            if(banquise->grid[line][col].flag == A)
            {
                Col_a = col;
                Ligne_a = line;
            }
        }
    }


    /*Positionnement des joueurs*/
    //Boucle -> recherche d'une position la plus proche, ajout joueur, etc.
    for(int i = 0; i < nb_players; i++)
    {
        int *found_pos = searchAvailablePos(banquise, Ligne_a, Col_a);
        int available_line = found_pos[0];
        int available_col = found_pos[1];

        /* On mémorise dans le joueur sa position... */
        players[i]->details.pos.line = available_line;
        players[i]->details.pos.col = available_col;

        /* Puis on assigne le joueur à la banquise, à la position correspondante */
        banquise->grid[available_line][available_col].player = players[i];
        free(found_pos);
    }
}


/*
    Auteur(e)(s): Amandine Fradet et Florian Legendre
    Utilité: Demande et renvoit le nombre de joueur
    Fonctionnement: L'affichage de ce menu (integre a main_menu() dans menus.c) est gere par des
                    fonctions consultables dans console_funcs.c. Apres cet affichage on demande a l'utilisateur
                    d'entre un entier, on controle la validite de l'entier et, si l'entier est valide, on le renvoit.
                    Cela permettra au jeu de se lancer avec le nombre de joueurs souhaite sur la banquise.
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: /
*/
int HowManyPlayers()
{
    int nb_players;
    nb_players = 0;

    /* Recuperation des données de la console pour centrage de l'affichage par rapport aux colonnes/lignes */
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    //Saut de ligne pour éviter que le menu colle au titre
    printf("\n");


    center_printf_col(columns / 2 - (BANQUISE_SIZE * 2 + 4));
    printf("Combien de joueurs etes-vous ?\n\n");

    center_printf_col(columns / 2 - (BANQUISE_SIZE * 2 + 4));
    printf("-> 0 pour quitter le jeu\n");

    center_printf_col(columns / 2 - (BANQUISE_SIZE * 2 + 4));
    printf("-> 1 joueur\n");

    center_printf_col(columns / 2 - (BANQUISE_SIZE * 2 + 4));
    printf("-> 2 joueurs\n");

    center_printf_col(columns / 2 - (BANQUISE_SIZE * 2 + 4));
    printf("-> 3 joueurs\n");

    center_printf_col(columns / 2 - (BANQUISE_SIZE * 2 + 4));
    printf("-> 4 joueurs\n\n");

    scanf("%d", &nb_players);

    //Dans le cas où l'input ne correspond pas à un bon nombre de joueur
    //Ou à l'input correspondant à une sortie du jeu immédiate
    while(nb_players < 0 || nb_players > 4)
    {
        printf("Veuillez entrer une valeur correcte (1 ou 2 ou 3 ou 4) : ");
        scanf("%d", &nb_players);
    }

    return nb_players;
}



/* ============================================ */
/* =========== DEPLACEMENT JOUEURS ============ */
/* ============================================ */


/*
    Auteur(e)(s): Florian Legendre
    Utilité: La famille de fonction move() et ses derivees Up, Down, Left, ... permettent de deplacer le joueur d'une case dans la direction souhaitee.
    Fonctionnement: On copie les anciennes positions du joueur dans des variables locales. Puis on verifie si le joueur n'est pas piege. S'il est piege on
                    ne met pas a jour le joueur et on arrete la fonction avec un return. Sinon, on verifie que la nouvelle position candidate est bien dans le plateau.
                    Si ce n'est pas le cas la mise a jour est egalement ignoree et la fonction s'arrete. Sinon on verifie si la case est disponible: si c'est le cas
                    on deplace le joueur dans la direction souhaitee. Sinon on verifie s'il s'agit d'un objet avec lequel le joueur peut interagir. Si c'est le cas
                    on aiguille l'interaction a lancer. Sinon on ignore egalement la mise a jour du joueur dont le deplacement est alors ignore.

                    Note 1: else if(IsFlake(theGame->banquise->grid[new_line][previous_col]) && (scalar_product(theGame->banquise->grid[new_line][previous_col].object->flake->vect, up_vect) < 0))
                    Ce cas prevoit la possibilite que le joueur interagisse avec un glacon qui lui fonce dessus directement ou de biais (grace au produit scalaire). Si c'est le cas
                    alors on lance tout de meme l'interaction mais le joueur meurt en lancant cette interaction.

                    Note 2: else if(IsFlagB(theGame->banquise->grid[new_line][previous_col])) prevoit le cas ou le joueur aurait reussi a rejoindre le drapeau B. Si c'est le cas
                    on indique au jeu que ce-dernier doit se terminer.
    Complexité en temps (au pire): O(1) car toutes les sous-procedures sont en O(1)
    Hypothèse d'amélioration possible: 1) Comme pour moveFlake(), faire une fonction PlayerIN() qui aiguille les interactions. Cela ameliorerait la lisibilite de cette fonction.
*/
void moveUp(T_player *player, T_game_parts *theGame)
{
    /* Passage du joueur en paramètre pour récupérer sa position */
    int previous_line = player->details.pos.line;
    int previous_col = player->details.pos.col;
    int new_line = previous_line - 1;
    T_vector up_vect = {-1, 0};

    if(player->details.freedom != player_free)
    {
        player->details.freedom++;
        return;
    }


    if(IsInbound(theGame->banquise->sizeB, new_line, previous_col))
    {
        if(IsCaseAvailable(theGame->banquise->grid[new_line][previous_col]))
        {
            /* Modification de la position du joueur */
            player->details.pos.line = new_line;
            player->details.pos.col = previous_col;

            /* Assignation à la case voulue du joueur sur la banquise*/
            updatePlayer(player, previous_line, previous_col, theGame->banquise);
        }

        else
        {
            if(IsObject(theGame->banquise->grid[new_line][previous_col]))
            {
                if(IsFlake(theGame->banquise->grid[new_line][previous_col]) && (scalar_product(theGame->banquise->grid[new_line][previous_col].object->flake->vect, up_vect) >= 0))
                    accelerateUp(theGame->banquise->grid[new_line][previous_col].object);

                else if(IsFlake(theGame->banquise->grid[new_line][previous_col]) && (scalar_product(theGame->banquise->grid[new_line][previous_col].object->flake->vect, up_vect) < 0))
                {
                    accelerateUp(theGame->banquise->grid[new_line][previous_col].object);
                    player->details.health = dead;
                }


                else if(IsTrap(theGame->banquise->grid[new_line][previous_col]))
                {
                    /* Modification de la position du joueur */
                    player->details.pos.line = new_line;
                    player->details.pos.col = previous_col;

                    /* Assignation à la case voulue du joueur sur la banquise*/
                    updatePlayer(player, previous_line, previous_col, theGame->banquise);

                    /* Modification du statut 'freedom' du joueur */
                    player->details.freedom = player_trapped;
                }


            }

            else if(IsFlagB(theGame->banquise->grid[new_line][previous_col]))
                theGame->game_on = false;
        }
    }

}


void moveLeft(T_player *player, T_game_parts *theGame)
{
    if(player->details.freedom != player_free)
    {
        player->details.freedom++;
        return;
    }

    /* Passage du joueur en paramètre pour récupérer sa position */
    int previous_line = player->details.pos.line;
    int previous_col = player->details.pos.col;
    int new_col = previous_col - 1;
    T_vector left_vect = {0, -1};

    if(IsInbound(theGame->banquise->sizeB, previous_line, new_col))
    {
        if(IsCaseAvailable(theGame->banquise->grid[previous_line][new_col]))
        {
            /* Modification de la position du joueur */
            player->details.pos.line = previous_line;
            player->details.pos.col = new_col;

            /* Assignation à la case voulue du joueur sur la banquise*/
            updatePlayer(player, previous_line, previous_col, theGame->banquise);
        }

        else
        {
            if(IsObject(theGame->banquise->grid[previous_line][new_col]))
            {
                if(IsFlake(theGame->banquise->grid[previous_line][new_col]) && (scalar_product(theGame->banquise->grid[previous_line][new_col].object->flake->vect, left_vect) >= 0))
                    accelerateLeft(theGame->banquise->grid[previous_line][new_col].object);

                else if(IsFlake(theGame->banquise->grid[previous_line][new_col]) && (scalar_product(theGame->banquise->grid[previous_line][new_col].object->flake->vect, left_vect) < 0))
                {
                    accelerateLeft(theGame->banquise->grid[previous_line][new_col].object);
                    player->details.health = dead;
                }

                else if(IsTrap(theGame->banquise->grid[previous_line][new_col]))
                {
                    /* Modification de la position du joueur */
                    player->details.pos.line = previous_line;
                    player->details.pos.col = new_col;

                    /* Assignation à la case voulue du joueur sur la banquise*/
                    updatePlayer(player, previous_line, previous_col, theGame->banquise);

                    /* Modification du statut 'freedom' du joueur */
                    player->details.freedom = player_trapped;
                }
            }

            else if(IsFlagB(theGame->banquise->grid[previous_line][new_col]))
                theGame->game_on = false;
        }
    }
}

void moveDown(T_player *player, T_game_parts *theGame)
{
    /* Passage du joueur en paramètre pour récupérer sa position */
    int previous_line = player->details.pos.line;
    int previous_col = player->details.pos.col;
    int new_line = previous_line + 1;
    T_vector down_vect = {1, 0};

    if(player->details.freedom != player_free)
    {
        player->details.freedom++;
        return;
    }


    if(IsInbound(theGame->banquise->sizeB, new_line, previous_col))
    {
        if(IsCaseAvailable(theGame->banquise->grid[new_line][previous_col]))
        {
            /* Modification de la position du joueur */
            player->details.pos.line = new_line;
            player->details.pos.col = previous_col;

            /* Assignation à la case voulue du joueur sur la banquise*/
            updatePlayer(player, previous_line, previous_col, theGame->banquise);
        }

        else
        {
            if(IsObject(theGame->banquise->grid[new_line][previous_col]))
            {
                if(IsFlake(theGame->banquise->grid[new_line][previous_col]) && (scalar_product(theGame->banquise->grid[new_line][previous_col].object->flake->vect, down_vect) >= 0))
                    accelerateDown(theGame->banquise->grid[new_line][previous_col].object);

                else if(IsFlake(theGame->banquise->grid[new_line][previous_col]) && (scalar_product(theGame->banquise->grid[new_line][previous_col].object->flake->vect, down_vect) < 0))
                {
                    accelerateDown(theGame->banquise->grid[new_line][previous_col].object);
                    player->details.health = dead;
                }

                else if(IsTrap(theGame->banquise->grid[new_line][previous_col]))
                {
                    /* Modification de la position du joueur */
                    player->details.pos.line = new_line;
                    player->details.pos.col = previous_col;

                    /* Assignation à la case voulue du joueur sur la banquise*/
                    updatePlayer(player, previous_line, previous_col, theGame->banquise);

                    /* Modification du statut 'freedom' du joueur */
                    player->details.freedom = player_trapped;
                }
            }

            else if(IsFlagB(theGame->banquise->grid[new_line][previous_col]))
                theGame->game_on = false;
        }
    }
}

void moveRight(T_player *player, T_game_parts *theGame)
{
    /* Passage du joueur en paramètre pour récupérer sa position */
    int previous_line = player->details.pos.line;
    int previous_col = player->details.pos.col;
    int new_col = previous_col + 1;
    T_vector right_vect = {0, 1};

    if(player->details.freedom != player_free)
    {
        player->details.freedom++;
        return;
    }

    if(IsInbound(theGame->banquise->sizeB, previous_line, new_col))
    {
        if(IsCaseAvailable(theGame->banquise->grid[previous_line][new_col]))
        {
            /* Modification de la position du joueur */
            player->details.pos.line = previous_line;
            player->details.pos.col = new_col;

            /* Assignation à la case voulue du joueur sur la banquise*/
            updatePlayer(player, previous_line, previous_col, theGame->banquise);
        }

        else
        {
            if(IsObject(theGame->banquise->grid[previous_line][new_col]))
            {
                if(IsFlake(theGame->banquise->grid[previous_line][new_col]) && (scalar_product(theGame->banquise->grid[previous_line][new_col].object->flake->vect, right_vect) >= 0))
                    accelerateRight(theGame->banquise->grid[previous_line][new_col].object);

                else if(IsFlake(theGame->banquise->grid[previous_line][new_col]) && (scalar_product(theGame->banquise->grid[previous_line][new_col].object->flake->vect, right_vect) < 0))
                {
                    accelerateLeft(theGame->banquise->grid[previous_line][new_col].object);
                    player->details.health = dead;
                }

                else if(IsTrap(theGame->banquise->grid[previous_line][new_col]))
                {
                    /* Modification de la position du joueur */
                    player->details.pos.line = previous_line;
                    player->details.pos.col = new_col;

                    /* Assignation à la case voulue du joueur sur la banquise*/
                    updatePlayer(player, previous_line, previous_col, theGame->banquise);

                    /* Modification du statut 'freedom' du joueur */
                    player->details.freedom = player_trapped;
                }
            }

            else if(IsFlagB(theGame->banquise->grid[previous_line][new_col]))
                theGame->game_on = false;
        }
    }
}



/*
    Auteur(e)(s): Florian Legendre
    Utilité: Met à jour la banquise en fonction des nouvelles positions du joueur. Les anciennes coordonnées du joueurs doivent être données.
    Fonctionnement: Debranche le joueur de la banquise a son ancienne position, le rebranche a sa nouvelle position
    Complexité en temps (au pire): O(1)
    Hypothèse d'amélioration possible: /
*/
void updatePlayer(T_player *myPlayer, int previous_line, int previous_col, T_banquise *banquise)
{
    banquise->grid[previous_line][previous_col].player = NULL;
    banquise->grid[myPlayer->details.pos.line][myPlayer->details.pos.col].player = myPlayer;
}



/* ============================================ */
/* ============== ETATS JOUEURS =============== */
/* ============================================ */


/*
    Auteur(e)(s): Florian Legendre
    Utilité: Tuer un joueur en toute impunite
    Fonctionnement: On modifie sa variable d'etat health a "dead"
    Complexité en temps (au pire): O(1) -> Il faut generalement moins de temps pour detruire que pour construire!
    Hypothèse d'amélioration possible: kill -9 ? Sinon RAS
*/
void killPlayer(T_player *myPlayer)
{
    myPlayer->details.health = dead;
}
