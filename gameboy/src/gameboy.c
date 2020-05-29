#include "/home/utnso/tp-2020-1c-NN/biblioteca/biblioteca.c"
#include <commons/log.h>

int main(int argc, char *argv[]) {

	char *ip;
	char *puerto;
	int conexion;
	char *ips = string_new();
	char *puertos = string_new();
	char *mensajeFinal = string_new();

	fflush(stdout);

	t_log* logger;
	t_config* config;

	char* conf = "/home/utnso/tp-2020-1c-NN/gameboy/src/gameboy.config";

	logger =log_create("gameboy.log", "Gameboy", 1, LOG_LEVEL_INFO);

	config=config_create(conf);

	string_append_with_format(&ips,"%s_%s","IP",argv[1]);
	string_append_with_format(&puertos,"%s_%s","PUERTO",argv[1]);

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
		string_append_with_format(&mensajeFinal,"%s",argv[3]);
		string_append_with_format(&mensajeFinal," %s",argv[4]);
		string_append_with_format(&mensajeFinal," %s",argv[5]);
		string_append_with_format(&mensajeFinal," %s\n",argv[6]);
		enviar_mensaje(mensajeFinal,conexion,SUSCRIBIR);
	}else if(strcmp(argv[2],"CATCH_POKEMON") && strcmp(argv[1],"BROKER") ==0){
		enviar_mensaje("Broker Puto",conexion,CATCH_POKEMON);
	}else if(strcmp(argv[2],"CATCH_POKEMON") && strcmp(argv[1],"GAMECARD") ==0){
		enviar_mensaje("Broker Puto",conexion,CATCH_POKEMON);
	}

	log_info(logger,"Envie mensaje");

	terminar_programa(conexion,logger,config);

	return EXIT_SUCCESS;
}
