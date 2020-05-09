#include "/home/utnso/tp-2020-1c-NN/biblioteca/biblioteca.c"
#include <commons/log.h>

int main(void){

	char* ip;
	char* puerto;
	int conexionAppeared, conexionCaugth, conexionLocalized;

	t_log* logger;
	t_config* config;


	fflush(stdout);


	char* conf = "/home/utnso/tp-2020-1C-NN/team/src/team.config";

	logger =log_create("team.log", "Team", 1, LOG_LEVEL_INFO);

	config=config_create(conf);

	ip= config_get_string_value(config,"IP_BROKER");
	puerto= config_get_string_value(config,"PUERTO_BROKER");

	log_info(logger,"Lei la IP %s y puerto %s", ip, puerto);

	//Conectarse al broker
	conexionAppeared = crear_conexion(ip,puerto);

	enviar_mensaje("Hola",conexionAppeared);

	log_info(logger,"Envie mensaje");
/*
	conexionCaugth = crear_conexion(ip,puerto);
	conexionLocalized =	crear_conexion(ip,puerto);

    //Escuchar gameboy
	iniciar_servidor(ip,puerto);

*/
	char *mensaje = recibir_mensaje_cliente(conexionAppeared);

		log_info(logger,"El mensaje recibido es %s\n",mensaje);
	terminar_programa(conexionAppeared,logger,config);

}




