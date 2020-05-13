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

	ip= config_get_string_value(config,"IP_BROKER");
	puerto= config_get_string_value(config,"PUERTO_BROKER");

	log_info(logger,"Lei la IP %s y puerto %s", ip, puerto);

	//Conectarse al broker
	//conexionAppeared = crear_conexion(ip,puerto);
	//conexionCaugth = crear_conexion(ip,puerto);
    conexionLocalized =	crear_conexion(ip,puerto);

    enviar_mensaje("Suscribime",conexionAppeared, SUS_LOC);

    log_info(logger,"Envie mensaje");

    //Escuchar gameboy
	//iniciar_servidor(ip,puerto);

    //Envio mensaje a la cola get_pokemon por cada pokemon que necesito

    int pokemones [5];
    int i,get;
    for (i = 0;  i < pokemones; i++) {
		get = crear_conexion(ip,puerto);
		enviar_mensaje(pokemones[i],get,GET_POKEMON);
		close(get);
	}

    //Espero respuestas del get en la cola LOCALIZED

	char *posicion = recibir_mensaje_cliente(conexionLocalized);

	//log_info(logger,"El mensaje recibido es %s\n",mensaje);

	terminar_programa(conexionAppeared,logger,config);

}

/*
for(int i=0;i<algo;i++)
pthread_create(&hiloEntrena,(void)funcionEntrena,&entrenador[i]);
pthread_join(hiloEntrena);


void funcionEntrena(t_entrenador)
*/

