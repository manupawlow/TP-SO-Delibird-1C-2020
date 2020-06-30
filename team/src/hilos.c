#include "hilos.h"

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
		entrenador->pokemones_objetivos= list_get(pokemonesObjetivos,i);

		entrenador->pokemones_capturados= list_create();
		entrenador->pokemones_a_intercambiar = list_create();
		if(list_get(pokemonesCapturados,i)!= NULL){
			agregar_segun_objetivo( list_get(pokemonesCapturados,i), entrenador);
		}

		entrenador->pokemones_faltantes= filtrado(list_get(pokemonesCapturados,i),list_get(pokemonesObjetivos,i));
		entrenador->posicion=list_get(posiciones,i);
		entrenador->posicion_a_capturar= base;
		entrenador->idCatch=0;
		entrenador->puede_capturar = list_size(entrenador->pokemones_objetivos) - list_size(entrenador->pokemones_capturados) - list_size(entrenador->pokemones_a_intercambiar);

		entrenador->block_agarrar = false;
		entrenador->block_capturar = false;
		entrenador->block_deadlock = false;

		sem_init(&entrenador->sem_entrenador,0,0);
		pthread_t hiloEntrenador;
		pthread_create(&hiloEntrenador,NULL,(void *) realizar_tareas,entrenador);
		list_add(entrenadores,entrenador);
	}

	return entrenadores;
}

void conexion_localized(){

	int conexionLocalized =	crear_conexion_broker(ip,puerto,logger, config->reconexion, SUS_LOC);

	log_info(logger,"Me suscribi a la cola Localized!");

	while(1){
		process_request(conexionLocalized);

	}
}

void conexion_caugth(){

	int conexionCaugth = crear_conexion_broker(ip,puerto,logger, config->reconexion, SUS_CAUGHT);

	log_info(logger,"Me suscribi a la cola Caugth!");

	while(conexionCaugth != -1){
		process_request(conexionCaugth);

	}
}

void conexion_appeared(){

	int conexionAppeared = crear_conexion_broker(ip,puerto,logger, config->reconexion, SUS_APP);

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
		mensaje = recibir_mensaje_struct(socket_cliente);
		Poketeam *pokemon = malloc(sizeof(Poketeam));
		pokemon->pokemon = malloc(mensaje->pokemon_length);
		pokemon->pos.x = mensaje->posx;
		pokemon->pos.y = mensaje->posy;
		memcpy(pokemon->pokemon,mensaje->pokemon,mensaje->pokemon_length);

		log_info(logger,"Llego pokemon %s con posicion x:%d y:%d a la cola Appeared!",
				pokemon->pokemon,pokemon->pos.x,pokemon->pos.y);

		if(necesitaPokemon(mensaje->pokemon, objetivoGlobal)){
			menorDistancia(pokemon);

		}else
			log_info(logger,"No se necesita!");

		free(pokemon->pokemon);
		free(pokemon);
		free(mensaje);
		break;

	case LOCALIZED_POKEMON:
		mensaje = recibir_mensaje_struct(socket_cliente);

		//if(id_correspondiente(mensaje->id_mensaje))
			//menorDistancia(pokemon);

		break;
	case CAUGHT_POKEMON:
		//if esSuPokemon
		mensaje = recibir_mensaje_struct(socket_cliente);
		t_list *blockCaugth = list_filter(block, (void*) bloqueado_por_capturar);
		entrenador= list_get(blockCaugth,0);
		entrenador->block_capturar= false;
		remover_entrenador(entrenador->entrenadorNumero,block);
		list_add(ready,entrenador);
		sem_post(&semaforoExce);
		free(mensaje);
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

		sem_post(&entrenador->sem_entrenador);
		log_info(logger,"Entrenador %d ejecutando", entrenador->entrenadorNumero);
	}

}

