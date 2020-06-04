#ifndef CONEXIONES_H_
#define CONEXIONES_H_

#include "team.h"

//void conexion_localized(Config_Team *config_team);
void conexion_gameboy();
void process_request(int socket_cliente);
bool necesitaPokemon(char *pokemon, t_list *objetivoGlobal);


#endif /* CONEXIONES_H_ */
