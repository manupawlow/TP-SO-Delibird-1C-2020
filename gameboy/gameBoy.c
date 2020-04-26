#include "gameBoy.h"
#include "comunic.h"

int main(void)
{
	int conexion;
	char* ip;
	char* puerto;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger();

	log_info(logger,"Soy un log");

	config = leer_config();

	ip= config_get_string_value(config,"IP_BROKER");
	puerto= config_get_string_value(config, "PUERTO_BROKER");

	log_info(logger,"Lei la IP %s y puerto %s", ip, puerto);

	conexion= crear_conexion(ip,puerto);

	enviar_mensaje("Hola",conexion);

	char *mensaje = recibir_mensaje(conexion);

	log_info(logger,"El mensaje recibido es %s\n",mensaje);

	terminar_programa(conexion, logger, config);
}



t_log* iniciar_logger(void)
{
	t_log *logger;
	if((logger=log_create("tp0.log", "TP0", 1, LOG_LEVEL_INFO))==NULL){
		printf("No pude crear el logger");
		exit(1);
	}else{
		return logger;
	}

}

t_config* leer_config(void)
{
	t_config *config;
	if((config=config_create("./gameBoy.config"))==NULL){
		printf("No pude leer la config");
		exit(2);
	}else{
		return config;
	}
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	if(logger != NULL){
	 log_destroy(logger);
	}

	if(config != NULL){
		config_destroy(config);
	}

	liberar_conexion(conexion);
}
