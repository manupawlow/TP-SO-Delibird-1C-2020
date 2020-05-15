#include "/home/utnso/tp-2020-1c-NN/biblioteca/biblioteca.c"
#include <commons/log.h>

int main(void){

	char* ip;
	char* puerto;
	int conexionAppeared, conexionCaugth, conexionLocalized;

	t_log* logger;
	t_config* config;

	char* conf = "/home/utnso/tp-2020-1c-NN/team/src/team.config";

	logger =log_create("team.log", "Team", 1, LOG_LEVEL_INFO);
	config=config_create(conf);

	//Obtener entrenadores y definir objetivo global
	int cantidadEntrenadores = 3;

	//Conectarse al broker
	ip= config_get_string_value(config,"IP_BROKER");
	puerto= config_get_string_value(config,"PUERTO_BROKER");

	log_info(logger,"Lei la IP %s y puerto %s", ip, puerto);

	//conexionAppeared = crear_conexion(ip,puerto);
	//conexionCaugth = crear_conexion(ip,puerto);
    conexionLocalized =	crear_conexion(ip,puerto);

    enviar_mensaje("Suscribime",conexionAppeared, SUS_LOC);

    log_info(logger,"Me suscribi a la cola LOCALIZED!");

    //Escuchar gameboy
//  iniciar_servidor(ip,puerto);

    //Envio mensaje a la cola get_pokemon por cada pokemon que necesito

    //int pokemones [5];
    //int i,get;
    //for (i = 0;  i < pokemones; i++) {
		int get = crear_conexion(ip,puerto);
		enviar_mensaje("Pikachu", get, GET_POKEMON);
		log_info(logger,"Le pedi posicion de Pikachu");
		close(get);
	//}

    //Espero respuestas del get en la cola LOCALIZED

	char *posicion = recibir_mensaje_cliente(conexionLocalized);

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

	log_info(logger,"La posicion de pikachu es %s\n",posicion);

	terminar_programa(conexionAppeared,logger,config);

}


