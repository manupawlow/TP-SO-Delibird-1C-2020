#include "conexiones.h"

t_list *crearEntrenadores(){
	t_list *posiciones = config ->posiciones_entrenadores;
	t_list *pokemones = config ->pokemon_entrenadores;
	t_list *entrenadores = list_create();

	for(int i=0; i< list_size(posiciones); i++){
		Entrenador *entrenador = malloc(sizeof(Entrenador));
		pthread_t hiloEntrenador;
		pthread_create(&hiloEntrenador,NULL,(void *) realizar_tareas,entrenador);
		entrenador->pokemones=list_get(pokemones,i);
		entrenador->posicion=list_get(posiciones,i);
		entrenador->posicion_a_capturar=list_get(posiciones,i);
		sem_init(&entrenador->mx_entrenador,0,0);
		list_add(entrenadores,entrenador);
	}

	return entrenadores;
}

void setearVariablesGlobales(){
	ip= config->ip_broker;
	puerto= config->puerto_broker;

	sem_init(&semaforoExce,0,0);
	pthread_mutex_init(&mxExce,NULL);
}

void realizar_tareas(Entrenador *entrenador){
	sem_wait(&entrenador->mx_entrenador);

	int dis_x= abs(entrenador->posicion_a_capturar->x - entrenador->posicion->x);
	int dis_y= abs(entrenador->posicion_a_capturar->y - entrenador->posicion->y);
	int moverse = (dis_x + dis_y) * config->retardo_cpu + 7;

	log_info(logger,"Tiempo %d segundos:", moverse);

	sleep(moverse);
	entrenador->posicion->x = entrenador->posicion_a_capturar->x;
	entrenador->posicion->y = entrenador->posicion_a_capturar->y;

	log_info(logger,"Agarre pokemon!");

	pthread_mutex_unlock(&mxExce);

}

void poner_en_exce(){

	while(1){
		sem_wait(&semaforoExce);
		log_info(logger,"LLego entrenador!");
		pthread_mutex_lock(&mxExce);

		Entrenador *entrenador= list_get(ready,0);
		list_remove(ready,0);

		sem_post(&entrenador->mx_entrenador);
		log_info(logger,"Entrenador ejecutando");
	}

}

void conexion_gameboy(){
	char *ip = "127.0.0.2";
	char *puerto = "5002";
	int socket_team = iniciar_servidor(ip,puerto);

    while(1){
    	int socket_cliente = esperar_cliente(socket_team);

    	process_request(socket_cliente);
    }
}
/*
void conexion_appeared(Config_Team config_team){

	int conexionAppeared = crear_conexion(ip,puerto);

	if(conexionAppeared ==-1){
		log_info(logger,"Reintenando reconectar cada 10 segundos");
		conexionAppeared= reintentar_conexion(ip,puerto,10);
	}

	log_info(logger,"Me conecte al broker!");

	enviar_mensaje("Suscribime",conexionAppeared, SUS_APP);

	log_info(logger,"Me suscribi a la cola Appeared!");

}
*/

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
			menorDistancia(pos);

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

void conexion_localized(){

	int conexionLocalized =	crear_conexion(ip,puerto);

	if(conexionLocalized ==-1){
		log_info(logger,"Reintenando reconectar cada %d segundos", config->reconexion);
        conexionLocalized= reintentar_conexion(ip,puerto,config->reconexion);
	}

	log_info(logger,"Me conecte al broker!");

	enviar_mensaje("Suscribime",conexionLocalized, GET_POKEMON);

	log_info(logger,"Me suscribi a la cola LOCALIZED!");

	while(1){

	}
}

void solicitar_pokemones(t_list *objetivoGlobal){

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

