#include "funciones_auxiliares.h"

Config_Team* construirConfigTeam(t_config* config){

	Config_Team* config_team = malloc(sizeof(Config_Team));

	config_team->ip_broker = config_get_string_value(config, "IP_BROKER");
	config_team->puerto_broker = config_get_string_value(config, "PUERTO_BROKER");

	config_team->objetivos_entrenadores = listaDeListas(config,"OBJETIVOS_ENTRENADORES");
	config_team->pokemon_entrenadores = listaDeListas(config, "POKEMON_ENTRENADORES");
	config_team->posiciones_entrenadores = listaPosiciones(config, "POSICIONES_ENTRENADORES");

	config_team->retardo_cpu = config_get_int_value(config, "RETARDO_CICLO_CPU");
	config_team->reconexion = config_get_int_value(config, "TIEMPO_RECONEXION");
	config_team->quantum = config_get_int_value(config, "QUANTUM");
	config_team->log = config_get_string_value(config, "LOG_FILE");

	return config_team;
}

void free_pokemon(char* pokemon){
	free(pokemon);
}

void free_entrenador(Entrenador *entrenador){
	free(entrenador->pokemon_a_caputar);
	free(entrenador->posicion);
	free(entrenador->posicion_a_capturar);
	list_destroy_and_destroy_elements(entrenador->pokemones_a_intercambiar, (void*) free_pokemon);
	list_destroy_and_destroy_elements(entrenador->pokemones_capturados, (void*) free_pokemon);
	list_destroy_and_destroy_elements(entrenador->pokemones_faltantes, (void*) free_pokemon);
	list_destroy_and_destroy_elements(entrenador->pokemones_objetivos, (void*) free_pokemon);
	free(entrenador);
}

void setearVariablesGlobales(){
	ip= config->ip_broker;
	puerto= config->puerto_broker;
	cantEntrenadores = list_size(new);
	ciclos_totales_cpu = 0;
	deadlocks = 0;
	cambios_contexto = 0;

	id_localized = list_create();

	ready= list_create();
	block= list_create();

	pokemones_en_busqueda = list_create();
	pokemones_pendientes = list_create();

	ciclos_por_entrenador = list_create();
	int ciclos= 0;
	for(int i=0; i<list_size(new); i++){
		list_add(ciclos_por_entrenador, (void*) ciclos);
	}

	sem_init(&semaforoExce,0,0);
	sem_init(&semaforoIntercambio,0,0);
	sem_init(&semaforoDeadlock,0,0);
	pthread_mutex_init(&mxExce,NULL);
	pthread_mutex_init(&mx_llegada_pokemon,NULL);
	pthread_mutex_init(&mx_llegada_localized,NULL);
}

t_list *obtenerObjetivoGlobal(){

	 t_list *pokemonesEntrenadores = config->pokemon_entrenadores;
	 t_list *objetivosEntrenadores = config->objetivos_entrenadores;

	 t_list *objetivoUnificado = listaUnica(objetivosEntrenadores);
     t_list *pokemonesUnificado = listaUnica(pokemonesEntrenadores);
	 t_list *objetivoGlobal = filtrado(pokemonesUnificado,objetivoUnificado);

	 list_destroy_and_destroy_elements(pokemonesUnificado, (void*) free_pokemon);
	 list_destroy(objetivoUnificado);

	 return objetivoGlobal;
}


