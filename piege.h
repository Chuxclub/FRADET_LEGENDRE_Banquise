#ifndef PIEGE_H_INCLUDED
#define PIEGE_H_INCLUDED

T_trap *initTrap();
T_object **initTraps(int nb_traps);
void addTraps(T_banquise *banquise, T_object **traps, int nb_traps);


#endif // PIEGE_H_INCLUDED
