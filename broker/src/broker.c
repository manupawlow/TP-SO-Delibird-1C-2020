#include "/home/utnso/tp-2020-1c-NN/biblioteca/biblioteca.c"
#include <commons/log.h>

#include <stdio.h>
#include <stdlib.h>

int main(void) {

	char* ip;
	char* puerto;

	t_log* logger;
	t_config* config;

	char* conf = "/home/utnso/tp-2020-1c-NN/broker/src/broker.config";

	logger =log_create("broker.log", "Broker", 1, LOG_LEVEL_INFO);

	config=config_create(conf);

	ip= config_get_string_value(config,"IP_BROKER");
	puerto= config_get_string_value(config,"PUERTO_BROKER");

	log_info(logger,"Lei la IP %s y puerto %s", ip, puerto);

	iniciar_servidor(ip,puerto);


	return EXIT_SUCCESS;
}
