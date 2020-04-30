#include "/home/utnso/tp-2020-1c-NN/biblioteca/biblioteca.c"

int main(void){

	char* ip;
	char* puerto;
	int conexionAppeared, conexionCaugth, conexionLocalized;

	t_log* logger;
	t_config* config;

	char* conf = "./team.config";

	logger =log_create("team.log", "Team", 1, LOG_LEVEL_INFO);
	config=config_create(conf);

	ip= config_get_string_value(config,"IP_BROKER");
	puerto= config_get_string_value(config,"PUERTO_BROKER");

	log_info(logger,"Lei la IP %s y puerto %s", ip, puerto);
	printf("mm");
	conexionAppeared = crear_conexion(ip,puerto);
	conexionCaugth = crear_conexion(ip,puerto);
	conexionLocalized =	crear_conexion(ip,puerto);

    printf("mm");
	//iniciar_servidor(ip,puerto);



}