void solicitar_pokemones(t_list *objetivoGlobal){

	t_buffer *buffer;
    t_mensaje* mensaje = malloc(sizeof(t_mensaje));

    t_list *duplicada = list_duplicate(objetivoGlobal);

    for(int i=0; i< list_size(duplicada); i++){
    	char *pokemon = list_get(duplicada,i);

    	for(int j=0; j< list_size(duplicada); j++){
    		char *comparar = list_get(duplicada,j);

    		if(i==j){

    		}else{
    			if(strcmp(comparar,pokemon)==0)
    				list_remove(duplicada,j);
    		}


    	}
    }

	for(int i=0; i< list_size(duplicada); i++){
		int conexionGet = crear_conexion(ip,puerto);
		char *pokemon = list_get(duplicada,i);

		if(conexionGet == -1){
			log_info(logger,"<MENSAJE> Fallo conexion broker, no se pudo enviar mensaje Get %s", pokemon);
		}else{

			mensaje->pokemon = malloc(strlen(pokemon)+1);
			memcpy(mensaje->pokemon,pokemon,strlen(pokemon)+1);
			mensaje->pokemon_length = strlen(pokemon)+1;
			mensaje->resultado = 0;
			mensaje->posx = 0;
			mensaje->posy = 0;
			mensaje->cantidad = 0;
			mensaje->id_mensaje = 0;
			mensaje->id_mensaje_correlativo = 0;

			log_info(logger,"<MENSAJE> Se envio mensaje Get %s", mensaje->pokemon);
			buffer = serializar_mensaje_struct(mensaje);
			enviar_mensaje_struct(buffer,conexionGet,GET_POKEMON);
			free(buffer->stream);
			free(buffer);

			int cod_op;
			recv(conexionGet, &cod_op, sizeof(op_code), MSG_WAITALL);

			mensaje = recibir_mensaje_struct(conexionGet);
			list_add(id_localized,(void *) mensaje->id_mensaje);
			log_info(logger,"<MENSAJE> Id %d mensaje Get %s ", mensaje->id_mensaje, mensaje->pokemon);

			close(conexionGet);

			free(mensaje->pokemon);
		}
	}
	free(mensaje);
}

void agregar_segun_objetivo(t_list *capturados, Entrenador *entrenador){
	t_list *objetivo = entrenador->pokemones_objetivos;

	for(int i=0; i<list_size(capturados); i++){
		char *pokemon = malloc(strlen((char*)list_get(capturados,i))+1);
		memcpy(pokemon,list_get(capturados,i),strlen((char*)list_get(capturados,i))+1);
		bool intercambio = true;

		for(int j=0; j<list_size(objetivo); j++){
			char *pokemon_objetivo = list_get(objetivo,j);

			if((strcmp(pokemon,pokemon_objetivo)) == 0){
				list_add(entrenador->pokemones_capturados, pokemon);
				intercambio= false;
				break;
			}
		}

		if(intercambio)
			list_add(entrenador->pokemones_a_intercambiar, pokemon);

	}

}

void agregar_segun_faltantes(char* pokemon, Entrenador *entrenador){
	t_list *faltantes = entrenador->pokemones_faltantes;
	bool intercambio= true;

	for(int i=0; i<list_size(faltantes); i++){
		char *pokemon_objetivo = list_get(faltantes,i);

		if((strcmp(pokemon,pokemon_objetivo)) == 0){
			list_add(entrenador->pokemones_capturados, pokemon);
			intercambio = false;
			for(int j=0; j<list_size(entrenador->pokemones_faltantes);j++){
				char *eleminar = list_get(entrenador->pokemones_faltantes,j);
				if((strcmp(pokemon,eleminar)) == 0)
					list_remove(entrenador->pokemones_faltantes,j);
			}

			break;
			}
	}

	if(intercambio)
		list_add(entrenador->pokemones_a_intercambiar, pokemon);

}


void recorrerLista(t_list *lista, t_log *logger){
	for (int i =0; i< list_size(lista); i++){
			char *pokemon= list_get(lista,i);
			log_info(logger,"%s",pokemon);
		}
}

t_list *listaUnica(t_list *listaDeListas){
	int sizeListaDeListas = list_size(listaDeListas);
	t_list *listaUnica = list_create();
	for (int i =0; i< sizeListaDeListas; i++){
		t_list *lista = list_get(listaDeListas,i);
		int size = list_size(lista);
		for(int j=0; j< size ; j++){
			char *pokemon = malloc(strlen((char*)list_get(lista,j))+1);
			memcpy(pokemon,list_get(lista,j),strlen((char*)list_get(lista,j))+1);
			list_add(listaUnica, pokemon);
		}
	}

	return listaUnica;
}

