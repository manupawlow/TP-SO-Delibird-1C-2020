#ifndef HILOS_H_
#define HILOS_H_

#include "team.h"

t_list *crearEntrenadores();

void conexion_localized();
void conexion_appeared();
void conexion_caugth();
void conexion_gameboy();
void realizar_tareas(Entrenador *entrenador);
void process_request(int );
void poner_en_exce();
void deadlock();

void finalizar_si_corresponde(Entrenador *entrenador);
bool necesitaPokemon(char *, t_list *);
bool bloqueado_por_capturar(Entrenador *entrenador);

bool bloqueado_por_deadlock(Entrenador *entrenador);

#endif /* HILOS_H_ */
