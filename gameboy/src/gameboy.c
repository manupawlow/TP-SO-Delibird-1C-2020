#include "/home/utnso/tp-2020-1c-NN/biblioteca/biblioteca.c"
#include <commons/log.h>

void concatena(char *inicio, char *fin){
	while(*inicio){
		inicio++;
	}
	while(*fin){
		*inicio=*fin;
		inicio++;
		fin++;
	}
	*inicio='\0';
}

int main(int argc, char *argv[]) {

	char *ip;
	char *puerto;
	int conexion;

	char ips[20] = "IP_";
	char puertos[20] = "PUERTO_";
	//code_op codigo = argv[2];

	fflush(stdout);

	t_log* logger;
	t_config* config;

	char* conf = "/home/utnso/tp-2020-1c-NN/gameboy/src/gameboy.config";

	logger =log_create("gameboy.log", "Gameboy", 1, LOG_LEVEL_INFO);

	config=config_create(conf);

	concatena(ips, argv[1]);
	concatena(puertos, argv[1]);

	ip= config_get_string_value(config,ips);
	puerto= config_get_string_value(config,puertos);

	log_info(logger,"Lei la IP %s y puerto %s", ip, puerto);
/*
	switch(codigo){
	case CATCH:
	   enviar
	}
	*/
	//Conectarse al broker

	conexion = crear_conexion(ip,puerto);

	enviar_mensaje("Broker Puto",conexion,SUSCRIBIR);

	log_info(logger,"Envie mensaje");

	terminar_programa(conexion,logger,config);

	return EXIT_SUCCESS;
}
