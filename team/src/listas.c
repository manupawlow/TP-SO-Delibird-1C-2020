#include "listas.h"

t_list *obtenerObjetivoGlobal(){

	 t_list *pokemonesEntrenadores = config->pokemon_entrenadores;
	 t_list *objetivosEntrenadores = config->objetivos_entrenadores;

	 t_list *objetivoUnificado = listaUnica(objetivosEntrenadores);
     t_list *pokemonesUnificado = listaUnica(pokemonesEntrenadores);
	 t_list *objetivoGlobal = filtrado(pokemonesUnificado,objetivoUnificado);

	 return objetivoGlobal;

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

	for(int i=0; i<list_size(objetivos); i++){
		char *objetivo= list_get(objetivos,i);
		if(capturados==NULL)
			return aux;
		for(int j=0; j<list_size(capturados); j++){
			char *capturado= list_get(capturados,j);
			if(strcmp(objetivo,capturado)==0){
				list_remove(capturados,j);
				list_remove(aux,i);
				i--;
				break;
			}
		}
	}
	return aux;

}

Config_Team* construirConfigTeam(t_config* config){

	Config_Team* config_team = malloc(sizeof(Config_Team));
	config_team->objetivos_entrenadores= malloc(sizeof(t_list));
	config_team->posiciones_entrenadores= malloc(sizeof(t_list));
	config_team->pokemon_entrenadores= malloc(sizeof(t_list));

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
			if( distancia(entrenador,posicion) < distancia(entrenadorMasCerca,posicion) ){
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

void ponerEnReady(Entrenador *entrenador, t_list *lista, int indice, Poketeam pokemon){
	entrenador->posicion_a_capturar->x = pokemon.pos.x;
    entrenador->posicion_a_capturar->y = pokemon.pos.y;
    entrenador->pokemon_a_caputar = pokemon.pokemon;
	list_remove(lista,indice);
	list_add(ready,entrenador);
	sem_post(&semaforoExce);
}

void menorDistancia (Poketeam pokemon){

	int indiceNew;
	int indiceBlock;
	Entrenador *entrenadorMasCercaNew;
	Entrenador *entrenadorMasCercaBlock;
	Posicion posicion = pokemon.pos;

	if(list_is_empty(new) && list_is_empty(block)){
			log_info(logger,"No hay entrenador disponible");
		}

	else if(!list_is_empty(new) && !list_is_empty(block)){
		indiceNew = indiceMasCercano(posicion,new);
		entrenadorMasCercaNew = list_get(new,indiceNew);
		indiceBlock = indiceMasCercano(posicion,block);
		entrenadorMasCercaBlock = list_get(block,indiceBlock);

		if(distancia(entrenadorMasCercaBlock,posicion) < distancia(entrenadorMasCercaNew,posicion)){
			ponerEnReady(entrenadorMasCercaBlock,block,indiceBlock,pokemon);
		}else{
			ponerEnReady(entrenadorMasCercaNew,new,indiceNew,pokemon);
		}
	}

	else if(list_is_empty(new) && !list_is_empty(block)){
		indiceBlock = indiceMasCercano(posicion,block);
		entrenadorMasCercaBlock = list_get(block,indiceBlock);
		ponerEnReady(entrenadorMasCercaBlock,block,indiceBlock,pokemon);
	}

	else if(!list_is_empty(new) && list_is_empty(block)){
		indiceNew = indiceMasCercano(posicion,new);
		entrenadorMasCercaNew = list_get(new,indiceNew);
		ponerEnReady(entrenadorMasCercaNew,new,indiceNew,pokemon);
	}

}

float distancia(Entrenador *entrenador, Posicion posicion){
	float x= (entrenador->posicion->x) - (posicion.x);
    float y= (entrenador->posicion->y) - (posicion.y);
    return sqrt(x*x+y*y);
}
