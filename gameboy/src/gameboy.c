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
	//*inicio='\0';
}
void concatena_con_espacio(char *inicio, char *fin, char *espacio){
	while(*inicio){
		inicio++;
	}
	*inicio=*espacio;
	inicio++;
	while(*fin){
		*inicio=*fin;
		inicio++;
		fin++;
	}
}
int main(int argc, char *argv[]) {

	char *ip;
	char *puerto;
	int conexion;
	char *espacio;
	espacio = " ";

	char *ips = NULL;
	ips = (char*) malloc(20*sizeof(char));
	memcpy(ips,"IP_",4);
	char *puertos = NULL;
	puertos = (char*) malloc(20*sizeof(char));
	memcpy(puertos,"PUERTO_",8);

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

	//Conectarse al broker

	conexion = crear_conexion(ip,puerto);

	/*if(strcmp(argv[2],"SUSCRIBIR") == 0){
		enviar_mensaje("Broker Puto",conexion,SUSCRIBIR);
	}*//*if(strcmp(argv[2],"SUSCRIBIR") == 0){
		for(int i =3; i<6;i++){
			enviar_mensaje(argv[i],conexion,SUSCRIBIR);
		}
	}*/if(strcmp(argv[2],"NEW_POKEMON") == 0){
		enviar_mensaje("Broker Puto",conexion,NEW_POKEMON);
	}else if(strcmp(argv[2],"APPEARED_POKEMON") == 0){
		enviar_mensaje("Broker Puto",conexion,APPEARED_POKEMON);
	}else if(strcmp(argv[2],"CAUGHT_POKEMON") == 0){
		enviar_mensaje("Broker Puto",conexion,CAUGHT_POKEMON);
	}else if(strcmp(argv[2],"SUSCRIBIR") == 0){
		concatena_con_espacio(argv[3],argv[4],espacio);
		concatena_con_espacio(argv[3],argv[5],espacio);
		concatena_con_espacio(argv[3],argv[6],espacio);
		enviar_mensaje(argv[3],conexion,SUSCRIBIR);
	}else if(strcmp(argv[2],"CATCH_POKEMON") && strcmp(argv[1],"BROKER") ==0){
		enviar_mensaje("Broker Puto",conexion,CATCH_POKEMON);
	}else if(strcmp(argv[2],"CATCH_POKEMON") && strcmp(argv[1],"GAMECARD") ==0){
		enviar_mensaje("Broker Puto",conexion,CATCH_POKEMON);
	}

	log_info(logger,"Envie mensaje");

	terminar_programa(conexion,logger,config);

	return EXIT_SUCCESS;
}
