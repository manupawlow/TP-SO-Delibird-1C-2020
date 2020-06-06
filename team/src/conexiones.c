#include "conexiones.h"

void conexion_gameboy(){
	char *ip = "127.0.0.2";
	char *puerto = "5002";
	int socket_team = iniciar_servidor(ip,puerto);

    while(1){
    	int socket_cliente = esperar_cliente(socket_team);

    	process_request(socket_cliente);
    }

}

void conexion_appeared(Config_Team config_team){
	char *ip = config_team->ip_broker;
	char *puerto = config_team->puerto_broker;

	int conexionAppeared = crear_conexion(ip,puerto);

	if(conexionAppeared ==-1){
		log_info(logger,"Reintenando reconectar cada 10 segundos");
		conexionAppeared= reintentar_conexion(ip,puerto,10);
	}

	log_info(logger,"Me conecte al broker!");

	enviar_mensaje("Suscribime",conexionAppeared, SUS_APP);

	log_info(logger,"Me suscribi a la cola Appeared!");

}

void process_request(int socket_cliente){
	int cod_op;

	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) == -1)
				cod_op = -1;

	switch (cod_op){
	case APPEARED_POKEMON:
		log_info(logger,"Llego pokemon a la cola APPEARED!");

		char* pokemon = "Pikachu";
		Posicion pos;
		pos.x=2;
		pos.y=3;
		if(necesitaPokemon(pokemon, objetivoGlobal)){
			menorDistancia(entrenadores,pos);
			log_info(logger,"wowo!");

		}else
			log_info(logger,"No se necesita!");

		break;

	}
}

bool necesitaPokemon(char *pokemon, t_list *objetivoGlobal){

	for(int i=0; i< list_size(objetivoGlobal); i++){
		char *objetivo = list_get(objetivoGlobal,i);
		if(!strcmp(pokemon,objetivo)){
			list_remove(objetivoGlobal,i);
			return true;
		}
	}

	return false;
}

void conexion_localized(Config_Team *config_team){
	char *ip = config_team->ip_broker;
	char *puerto = config_team->puerto_broker;

	int conexionLocalized =	crear_conexion(ip,puerto);

	if(conexionLocalized ==-1){
		log_info(logger,"Reintenando reconectar cada 10 segundos");
        conexionLocalized= reintentar_conexion(ip,puerto,10);
	}

	log_info(logger,"Me conecte al broker!");

	enviar_mensaje("Suscribime",conexionLocalized, GET_POKEMON);

	log_info(logger,"Me suscribi a la cola LOCALIZED!");

	while(1){

	}

}

void solicitar_pokemones(t_list *objetivoGlobal, Config_Team *config_team){
	char *ip = config_team->ip_broker;
	char *puerto = config_team->puerto_broker;

	for(int i=0; i< list_size(objetivoGlobal); i++){
		int conexionGet = crear_conexion(ip,puerto);
		if(conexionGet == -1){
			log_info(logger,"No se pudo solicitar pokemon");
		}else{
		char *pokemon = list_get(objetivoGlobal,i);
		enviar_mensaje(pokemon,conexionGet,GET_POKEMON);
		close(conexionGet);
		}
	}
}

