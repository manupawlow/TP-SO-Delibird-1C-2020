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
	config_team->log = config_get_string_value(config, "LOG_FILE");

	return config_team;
}

void setearVariablesGlobales(){
	ip= config->ip_broker;
	puerto= config->puerto_broker;

	ready= list_create();
	block= list_create();

	sem_init(&semaforoExce,0,0);
	sem_init(&semaforoIntercambio,0,0);
	sem_init(&semaforoDeadlock,0,0);
	pthread_mutex_init(&mxExce,NULL);
}

t_list *obtenerObjetivoGlobal(){

	 t_list *pokemonesEntrenadores = config->pokemon_entrenadores;
	 t_list *objetivosEntrenadores = config->objetivos_entrenadores;

	 t_list *objetivoUnificado = listaUnica(objetivosEntrenadores);
     t_list *pokemonesUnificado = listaUnica(pokemonesEntrenadores);
	 t_list *objetivoGlobal = filtrado(pokemonesUnificado,objetivoUnificado);

	 return objetivoGlobal;
}

void solicitar_pokemones(t_list *objetivoGlobal){

	for(int i=0; i< list_size(objetivoGlobal); i++){
		int conexionGet = crear_conexion(ip,puerto);
		if(conexionGet == -1){
			log_info(logger,"No se pudo solicitar pokemon");
		}else{
			char *pokemon = list_get(objetivoGlobal,i);
			enviar_mensaje(pokemon,conexionGet,GET_POKEMON);
			int cod_op;
			t_mensaje* mensaje = malloc(sizeof(t_mensaje));
			if(recv(conexionGet, &cod_op, sizeof(op_code), MSG_WAITALL) == -1)
			cod_op = -1;
			mensaje = recibir_mensaje_struct(conexionGet);
			list_add(id_localized, mensaje->id_mensaje);
			close(conexionGet);
		}
	}
}

void a_lista_segun_objetivo(t_list *capturados, Entrenador *entrenador){
	t_list *objetivo = entrenador->pokemones_objetivos;

	for(int i=0; i<list_size(capturados); i++){
		char *pokemon= list_get(capturados,i);
		bool intercambio = true;

		for(int j=0; j<list_size(objetivo); j++){
			char *pokemon_objetivo= list_get(objetivo,j);

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

void agregar_segun_objetivo(char* pokemon, Entrenador *entrenador){
	t_list *objetivo = entrenador->pokemones_objetivos;
	bool intercambio= true;

	for(int i=0; i<list_size(objetivo); i++){
		char *pokemon_objetivo= list_get(objetivo,i);

		if((strcmp(pokemon,pokemon_objetivo)) == 0){
			list_add(entrenador->pokemones_capturados, pokemon);
			intercambio= false;
			for(int j=0; j<list_size(entrenador->pokemones_faltantes);j++){
				char *eleminar = list_get(entrenador->pokemones_faltantes,j);
				if((strcmp(pokemon,pokemon_objetivo)) == 0)
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
	int sizeListaDeListas= list_size(listaDeListas);
	t_list *listaUnica = list_create();
	for (int i =0; i< sizeListaDeListas; i++){
		t_list *lista = list_get(listaDeListas,i);
		int size = list_size(lista);
		for(int j=0; j< size ; j++){
			char *pokemon= list_get(lista,j);
			list_add(listaUnica, pokemon);
		}
	}

	return listaUnica;
}

t_list* filtrado(t_list* capturados, t_list* objetivos){

	t_list *aux = objetivos;
	t_list *auxCapturados= capturados;

	for(int i=0; i<list_size(objetivos); i++){
		char *objetivo= list_get(objetivos,i);
		if(auxCapturados==NULL)
			return aux;
		for(int j=0; j<list_size(auxCapturados); j++){
			char *capturado= list_get(auxCapturados,j);
			if(strcmp(objetivo,capturado)==0){
				list_remove(auxCapturados,j);
				list_remove(aux,i);
				i--;
				break;
			}
		}
	}
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

	finalizar = filtrado(entrenador->pokemones_capturados, entrenador->pokemones_objetivos);

	return list_is_empty(finalizar);
}

void ponerEnReady(Entrenador *entrenador, Poketeam pokemon, t_list *lista){
	entrenador->posicion_a_capturar->x = pokemon.pos.x;
    entrenador->posicion_a_capturar->y = pokemon.pos.y;
    entrenador->pokemon_a_caputar = pokemon.pokemon;
    entrenador->block_agarrar= false;
    remover_entrenador(entrenador->entrenadorNumero, lista);
	list_add(ready,entrenador);
	sem_post(&semaforoExce);
}

void remover_entrenador(int entrenadorNumero, t_list *lista){

	bool remover(Entrenador *entrenador){
		return entrenador->entrenadorNumero == entrenadorNumero;
	}

	list_remove_by_condition(lista, remover);
}

bool bloqueado_por_agarrar(Entrenador *entrenador){
	return entrenador->block_agarrar;
}
/*
bool bloqueado_por_capturar(Entrenador *entrenador){
	return entrenador->block_capturar;
}

bool bloqueado_por_deadlock(Entrenador *entrenador){
	return entrenador->block_deadlock;
}
*/
void menorDistancia (Poketeam pokemon){

	int indiceNew;
	int indiceblockAgarrar;
	Entrenador *entrenadorMasCercaNew;
	Entrenador *entrenadorMasCercaBlock;
	Posicion posicion = pokemon.pos;

	t_list *blockAgarrar = list_filter(block,bloqueado_por_agarrar);

	if(list_is_empty(new) && list_is_empty(blockAgarrar)){
			log_info(logger,"No hay entrenador disponible");
		}

	else if(!list_is_empty(new) && !list_is_empty(blockAgarrar)){
		indiceNew = indiceMasCercano(posicion,new);
		entrenadorMasCercaNew = list_get(new,indiceNew);

		indiceblockAgarrar = indiceMasCercano(posicion,blockAgarrar);
		entrenadorMasCercaBlock = list_get(blockAgarrar,indiceblockAgarrar);

		if(distancia(entrenadorMasCercaBlock,posicion) < distancia(entrenadorMasCercaNew,posicion)){
			ponerEnReady(entrenadorMasCercaBlock,pokemon,block);
		}else{
			ponerEnReady(entrenadorMasCercaNew,pokemon,new);
		}
	}

	else if(list_is_empty(new) && !list_is_empty(blockAgarrar)){
		indiceblockAgarrar = indiceMasCercano(posicion,blockAgarrar);
		entrenadorMasCercaBlock = list_get(blockAgarrar,indiceblockAgarrar);
		ponerEnReady(entrenadorMasCercaBlock,pokemon,block);
	}

	else if(!list_is_empty(new) && list_is_empty(blockAgarrar)){
		indiceNew = indiceMasCercano(posicion,new);
		entrenadorMasCercaNew = list_get(new,indiceNew);
		ponerEnReady(entrenadorMasCercaNew,pokemon,new);
	}

}

int distancia(Posicion *posicion1, Posicion posicion2){
	int x= abs(posicion2.x - posicion1->x);
    int y= abs(posicion2.y - posicion1->y);
    return x+y;
}

void moverse(Entrenador *entrenador){
	entrenador->posicion->x = entrenador->posicion_a_capturar->x;
	entrenador->posicion->y = entrenador->posicion_a_capturar->y;
}
