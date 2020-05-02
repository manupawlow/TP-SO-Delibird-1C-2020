#include "/home/utnso/tp-2020-1c-NN/biblioteca/biblioteca.c"
#include <commons/log.h>

int main(void) {

	char* ip;
	char* puerto;
	int conexion;

	t_log* logger;
	t_config* config;

	char* conf = "/home/utnso/tp-2020-1c-NN/gamecard/src/gamecard.config";

	logger =log_create("gamecard.log", "Gamecard", 1, LOG_LEVEL_INFO);

    config=config_create(conf);

	ip= config_get_string_value(config,"IP_BROKER");
	puerto= config_get_string_value(config,"PUERTO_BROKER");

	log_info(logger,"Lei la IP %s y puerto %s", ip, puerto);

	//Conectarse al broker
	conexion= crear_conexion(ip,puerto);

	enviar_mensaje("subscripcion",conexion);

	char *mensaje = recibir_mensaje_cliente(conexion);

	log_info(logger,"El mensaje recibido es %s\n",mensaje);

	terminar_programa(conexion,logger,config);

	return EXIT_SUCCESS;
}
