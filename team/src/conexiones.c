#include "conexiones.h"

t_list *crearEntrenadores(){
	t_list *posiciones = config ->posiciones_entrenadores;
	t_list *pokemonesCapturados = config ->pokemon_entrenadores;
	t_list *pokemonesObjetivos = config ->objetivos_entrenadores;
	t_list *entrenadores = list_create();

	Posicion *base = malloc(sizeof(Posicion));
	base->x=0;
	base->y=0;

	for(int i=0; i< list_size(posiciones); i++){
		Entrenador *entrenador = malloc(sizeof(Entrenador));
		entrenador->entrenadorNumero = i + 1;
		if(list_get(pokemonesCapturados,i)== NULL)
			entrenador->pokemones_capturados= list_create();
		else
			entrenador->pokemones_capturados = list_get(pokemonesCapturados,i);
		entrenador->pokemones_objetivos= list_get(pokemonesObjetivos,i);
		entrenador->posicion=list_get(posiciones,i);
		entrenador->posicion_a_capturar= base;
		entrenador->idCatch=0;
		entrenador->puede_capturar = list_size(entrenador->pokemones_objetivos) - list_size(entrenador->pokemones_capturados);

		sem_init(&entrenador->mx_entrenador,0,0);
		pthread_t hiloEntrenador;
		pthread_create(&hiloEntrenador,NULL,(void *) realizar_tareas,entrenador);
		entrenador->hilo = hiloEntrenador;
		list_add(entrenadores,entrenador);
	}

	return entrenadores;
}

void setearVariablesGlobales(){
	ip= config->ip_broker;
	puerto= config->puerto_broker;

	ready= list_create();
	block= list_create();
	blockCaugth = list_create();
	blockDeadlock = list_create();

	sem_init(&semaforoExce,0,0);
	pthread_mutex_init(&mxExce,NULL);
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

	enviar_mensaje("Suscribime",conexionCaugth, SUS_CAUGHT);
	log_info(logger,"Me suscribi a la cola Caugth!");

	while(conexionCaugth != -1){
		process_request(conexionCaugth);

	}
}

void conexion_appeared(){

	int conexionAppeared = crear_conexion(ip,puerto);

	if(conexionAppeared ==-1){
		log_info(logger,"Reintenando reconectar cada %d segundos", config->reconexion);
		conexionAppeared= reintentar_conexion(ip,puerto,config->reconexion);
	}

	enviar_mensaje("Suscribime",conexionAppeared, SUS_APP);
	log_info(logger,"Me suscribi a la cola Appeared!");

	while(conexionAppeared != -1){
		process_request(conexionAppeared);
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
	Entrenador *entrenador;
	t_mensaje* mensaje = malloc(sizeof(t_mensaje));
	if(recv(socket_cliente, &cod_op, sizeof(op_code), MSG_WAITALL) == -1)
				cod_op = -1;

	switch (cod_op){
	case APPEARED_POKEMON:
		log_info(logger,"Llego pokemon a la cola APPEARED!");
		mensaje = recibir_mensaje_struct(socket_cliente);
		Poketeam pokemon; // = malloc(sizeof(Poketeam));
		pokemon.pos.x = mensaje->posx;
		pokemon.pos.y = mensaje->posy;
		pokemon.pokemon = mensaje->pokemon;

		if(necesitaPokemon(mensaje->pokemon, objetivoGlobal)){
			menorDistancia(pokemon);

		}else
			log_info(logger,"No se necesita!");
		free(mensaje);
		break;

	case CAUGHT_POKEMON:
		//if esSuPokemon
		mensaje = recibir_mensaje_struct(socket_cliente);
		entrenador= list_get(blockCaugth,0);
		list_remove(blockCaugth,0);
		list_add(ready,entrenador);
		sem_post(&semaforoExce);

		break;
	case -1:
		socket_cliente = crear_conexion(ip,puerto);
		break;

	}
}

void poner_en_exce(){

	while(1){
		sem_wait(&semaforoExce);
		log_info(logger,"Nuevo entrenador en ready!");
		pthread_mutex_lock(&mxExce);

		Entrenador *entrenador= list_get(ready,0);
		list_remove(ready,0);

		sem_post(&entrenador->mx_entrenador);
		log_info(logger,"Entrenador %d ejecutando", entrenador->entrenadorNumero);
	}

}

void realizar_tareas(Entrenador *entrenador){

	while(entrenador->puede_capturar > 0){
		sem_wait(&entrenador->mx_entrenador);
		int dis_x= abs(entrenador->posicion_a_capturar->x - entrenador->posicion->x);
		int dis_y= abs(entrenador->posicion_a_capturar->y - entrenador->posicion->y);
		int moverse = (dis_x + dis_y);//*config->retardo_cpu;

		log_info(logger,"Tiempo %d segundos:", moverse);

		sleep(moverse);
		entrenador->posicion->x = entrenador->posicion_a_capturar->x;
		entrenador->posicion->y = entrenador->posicion_a_capturar->y;
		//int socket_catch= crear_conexion(ip,puerto);
		//enviar_mensaje_struct(a,socket_catch,CATCH_POKEMON);
		log_info(logger,"LLegue a la posicion del pokemon!");
		list_add(blockCaugth,entrenador);

		pthread_mutex_unlock(&mxExce);

		sem_wait(&entrenador->mx_entrenador);

		list_add(entrenador->pokemones_capturados, entrenador->pokemon_a_caputar);
		log_info(logger,"Agarre pokemon!");
		entrenador->puede_capturar -= 1;

		pthread_mutex_unlock(&mxExce);

		if(entrenador->puede_capturar >0)
			list_add(block,entrenador);
	}

	if(puedeFinalizar(entrenador)){
		log_info(logger,"Finalizo entrenador %d", entrenador->entrenadorNumero);
		pthread_exit(entrenador->hilo);
	}
	else
		list_add(blockDeadlock,entrenador);


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
