#include "team.h"
#include "funciones_auxiliares.h"
#include "hilos.h"

int main(/*int argc, char* argv[]*/){

	/*ID_PROCESO = malloc(strlen(argv[1])+1);
	strcpy(ID_PROCESO,argv[1]);*/

	char* conf = "/home/utnso/tp-2020-1c-NN/team/src/team.config";
	t_config *config_team = config_create(conf);
	config = construirConfigTeam(config_team);
	logger = log_create(config->log, "Team", 1, LOG_LEVEL_INFO);
	setearVariablesGlobales();

	//Crear hilo por cada entrenadores y definir objetivo global

	log_info(logger,"Cantidad de entrenadores %d", cantEntrenadores);
	new = crearEntrenadores(config_team);

	log_info(logger,"Objetivo Global:");
	objetivoGlobal = obtenerObjetivoGlobal(config_team);
	recorrerLista(objetivoGlobal,logger);

	//Conectarse al broker

	pthread_t conexionLocalized;
    pthread_create(&conexionLocalized, NULL,(void*) conexion_localized, NULL);

    pthread_t conexionCaugth;
    pthread_create(&conexionCaugth, NULL, (void*) conexion_caugth, NULL);

    pthread_t conexionAppeared;
    pthread_create(&conexionAppeared, NULL,(void*) conexion_appeared ,NULL);

    //Escuchar gameboy

    pthread_t conexionGameboy;
    pthread_create(&conexionGameboy, NULL,(void*) conexion_gameboy, NULL);

    //Envio mensaje a la cola get_pokemon por cada pokemon que necesito

    solicitar_pokemones(objetivoGlobal);

    pthread_t ponerEnEjecuccion;
    pthread_create(&ponerEnEjecuccion, NULL, (void*) poner_en_exce, NULL);

    pthread_t deadLock;
    pthread_create(&deadLock, NULL, (void*) deadlock, NULL);

    pthread_join(deadLock, NULL);

    log_destroy(logger);
    config_destroy(config_team);
    list_destroy_and_destroy_elements(objetivoGlobal, (void*) free);
    pthread_mutex_destroy(&mxExce);
    sem_destroy(&semaforoExce);
    sem_destroy(&semaforoIntercambio);
    sem_destroy(&semaforoDeadlock);

    liberar_listas_config();

    //free(ID_PROCESO);

}

void free_poketeam(Poketeam* pokemon){
	free(pokemon->pokemon);
	free(pokemon);
}

void liberar_listas_config(){
	liberar_lista_de_listas(config->objetivos_entrenadores);
	liberar_lista_de_listas(config->pokemon_entrenadores);
	list_destroy_and_destroy_elements(config->posiciones_entrenadores, (void*) free);
}

void liberar_lista_de_listas(t_list* listaDeListas){
	int sizeListaDeListas = list_size(listaDeListas);
	for (int i =0; i< sizeListaDeListas; i++){
		list_destroy_and_destroy_elements(list_get(listaDeListas,i), (void*) free);
	}
}