t_list* filtrado(t_list* capturados, t_list* objetivos){

	t_list *aux = list_duplicate(objetivos);
	t_list *auxCapturados;
	if(capturados==NULL)
		return aux;
	else
		auxCapturados = list_duplicate(capturados);

	for(int i=0; i<list_size(aux); i++){
		char *objetivo = list_get(aux,i);

		for(int j=0; j<list_size(auxCapturados); j++){
			char *capturado = list_get(auxCapturados,j);
			if(strcmp(objetivo,capturado)==0){
				list_remove(auxCapturados,j);
				list_remove(aux,i);
				i--;
				break;
			}
		}
	}

	list_destroy_and_destroy_elements(auxCapturados, (void*) free_pokemon);

	return aux;

}

t_list* listaDeListas(t_config* config, char* cadena) {
  char** array = config_get_array_value(config, cadena);

  t_list* lista = list_create();
  t_list* sublista;

  void agregar(char *pokemon) {
    if (pokemon != NULL) {
      list_add(sublista, pokemon);
    }
  }

  void dividir(char *string) {
	sublista = list_create();
    if(string != NULL) {
      char** pokemon = string_split(string, "|");
      string_iterate_lines(pokemon, agregar);
      free(pokemon);
    } else exit(1);
    list_add(lista,sublista);
  }
  string_iterate_lines(array, dividir);

  string_iterate_lines(array, (void*) free);

  free(array);
  return lista;
}

t_list* listaPosiciones(t_config* config, char* cadena) {
  char** array = config_get_array_value(config, cadena);

  t_list* posiciones = list_create();

  void dividir(char *string) {
	  Posicion *posicion = malloc(sizeof(Posicion));
	  if(string != NULL) {
      char** punto = string_split(string, "|");
      posicion->x = atoi(punto[0]);
      posicion->y = atoi(punto[1]);
      string_iterate_lines(punto, (void*) free);
      free(punto);
    } else exit(1);
    list_add(posiciones,posicion);
  }

  string_iterate_lines(array, dividir);

  string_iterate_lines(array, (void*) free);

  free(array);

  return posiciones;
}

int indiceMasCercano(Posicion posicion, t_list *lista){
	Entrenador *entrenadorMasCerca = list_get(lista,0);
	int indiceEntrenadorMasCerca = 0;

		for(int i=1; i<list_size(lista);i++){
			Entrenador *entrenador= list_get(lista,i);
			if( distancia(entrenador->posicion,posicion) < distancia(entrenadorMasCerca->posicion,posicion) ){
				entrenadorMasCerca=entrenador;
			    indiceEntrenadorMasCerca=i;
			}
		}
	return indiceEntrenadorMasCerca;
}

bool puedeFinalizar(Entrenador *entrenador){
	t_list *finalizar;
	int booleano;//agregado

	finalizar = filtrado(entrenador->pokemones_capturados, entrenador->pokemones_objetivos);

	booleano = list_is_empty(finalizar);//agregado

	list_destroy(finalizar);//agregado

	return booleano;//agregado
}

void ponerEnReady(Entrenador *entrenador, Poketeam *pokemon){

	entrenador->pokemon_a_caputar = malloc(strlen(pokemon->pokemon)+1);
	memcpy(entrenador->pokemon_a_caputar,pokemon->pokemon,strlen(pokemon->pokemon)+1);

	//agregado
	entrenador->posicion_a_capturar->x = pokemon->pos.x;
    entrenador->posicion_a_capturar->y = pokemon->pos.y;

    entrenador->block_agarrar= false;
	list_add(ready,entrenador);
	log_info(logger,"Entrenador %d en ready por ser el mas cercano a %s x:%d y:%d",
			entrenador->entrenadorNumero, entrenador->pokemon_a_caputar, entrenador->posicion_a_capturar->x, entrenador->posicion->y);

	sem_post(&semaforoExce);
}

