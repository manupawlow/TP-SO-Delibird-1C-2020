#include "team.h"
#include "conexiones.h"
#include "listas.h"

int main(void){

	char* conf = "/home/utnso/tp-2020-1c-NN/team/src/team.config";
	t_config *config_team = config_create(conf);
	config = construirConfigTeam(config_team);
	logger = log_create("team.log", "Team", 1, LOG_LEVEL_INFO);

	setearVariablesGlobales();

	//Crear hilo por cada entrenadores y definir objetivo global

	log_info(logger,"Cantidad de entrenadores %d", list_size(config->objetivos_entrenadores));
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

    //solicitar_pokemones(objetivoGlobal,config_team);

    pthread_t ponerEnEjecuccion;
    pthread_create(&ponerEnEjecuccion, NULL, (void*) poner_en_exce, NULL);

    pthread_join(conexionLocalized,NULL);
    pthread_join(conexionGameboy,NULL);
}
