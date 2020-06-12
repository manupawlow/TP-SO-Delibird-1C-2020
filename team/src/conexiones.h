#ifndef CONEXIONES_H_
#define CONEXIONES_H_

#include "team.h"

t_list *crearEntrenadores();
void setearVariablesGlobales();
void realizar_tareas(Entrenador *entrenador);
void conexion_localized();
void conexion_appeared();
void conexion_caugth();
void conexion_gameboy();
void process_request(int );
bool necesitaPokemon(char *, t_list *);
void solicitar_pokemones(t_list *);
void poner_en_exce();

#endif /* CONEXIONES_H_ */
