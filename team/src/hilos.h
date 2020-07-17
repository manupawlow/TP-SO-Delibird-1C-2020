#ifndef HILOS_H_
#define HILOS_H_

#include "funciones_auxiliares.h"

t_list *crearEntrenadores();

void conexion_localized();
void conexion_appeared();
void conexion_caugth();
void conexion_gameboy();
void realizar_tareas(Entrenador *entrenador);
int process_request(int );
void poner_en_exce();
void deadlock();
void finalizar_si_corresponde(Entrenador *entrenador);
void llegada_pokemon(Poketeam *pokemon);
void round_robin(Entrenador *entrenador, int cpu);
void llegada_localized(t_list* localized);
void aumentar_ciclos(Entrenador *entrenador, int cant);

#endif /* HILOS_H_ */
