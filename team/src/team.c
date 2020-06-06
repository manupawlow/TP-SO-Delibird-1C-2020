#include "team.h"
#include "conexiones.h"

int main(void){

	char* conf = "/home/utnso/tp-2020-1c-NN/team/src/team.config";
	t_config *config = config_create(conf);
	Config_Team *config_team = construirConfigTeam(config);
	logger = log_create("team.log", "Team", 1, LOG_LEVEL_INFO);

	//Obtener entrenadores y definir objetivo global

	log_info(logger,"Cantidad de entrenadores %d", list_size(config_team->objetivos_entrenadores));

	entrenadores = crearEntrenadores(config_team);

	log_info(logger,"Objetivo Global:");
	objetivoGlobal = obtenerObjetivoGlobal(config_team);
	recorrerLista(objetivoGlobal,logger);

	//Conectarse al broker

	pthread_t *conexionLocalized;
    pthread_create(&conexionLocalized, NULL, conexion_localized, config_team);

    //pthread_t *conexionAppeared;
    //pthread_create(&conexionAppeared, NULL,conexion_appeared ,config_team);

    //Escuchar gameboy

    pthread_t *conexionGameboy;
    pthread_create(&conexionGameboy, NULL, conexion_gameboy, NULL);

    //Envio mensaje a la cola get_pokemon por cada pokemon que necesito

    //solicitar_pokemones(objetivoGlobal,config_team);

    ready= list_create();

    pthread_join(conexionLocalized,NULL);
    pthread_join(conexionGameboy,NULL);
}