void remover_entrenador(int entrenadorNumero, t_list *lista){

	bool remover(Entrenador *entrenador){
		return entrenador->entrenadorNumero == entrenadorNumero;
	}
	/*void free_especialito(Entrenador* entrenador){
		free(entrenador->pokemon_a_caputar);
		free(entrenador->posicion_a_capturar);
		free(entrenador);
	}*/

	list_remove_by_condition(lista, (void*) remover);
}

Entrenador* id_coincidente(int id, t_list *lista){

	bool coincide(Entrenador *entrenador){
		return entrenador->idCatch == id;
	}

	return list_find(lista, (void*)coincide);
}

bool pokemon_en_lista(char *pokemon, t_list *lista){

	bool coincide(char *busqueda){
			return (strcmp(pokemon,busqueda) ==0);
		}

		return list_any_satisfy(lista,(void*)coincide);
}

void eliminar_pokemon(char *pokemon, t_list *lista){

	bool coincide(char* busqueda){
		return (strcmp(busqueda,pokemon) == 0);
	}

	//list_remove_and_destroy_by_condition(lista, (void*) coincide, (void*) free_pokemon);//agregado
	list_remove_by_condition(lista, (void*) coincide);
}

void eliminar_pendientes(char *pokemon){

	bool coincide(Poketeam *poketeam){
		return (strcmp(pokemon, poketeam->pokemon)==0);
	}


	if(!pokemon_en_lista(pokemon,objetivoGlobal))
		//list_remove_by_condition(pokemones_pendientes, (void*) coincide);
		list_remove_and_destroy_by_condition(pokemones_pendientes, (void*) coincide, (void*) free_poketeam);//agregado

}

bool id_en_lista(int id_mensaje){

	bool coincide(int id){
		return id == id_mensaje;
	}

	return list_any_satisfy(id_localized,(void*) coincide);
}

bool bloqueado_por_agarrar(Entrenador *entrenador){
	return entrenador->block_agarrar;
}

bool bloqueado_por_capturar(Entrenador *entrenador){
	return entrenador->block_capturar;
}

bool bloqueado_por_deadlock(Entrenador *entrenador){
	return entrenador->block_deadlock;
}

bool necesitaPokemon(char *pokemon, t_list *objetivoGlobal){

	for(int i=0; i< list_size(objetivoGlobal); i++){
		char *objetivo = list_get(objetivoGlobal,i);
		if(!strcmp(pokemon,objetivo)){
			return true;
		}
	}

	return false;
}

void menorDistancia (Poketeam *pokemon){

	int indiceNew;
	int indiceblockAgarrar;
	Entrenador *entrenadorMasCercaNew;
	Entrenador *entrenadorMasCercaBlock;
	Posicion posicion = pokemon->pos;

	t_list *blockAgarrar = list_filter(block,(void*)bloqueado_por_agarrar);

	if(list_is_empty(new) && list_is_empty(blockAgarrar)){
		Poketeam *pendiente = malloc(sizeof(Poketeam));
		pendiente->pokemon = malloc(strlen(pokemon->pokemon)+1);
		pendiente->pos = pokemon->pos;
		memcpy(pendiente->pokemon, pokemon->pokemon, strlen(pokemon->pokemon)+1);
		list_add(pokemones_pendientes,pendiente);
		log_info(logger,"Se agrego pokemon %s a la lista de pokemones pendientes", pokemon->pokemon);

		}
    else if(!list_is_empty(new) && !list_is_empty(blockAgarrar)){
		indiceNew = indiceMasCercano(posicion,new);
		entrenadorMasCercaNew = list_get(new,indiceNew);

		indiceblockAgarrar = indiceMasCercano(posicion,blockAgarrar);
		entrenadorMasCercaBlock = list_get(blockAgarrar,indiceblockAgarrar);

		if(distancia(entrenadorMasCercaBlock->posicion,posicion) < distancia(entrenadorMasCercaNew->posicion,posicion)){
			ponerEnReady(entrenadorMasCercaBlock,pokemon);
			remover_entrenador(entrenadorMasCercaBlock->entrenadorNumero, block);
		}else{
			ponerEnReady(entrenadorMasCercaNew,pokemon);
			remover_entrenador(entrenadorMasCercaNew->entrenadorNumero, new);
		}
	}

	else if(list_is_empty(new) && !list_is_empty(blockAgarrar)){
		indiceblockAgarrar = indiceMasCercano(posicion,blockAgarrar);
		entrenadorMasCercaBlock = list_get(blockAgarrar,indiceblockAgarrar);
		ponerEnReady(entrenadorMasCercaBlock,pokemon);
		remover_entrenador(entrenadorMasCercaBlock->entrenadorNumero, block);
	}

	else if(!list_is_empty(new) && list_is_empty(blockAgarrar)){
		indiceNew = indiceMasCercano(posicion,new);
		entrenadorMasCercaNew = list_get(new,indiceNew);
		ponerEnReady(entrenadorMasCercaNew,pokemon);
		remover_entrenador(entrenadorMasCercaNew->entrenadorNumero, new);
	}
	list_destroy(blockAgarrar);//agregado
}

