#include "/home/utnso/tp-2020-1c-NN/biblioteca/biblioteca.c"
#include <commons/log.h>
#include <commons/temporal.h>

int main(int argc, char *argv[]) {

	char *ip;
	char *puerto;
	int conexion;
	char *ips = string_new();
	char *puertos = string_new();
	char *mensaje;

	fflush(stdout);

	t_log* logger;
	t_config* config;

	char* conf = "/home/utnso/tp-2020-1c-NN/gameboy/src/gameboy.config";

	logger =log_create("gameboy.log", "Gameboy", 1, LOG_LEVEL_INFO);

	config=config_create(conf);

	if(strcmp(argv[1],"SUSCRIPTOR") == 0){

		ip= config_get_string_value(config,"IP_BROKER");
		puerto= config_get_string_value(config,"PUERTO_BROKER");

		conexion = crear_conexion(ip,puerto);

		log_info(logger,"Me conecte a la IP %s y puerto %s", ip, puerto);

		enviar_mensaje(concatena(6,argv),conexion,SUSCRIBIR);
		log_info(logger,"Suscripcion a cola %s por %d segundos",argv[2],argv[3]);

		//temporal_get_string_time() devuelve el tiempo en hh:mm:ss:mmmm (es un char*)

		int segundosPermitidos = atoi(argv[3]);
		char ** tiempo = string_split(temporal_get_string_time(),":");
		int horas = atoi(tiempo[0]);
		int minutos = atoi(tiempo[1]);
		int segundos = atoi(tiempo[2]);
		int segundosFinal = segundos + horas*60*60 + minutos*60 + segundosPermitidos;

		while(segundos < segundosFinal){

			mensaje = recibir_mensaje_cliente(conexion);
			log_info(logger,"El mensaje recibido es %s\n",mensaje);

			tiempo = string_split(temporal_get_string_time(),":");
			horas = atoi(tiempo[0]);
			minutos = atoi(tiempo[1]);
			segundos = atoi(tiempo[2]);
			segundos = segundos + horas*60*60 + minutos*60;
		}

	}else{

		string_append_with_format(&ips,"%s_%s","IP",argv[1]);
		string_append_with_format(&puertos,"%s_%s","PUERTO",argv[1]);

		ip= config_get_string_value(config,ips);
		puerto= config_get_string_value(config,puertos);

		conexion = crear_conexion(ip,puerto);

		log_info(logger,"Me conecte a la IP %s y puerto %s", ip, puerto);
	}

	if(strcmp(argv[2],"NEW_POKEMON") == 0){
		if(strcmp(argv[1],"BROKER") == 0){
			enviar_mensaje(concatena(5,argv),conexion,NEW_POKEMON);
			log_info(logger,"Envie un mensaje a la cola %s",argv[2]);
		} else if(strcmp(argv[1],"GAMECARD") == 0){
			enviar_mensaje(concatena(6,argv),conexion,NEW_POKEMON);
			log_info(logger,"Envie un mensaje a la cola %s",argv[2]);
		}
	}else if(strcmp(argv[2],"APPEARED_POKEMON") == 0){
		if(strcmp(argv[1],"TEAM") == 0){
			enviar_mensaje(concatena(5,argv),conexion,APPEARED_POKEMON);
			log_info(logger,"Envie un mensaje a la cola %s",argv[2]);
		} else if(strcmp(argv[1],"BROKER") == 0){
			enviar_mensaje(concatena(6,argv),conexion,APPEARED_POKEMON);
			log_info(logger,"Envie un mensaje a la cola %s",argv[2]);
		}
	}else if(strcmp(argv[2],"CATCH_POKEMON") == 0){
		if(strcmp(argv[1],"GAMECARD") == 0){
			enviar_mensaje(concatena(6,argv),conexion,CATCH_POKEMON);
			log_info(logger,"Envie un mensaje a la cola %s",argv[2]);
		} else if(strcmp(argv[1],"BROKER") == 0){
			enviar_mensaje(concatena(5,argv),conexion,CATCH_POKEMON);
			log_info(logger,"Envie un mensaje a la cola %s",argv[2]);
		}
	}else if(strcmp(argv[2],"CAUGHT_POKEMON") == 0){
		enviar_mensaje(concatena(4,argv),conexion,CAUGHT_POKEMON);
		log_info(logger,"Envie un mensaje a la cola %s",argv[2]);
	}
	else if(strcmp(argv[2],"GET_POKEMON") == 0){
		if(strcmp(argv[1],"GAMECARD") == 0){
		enviar_mensaje(concatena(4,argv),conexion,GET_POKEMON);
		log_info(logger,"Envie un mensaje a la cola %s",argv[2]);
		} else if(strcmp(argv[1],"BROKER") == 0){
		enviar_mensaje(concatena(3,argv),conexion,GET_POKEMON);
		log_info(logger,"Envie un mensaje a la cola %s",argv[2]);
		}
	}
	//Este es para hacer las pruebas

	else if(strcmp(argv[2],"SUSCRIBIR") == 0){
		enviar_mensaje(concatena(6,argv),conexion,SUSCRIBIR);
		log_info(logger,"Envie un mensaje a la cola %s",argv[2]);
	}

	terminar_programa(conexion,logger,config);

	return EXIT_SUCCESS;
}
