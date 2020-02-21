#include <stdlib.h>
#include "banquise.h"

//Teste si une case est disponible pour y placer un objet interactif ou un joueur
//Renvoit 1 si la case est disponible, 0 sinon
int IsCaseAvailable(T_case banquise_case)
{
    if((banquise_case.ice == ice && banquise_case.object == no_object) && (banquise_case.A != 1 && banquise_case.B != 1) && banquise_case.player == NULL)
        return 1;

    else
        return 0;
}
