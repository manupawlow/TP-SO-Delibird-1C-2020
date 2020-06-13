#include "conexiones.h"

t_list *crearEntrenadores(){
	t_list *posiciones = config ->posiciones_entrenadores;
	t_list *pokemones = config ->pokemon_entrenadores;
	t_list *entrenadores = list_create();

	Posicion *base = malloc(sizeof(Posicion));
	base->x=0;
	base->y=0;

	for(int i=0; i< list_size(posiciones); i++){
		Entrenador *entrenador = malloc(sizeof(Entrenador));
		entrenador->pokemones=list_get(pokemones,i);
		entrenador->posicion=list_get(posiciones,i);
		entrenador->posicion_a_capturar= base;
		sem_init(&entrenador->mx_entrenador,0,0);
		pthread_t hiloEntrenador;
		pthread_create(&hiloEntrenador,NULL,(void *) realizar_tareas,entrenador);
		list_add(entrenadores,entrenador);
	}

	return entrenadores;
}

void setearVariablesGlobales(){
	ip= config->ip_broker;
	puerto= config->puerto_broker;

	ready= list_create();


	sem_init(&semaforoExce,0,0);
	pthread_mutex_init(&mxExce,NULL);
}

void realizar_tareas(Entrenador *entrenador){
	sem_wait(&entrenador->mx_entrenador);

	int dis_x= abs(entrenador->posicion_a_capturar->x - entrenador->posicion->x);
	int dis_y= abs(entrenador->posicion_a_capturar->y - entrenador->posicion->y);
	int moverse = (dis_x + dis_y) ;//* config->retardo_cpu;

	log_info(logger,"Tiempo %d segundos:", moverse);

	sleep(moverse);
	entrenador->posicion->x = entrenador->posicion_a_capturar->x;
	entrenador->posicion->y = entrenador->posicion_a_capturar->y;

	//list_add(block,entrenador);
	log_info(logger,"LLegue a la posicion del pokemon!");

	pthread_mutex_unlock(&mxExce);

	sem_wait(&entrenador->mx_entrenador);


}

void poner_en_exce(){

	while(1){
		sem_wait(&semaforoExce);
		log_info(logger,"Nuevo entrenador en ready!");
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

void process_request(int socket_cliente){
	int cod_op;
	Entrenador *ent;
	t_mensaje* mensaje = malloc(sizeof(t_mensaje));
	if(recv(socket_cliente, &cod_op, sizeof(op_code), MSG_WAITALL) == -1)
				cod_op = -1;

	switch (cod_op){
	case APPEARED_POKEMON:
		log_info(logger,"Llego pokemon a la cola APPEARED!");
		mensaje = recibir_mensaje_struct(socket_cliente);
		Posicion pos;
		pos.x = mensaje->posx;
		pos.y = mensaje->posy;
		if(necesitaPokemon(mensaje->pokemon, objetivoGlobal)){
			menorDistancia(pos);

		}else
			log_info(logger,"No se necesita!");
		free(mensaje);
		break;
	/*
	case CAUGHT_POKEMON:
		//if esSuPokemon
        ent= list_get(block,0);
		list_remove(block,0);
		list_add(ent,ready);
		sem_post(&semaforoExce);

		break;
	*/
	case -1:
		log_info(logger,"Error");

	}
}

void conexion_appeared(Config_Team config_team){

	int conexionAppeared = crear_conexion(ip,puerto);

	if(conexionAppeared ==-1){
		log_info(logger,"Reintenando reconectar cada %d segundos", config->reconexion);
		conexionAppeared= reintentar_conexion(ip,puerto,config->reconexion);
	}

	enviar_mensaje("Suscribime",conexionAppeared, SUS_APP);
	log_info(logger,"Me suscribi a la cola Appeared!");

	while(1){
		char *mensaje = recibir_mensaje_cliente(conexionAppeared);
		process_request(conexionAppeared);
	}

}

void conexion_localized(){

	int conexionLocalized =	crear_conexion(ip,puerto);

	if(conexionLocalized ==-1){
		log_info(logger,"Reintenando reconectar cada %d segundos", config->reconexion);
        conexionLocalized= reintentar_conexion(ip,puerto,config->reconexion);
	}

	enviar_mensaje("Suscribime",conexionLocalized, SUS_LOC);
	log_info(logger,"Me suscribi a la cola Localized!");

	while(1){

	}
}

void conexion_caugth(){

	int conexionCaugth = crear_conexion(ip,puerto);

	if(conexionCaugth ==-1){
		log_info(logger,"Reintenando reconectar cada %d segundos", config->reconexion);
        conexionCaugth= reintentar_conexion(ip,puerto,config->reconexion);
	}

	enviar_mensaje("Suscribime",conexionCaugth, SUS_GET);
	log_info(logger,"Me suscribi a la cola Caugth!");

	while(1){
		//char *mensaje = recibir_mensaje_cliente(conexionCaugth);

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
