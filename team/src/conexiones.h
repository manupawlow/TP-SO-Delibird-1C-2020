#ifndef CONEXIONES_H_
#define CONEXIONES_H_

#include "team.h"

void conexion_localized(Config_Team *);
void conexion_gameboy();
void process_request(int );
bool necesitaPokemon(char *, t_list *);
void solicitar_pokemones(t_list *, Config_Team *);


#endif /* CONEXIONES_H_ */
