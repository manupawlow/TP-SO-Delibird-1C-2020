#include "/home/utnso/tp-2020-1c-NN/biblioteca/biblioteca.c"
#include <commons/log.h>
#include "team.h"

int main(void){

	int conexionAppeared, conexionCaugth, conexionLocalized;

	t_log* logger;

	char* conf = "/home/utnso/tp-2020-1c-NN/team/src/team.config";
	t_config *config = config_create(conf);
	Config_Team *config_team = construirConfigTeam(config);

	logger =log_create("team.log", "Team", 1, LOG_LEVEL_INFO);

	//Obtener entrenadores y definir objetivo global

    //t_list *entrenadores = config_team->pokemon_entrenadores;

    //int cantEntrenadores = list_size(entrenadores);
    //log_info(logger,"Cantidad de entrenadores %d", cantEntrenadores);

	//Conectarse al broker

	char *ip = config_team->ip_broker;
	char *puerto = config_team->puerto_broker;
	log_info(logger,"Lei la IP %s y puerto %s", ip, puerto);

	//conexionAppeared = crear_conexion(ip,puerto);
	//conexionCaugth = crear_conexion(ip,puerto);
    //conexionLocalized =	crear_conexion(ip,puerto);

    //enviar_mensaje("Suscribime",conexionAppeared, SUS_LOC);

    //log_info(logger,"Me suscribi a la cola LOCALIZED!");

    //Escuchar gameboy
    //iniciar_servidor(ip,puerto);

    //Envio mensaje a la cola get_pokemon por cada pokemon que necesito

    t_list *pokemonEntrenadores = config_team->pokemon_entrenadores;
    t_list *objetivosEntrenadores = config_team->objetivos_entrenadores;

    int cantEntrenadores = list_size(pokemonEntrenadores);
    log_info(logger,"Cantidad de entrenadores %d", cantEntrenadores);

    t_list *objetivoUnificado = listaUnica(objetivosEntrenadores);
    t_list *pokemonesUnificado = listaUnica(pokemonEntrenadores);
    t_list *objetivoReal = filtrado(pokemonesUnificado,objetivoUnificado);

    recorrerLista(objetivoReal,logger);


    //Espero respuestas del get en la cola LOCALIZED

	//char *posicion = recibir_mensaje_cliente(conexionLocalized);

	//Planificacion de los entrenadores para ir hasta la posicion del pokemon

/*

	pthread_t entrenador[cantidadEntrenadores];
	/*
	for(i=0;i<cantidadEntrenadores;i++)
	pthread_create(&(entrenador[i]),NULL,(void)funcionEntrena,NULL);
	pthread_join(entrenador[i],NULL);


	void funcionEntrena(t_entrenador)


	t_queue *entrenadores= create_queue();
	queue_pop(entrenador);

*/

	//log_info(logger,"La posicion de pikachu es \n");

	//terminar_programa(get,logger,config);

}

void recorrerLista(t_list *lista, t_log *logger){
	for (int i =0; i< list_size(lista); i++){
			char *pokemon= list_get(lista,i);
			log_info(logger,"Pokemon: %s",pokemon);
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