void realizar_tareas(Entrenador *entrenador){

	while(entrenador->puede_capturar > 0){
		sem_wait(&entrenador->sem_entrenador);

		Posicion pos;
		pos.x= entrenador->posicion->x;
		pos.y= entrenador->posicion->y;

		int dis = distancia(entrenador->posicion_a_capturar, pos );//*config->retardo_cpu;
		log_info(logger,"Entrenador %d moviendose a x:%d y:%d, tiempo requerido %d segundos",
				entrenador->entrenadorNumero, entrenador->posicion_a_capturar->x, entrenador->posicion_a_capturar->y, dis);

		sleep(dis);
		moverse(entrenador);

		entrenador->block_capturar= true;
		list_add(block,entrenador);
		log_info(logger,"Entrenador %d en block a la espera de caugth",entrenador->entrenadorNumero);

		pthread_mutex_unlock(&mxExce);

		sem_wait(&entrenador->sem_entrenador);

		agregar_segun_faltantes(entrenador->pokemon_a_caputar, entrenador);

		log_info(logger,"Entrenador %d agarro pokemon %s en posicion x:%d y:%d",
				entrenador->entrenadorNumero, entrenador->pokemon_a_caputar ,entrenador->posicion_a_capturar->x, entrenador->posicion_a_capturar->y, dis);
		log_info(logger,"Entrenador %d intercambia pokemon %s",
				entrenador->entrenadorNumero, list_get(entrenador->pokemones_a_intercambiar,0));
		entrenador->puede_capturar -= 1;

		pthread_mutex_unlock(&mxExce);

		if(entrenador->puede_capturar >0){
			list_add(block,entrenador);
			entrenador->block_agarrar= true;
		}
	}

	if(puedeFinalizar(entrenador)){
		log_info(logger,"Finalizo entrenador %d", entrenador->entrenadorNumero);
		sem_post(&semaforoDeadlock);
	}
	else{
		list_add(block,entrenador);
		entrenador->block_deadlock = true;
		sem_post(&semaforoDeadlock);
	}

	while(!puedeFinalizar(entrenador)){

		sem_wait(&entrenador->sem_entrenador);

		int dis_x= abs(entrenador->posicion_a_capturar->x - entrenador->posicion->x);
		int dis_y= abs(entrenador->posicion_a_capturar->y - entrenador->posicion->y);
		int moverse = (dis_x + dis_y);//*config->retardo_cpu;

		log_info(logger,"Tiempo %d segundos:", moverse);

		sleep(moverse);

		agregar_segun_faltantes(entrenador->pokemon_a_caputar, entrenador);

		sem_post(&semaforoIntercambio);
		log_info(logger,"Se realizo el intercambio!");

		if(!puedeFinalizar(entrenador)){
			list_add(block,entrenador);
			entrenador->block_deadlock=true;
		}

		pthread_mutex_unlock(&mxExce);
	}

	//free_entrenador(entrenador);
	log_info(logger,"Finalizo entrenador %d", entrenador->entrenadorNumero);

}

void deadlock(){

	for(int i=0; i<cantEntrenadores; i++)
		sem_wait(&semaforoDeadlock);

	log_info(logger, "Inicio algoritmo deteccion de deadlock");
	t_list *blockDeadlock= list_filter(block, (void*) bloqueado_por_deadlock);
	bool intercambio= false;

	while(list_size(blockDeadlock)>1){
		Entrenador *entrenador= list_get(blockDeadlock,0);
		Entrenador *entrenador_intercambio;
		intercambio= false;
		char *pokemon = list_get(entrenador->pokemones_faltantes,0);
		char *mierda = list_get(entrenador->pokemones_a_intercambiar,0);
		log_info(logger,"%s", mierda);

		for(int i=1; i<list_size(blockDeadlock); i++){
			entrenador_intercambio= list_get(blockDeadlock,i);

			for(int j=0; j<list_size(entrenador_intercambio->pokemones_a_intercambiar); j++){
				char *pokemon_intercambio = list_get(entrenador_intercambio->pokemones_a_intercambiar,j);

				if(strcmp(pokemon,pokemon_intercambio)==0){

					log_info(logger, "Deadlock entre entrenador %d y entrenador %d",
							entrenador->entrenadorNumero, entrenador_intercambio->entrenadorNumero);

					log_info(logger, "%d", entrenador_intercambio->posicion->x );
					log_info(logger, "%d", entrenador_intercambio->posicion->y );
					log_info(logger, "%d", entrenador->posicion->x );
					log_info(logger, "%d", entrenador->posicion->y );

					entrenador->pokemon_a_caputar = pokemon_intercambio;//realloc(entrenador->pokemon_a_caputar, strlen(pokemon_intercambio)+1);
					//memcpy(entrenador->pokemon_a_caputar, pokemon_intercambio ,strlen(pokemon_intercambio)+1);

				    //memcpy(entrenador->posicion_a_capturar, entrenador_intercambio->posicion, sizeof(Posicion));
					entrenador->posicion_a_capturar->x= entrenador_intercambio->posicion->x;
					entrenador->posicion_a_capturar->y= entrenador_intercambio->posicion->y;

					list_remove(entrenador_intercambio->pokemones_a_intercambiar,j);

					agregar_segun_faltantes(mierda,entrenador_intercambio);
					intercambio=true;

					break;
				}

			if(intercambio)
			break;
			}

		}

		if(intercambio){
			remover_entrenador(entrenador->entrenadorNumero,block);
			entrenador->block_deadlock=false;
			list_add(ready,entrenador);
			sem_post(&semaforoExce);
			finalizar_si_corresponde(entrenador_intercambio);
			blockDeadlock= list_filter(block, (void*) bloqueado_por_deadlock);
		}

	}

   log_info(logger, "Finalizacion algoritmo deteccion de deadlock");


}

void finalizar_si_corresponde(Entrenador *entrenador){
	sem_wait(&semaforoIntercambio);
	if(puedeFinalizar(entrenador)){
		remover_entrenador(entrenador->entrenadorNumero,block);
		log_info(logger,"Finalizo entrenador %d", entrenador->entrenadorNumero);
	}

}