int distancia(Posicion *posicion1, Posicion posicion2){
	int x= abs(posicion2.x - posicion1->x);
    int y= abs(posicion2.y - posicion1->y);
    return x+y;
}

void moverse(Entrenador *entrenador){
	//Posicion* posicion = malloc(sizeof(Posicion));
	//memcpy(posicion, entrenador->posicion_a_capturar, sizeof(Posicion));
	//free(entrenador->posicion);
	entrenador->posicion->x = entrenador->posicion_a_capturar->x;
	entrenador->posicion->y = entrenador->posicion_a_capturar->y;
}


void enviar_catch(Entrenador *entrenador, int conexion_catch){
	t_buffer *buffer;
	t_mensaje* mensaje = malloc(sizeof(t_mensaje));

	mensaje->pokemon = malloc(strlen(entrenador->pokemon_a_caputar)+1);
	memcpy(mensaje->pokemon,entrenador->pokemon_a_caputar,strlen(entrenador->pokemon_a_caputar)+1);
	mensaje->pokemon_length = strlen(entrenador->pokemon_a_caputar)+1;
	mensaje->resultado = 0;
	mensaje->posx = entrenador->posicion_a_capturar->x;
	mensaje->posy = entrenador->posicion_a_capturar->y;
	mensaje->cantidad = 0;
	mensaje->id_mensaje = 0;
	mensaje->id_mensaje_correlativo = 0;

	buffer = serializar_mensaje_struct(mensaje);
	enviar_mensaje_struct(buffer,conexion_catch,CATCH_POKEMON);
	free(buffer->stream);//agregado
	free(buffer);//agregado

	int cod_op;
	recv(conexion_catch, &cod_op, sizeof(op_code), MSG_WAITALL);
	mensaje = recibir_mensaje_struct(conexion_catch);
	entrenador->idCatch = mensaje->id_mensaje;

	log_info(logger,"<MENSAJE> Id mensaje catch %s:%d, entrenador:%d",mensaje->pokemon, mensaje->id_mensaje, entrenador->entrenadorNumero);
	free(mensaje->pokemon);//agregado
	free(mensaje);//agregado
	close(conexion_catch);
}

t_list* recibirLocalized(t_mensaje_get* mensaje){
    char** arrayPos = string_split(mensaje->posiciones,".");

    t_list* listaPoke = list_create();

    int i=0;
    while(arrayPos[i]!=NULL){

        char** posiciones= string_split(arrayPos[i],"-");

        Poketeam* pokemon = malloc(sizeof(Poketeam));
        pokemon->pokemon = malloc(mensaje->pokemon_length);
        memcpy(pokemon->pokemon,mensaje->pokemon,mensaje->pokemon_length);
        pokemon->pos.x= atoi(posiciones[0]);
        pokemon->pos.y= atoi(posiciones[1]);

        list_add(listaPoke, pokemon);

        freeDoblePuntero(posiciones);
        i++;
    }

    return listaPoke;

}
