#include <windows.h>


/*
0: noir

1: bleu fonc�
2: vert
3: bleu-gris
4: marron
5: pourpre
6: kaki
7: gris clair
8: gris
9: bleu
10: vert fluo
11: turquoise
12: rouge
13: rose fluo
14: jaune fluo
15: blanc
*/


/*
    Auteur(e)(s): StackOverFlow
    Utilit�: Simplifier la colorisation des outputs par la cr�ation d'une fonction color()
    Fonctionnement: ?
    Complexit� en temps (au pire): ?
    Hypoth�se d'am�lioration possible: /
*/
void color(int t,int f)
{
        HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(H,f*16+t);
}
