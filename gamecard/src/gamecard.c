#include "/home/utnso/tp-2020-1c-NN/biblioteca/biblioteca.c"
#include <commons/log.h>

int main(void) {

	char* ip;
	char* puerto;
	int conexionGet, conexionCatch, conexionNew;

	t_log* logger;
	t_config* config;

	char* conf = "/home/utnso/tp-2020-1c-NN/gamecard/src/gamecard.config";

	logger =log_create("gamecard.log", "Gamecard", 1, LOG_LEVEL_INFO);
	config=config_create(conf);

	ip= config_get_string_value(config,"IP_BROKER");
	puerto= config_get_string_value(config,"PUERTO_BROKER");

	log_info(logger,"Lei la IP %s y puerto %s", ip, puerto);

	//Conectarse al broker
	conexionGet= crear_conexion(ip,puerto);
	//conexionCatch = crear_conexion(ip,puerto);
	//conexionNew = crear_conexion(ip,puerto);

	enviar_mensaje("Suscribime",conexionGet, SUS_GET);

	//Se queda esperando que le llegue algun mensaje a la cola GET
	while(1){
	char *mensaje = recibir_mensaje_cliente(conexionGet);
	log_info(logger,"Buscar posicion de %s\n",mensaje);
	}
	//TODO: se fija posicion pokemon

	//Envia posicion pokemon a la cola LOCALIZED
	int conexionLocalized = crear_conexion(ip,puerto);
	enviar_mensaje("Posicion x y",conexionLocalized, LOCALIZED_POKEMON);
	close(conexionLocalized);

    //log_info(logger,"El mensaje recibido es %s\n",mensaje);

	terminar_programa(conexionGet,logger,config);

	return EXIT_SUCCESS;
}
