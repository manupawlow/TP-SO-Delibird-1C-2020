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

	config_team -> ip_broker = config_get_string_value(config, "IP_BROKER");
	config_team -> puerto_broker = config_get_string_value(config, "PUERTO_BROKER");

	config_team -> objetivos_entrenadores = listaDeListas(config,"OBJETIVOS_ENTRENADORES");
	config_team -> pokemon_entrenadores = listaDeListas(config, "POKEMON_ENTRENADORES");
	config_team -> posiciones_entrenadores = listaPosiciones(config, "POSICIONES_ENTRENADORES");

	config_team -> retardo_cpu = config_get_int_value(config, "RETARDO_CICLO_CPU");
	config_team -> reconexion = config_get_int_value(config, "TIEMPO_RECONEXION");

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

void menorDistancia (Posicion posicion){

	Entrenador *entrenadorMasCerca = list_get(new,0);
	int indiceEntrenadorMasCerca = 0;

	for(int i=1; i<list_size(new);i++){
		Entrenador *entrenador= list_get(new,i);
		if( distancia(entrenador,posicion) < distancia(entrenadorMasCerca,posicion) ){
			entrenadorMasCerca=entrenador;
		    indiceEntrenadorMasCerca=i;
		}
	}

	entrenadorMasCerca = list_get(new,indiceEntrenadorMasCerca);
	entrenadorMasCerca->posicion_a_capturar->x = posicion.x;
	entrenadorMasCerca->posicion_a_capturar->y = posicion.y;
	list_remove(new,indiceEntrenadorMasCerca);
	list_add(ready,entrenadorMasCerca);

	sem_post(&semaforoExce);
	
}

float distancia(Entrenador *entrenador, Posicion posicion){
	float x= (entrenador->posicion->x) - (posicion.x);
    float y= (entrenador->posicion->y) - (posicion.y);
    return sqrt(x*x+y*y);
}
