#include <bibliotec.h>
	t_log* logger;
	pthread_mutex_t mx;

void recibir_y_mostrar(int conexion){
	pthread_mutex_lock(&mx);
	int cod_op;
	recv(conexion, &cod_op, sizeof(op_code), MSG_WAITALL);
	t_mensaje* mensaje= recibir_mensaje_struct(conexion);
	log_info(logger,"Recibi el mensaje %d", mensaje->id_mensaje);
	free(mensaje->pokemon);
	free(mensaje);
	pthread_mutex_unlock(&mx);
}
void recibir_y_mostrar_localized(int conexion){
	pthread_mutex_lock(&mx);
	int cod_op;
	recv(conexion, &cod_op, sizeof(op_code), MSG_WAITALL);
	t_mensaje_get* mensaje = recibir_mensaje_struct_get(conexion);
	log_info(logger,"Recibi el mensaje %d", mensaje->id_mensaje);
	free(mensaje->pokemon);
	free(mensaje);
	pthread_mutex_unlock(&mx);
}

int main(int argc, char *argv[]) {


	//argv[1]="SUSCRIPTOR";
	//argv[2]="NEW_POKEMON";
	//argv[3]="10";

	pthread_mutex_init(&mx,NULL);
	char *ip;
	char *puerto;
	int conexion;
	char *ips = string_new();
	char *puertos = string_new();
	int cod_op;

	t_mensaje* mensaje_a_recibir;
	t_mensaje* mensaje_struct = malloc(sizeof(t_mensaje));
	t_buffer* buffer;


	t_config* config;

	char* conf = "/home/utnso/tp-2020-1c-NN/gameboy/src/gameboy.config";

	logger = log_create("/home/utnso/gameboy.txt", "Gameboy", 1, LOG_LEVEL_INFO);

	config = config_create(conf);

	if(strcmp(argv[1],"SUSCRIPTOR") == 0){

		ip= config_get_string_value(config,"IP_BROKER");
		puerto= config_get_string_value(config,"PUERTO_BROKER");

		conexion = crear_conexion(ip,puerto);

		if(conexion == -1)
			log_info(logger,"No me pude conectar :(");

		log_info(logger,"Me suscribi a la cola %s", argv[2]);

		if(strcmp(argv[2],"NEW_POKEMON") == 0){
			enviar_mensaje("99",conexion,SUS_NEW);
		}else if(strcmp(argv[2],"APPEARED_POKEMON") == 0){
			enviar_mensaje("99",conexion,SUS_APP);
		}else if(strcmp(argv[2],"CATCH_POKEMON") == 0){
			enviar_mensaje("99",conexion,SUS_CATCH);
		}else if(strcmp(argv[2],"CAUGHT_POKEMON") == 0){
			enviar_mensaje("99",conexion,SUS_CAUGHT);
		}else if(strcmp(argv[2],"GET_POKEMON") == 0){
			enviar_mensaje("99",conexion,SUS_GET);
		}else if(strcmp(argv[2],"LOCALIZED_POKEMON") == 0){
			enviar_mensaje("99",conexion,SUS_LOC);
		}

		log_info(logger,"Suscripcion a cola %s por %s segundos",argv[2],argv[3]);

		int segundosPermitidos = atoi(argv[3]);
		uint64_t time = timestamp();
		uint64_t final_time = time + segundosPermitidos * 1000;
		pthread_t hilo;
		while(time < final_time){
			if(strcmp(argv[2],"LOCALIZED_POKEMON") == 0)
				pthread_create(&hilo,NULL,(void*)recibir_y_mostrar_localized,(void*) conexion);
			else
				pthread_create(&hilo,NULL,(void*)recibir_y_mostrar,(void*) conexion);

			time = timestamp();
		}
		log_info(logger,"Finalizo la suscripcion a la cola %s.", argv[2]);
		exit(1);

	}else{

		string_append_with_format(&ips,"%s_%s","IP",argv[1]);
		string_append_with_format(&puertos,"%s_%s","PUERTO",argv[1]);

		ip = config_get_string_value(config,ips);
		puerto = config_get_string_value(config,puertos);

		conexion = crear_conexion(ip,puerto);

		if(conexion == -1)
			log_info(logger,"No me pude conectar :(");

		log_info(logger,"Me conecte al proceso %s", argv[1]);

		mensaje_struct->pokemon = string_new();
		mensaje_struct->pokemon_length = strlen(mensaje_struct->pokemon)+1;
		mensaje_struct->resultado = 0;
		mensaje_struct->posx = 0;
		mensaje_struct->posy = 0;
		mensaje_struct->cantidad = 0;
		mensaje_struct->id_mensaje = 0;
		mensaje_struct->id_mensaje_correlativo = 0;
	}

	if(strcmp(argv[2],"NEW_POKEMON") == 0){
		if(strcmp(argv[1],"BROKER") == 0){
			mensaje_struct->pokemon = (char *) realloc(mensaje_struct->pokemon, strlen(argv[3]) +1);
			strcpy(mensaje_struct->pokemon,argv[3]);
			mensaje_struct->pokemon_length = strlen(mensaje_struct->pokemon)+1;
			mensaje_struct->posx = atoi(argv[4]);
			mensaje_struct->posy = atoi(argv[5]);
			mensaje_struct->cantidad = atoi(argv[6]);
			buffer = serializar_mensaje_struct(mensaje_struct);
			enviar_mensaje_struct(buffer,conexion,NEW_POKEMON);
			recv(conexion, &cod_op, sizeof(op_code), MSG_WAITALL);
			mensaje_a_recibir = recibir_mensaje_struct(conexion);
			log_info(logger,"Envie un mensaje a la cola %s",argv[2]);
			free(buffer->stream);
			free(buffer);
			free(mensaje_a_recibir->pokemon);
			free(mensaje_a_recibir);
		} else if(strcmp(argv[1],"GAMECARD") == 0){
			mensaje_struct->pokemon = (char *) realloc(mensaje_struct->pokemon, strlen(argv[3]) +1);
			strcpy(mensaje_struct->pokemon,argv[3]);
			mensaje_struct->pokemon_length = strlen(mensaje_struct->pokemon)+1;
			mensaje_struct->posx = atoi(argv[4]);
			mensaje_struct->posy = atoi(argv[5]);
			mensaje_struct->cantidad = atoi(argv[6]);
			mensaje_struct->id_mensaje = atoi(argv[7]);
			buffer = serializar_mensaje_struct(mensaje_struct);
			enviar_mensaje_struct(buffer,conexion,NEW_POKEMON);
			free(buffer->stream);
			free(buffer);
			log_info(logger,"Envie un mensaje a la cola %s",argv[2]);
		}
	}else if(strcmp(argv[2],"APPEARED_POKEMON") == 0){
		if(strcmp(argv[1],"TEAM") == 0){
			mensaje_struct->pokemon = (char *) realloc(mensaje_struct->pokemon, strlen(argv[3]) +1);
			strcpy(mensaje_struct->pokemon,argv[3]);
			mensaje_struct->pokemon_length = strlen(mensaje_struct->pokemon)+1;
			mensaje_struct->posx = atoi(argv[4]);
			mensaje_struct->posy = atoi(argv[5]);
			buffer = serializar_mensaje_struct(mensaje_struct);
			enviar_mensaje_struct(buffer,conexion,APPEARED_POKEMON);
			free(buffer->stream);
			free(buffer);
			log_info(logger,"Envie un mensaje a la cola %s",argv[2]);
		} else if(strcmp(argv[1],"BROKER") == 0){
			mensaje_struct->pokemon = (char *) realloc(mensaje_struct->pokemon, strlen(argv[3]) +1);
			strcpy(mensaje_struct->pokemon,argv[3]);
			mensaje_struct->pokemon_length = strlen(mensaje_struct->pokemon)+1;
			mensaje_struct->posx = atoi(argv[4]);
			mensaje_struct->posy = atoi(argv[5]);
			mensaje_struct->id_mensaje_correlativo = atoi(argv[6]);
			buffer = serializar_mensaje_struct(mensaje_struct);
			enviar_mensaje_struct(buffer,conexion,APPEARED_POKEMON);
			recv(conexion, &cod_op, sizeof(op_code), MSG_WAITALL);
			mensaje_a_recibir = recibir_mensaje_struct(conexion);
			log_info(logger,"Envie un mensaje a la cola %s",argv[2]);
			free(buffer->stream);
			free(buffer);
			free(mensaje_a_recibir->pokemon);
			free(mensaje_a_recibir);
		}
	}else if(strcmp(argv[2],"CATCH_POKEMON") == 0){
		if(strcmp(argv[1],"GAMECARD") == 0){
			mensaje_struct->pokemon = (char *) realloc(mensaje_struct->pokemon, strlen(argv[3]) +1);
			strcpy(mensaje_struct->pokemon,argv[3]);
			mensaje_struct->pokemon_length = strlen(mensaje_struct->pokemon)+1;
			mensaje_struct->posx = atoi(argv[4]);
			mensaje_struct->posy = atoi(argv[5]);
			mensaje_struct->id_mensaje = atoi(argv[6]);
			buffer = serializar_mensaje_struct(mensaje_struct);
			enviar_mensaje_struct(buffer,conexion,CATCH_POKEMON);
			free(buffer->stream);
			free(buffer);
			log_info(logger,"Envie un mensaje a la cola %s",argv[2]);
		} else if(strcmp(argv[1],"BROKER") == 0){
			mensaje_struct->pokemon = (char *) realloc(mensaje_struct->pokemon, strlen(argv[3]) +1);
			strcpy(mensaje_struct->pokemon,argv[3]);
			mensaje_struct->pokemon_length = strlen(mensaje_struct->pokemon)+1;
			mensaje_struct->posx = atoi(argv[4]);
			mensaje_struct->posy = atoi(argv[5]);
			buffer = serializar_mensaje_struct(mensaje_struct);
			enviar_mensaje_struct(buffer,conexion,CATCH_POKEMON);
			recv(conexion, &cod_op, sizeof(op_code), MSG_WAITALL);
			mensaje_a_recibir = recibir_mensaje_struct(conexion);
			log_info(logger,"Envie un mensaje a la cola %s",argv[2]);
			free(buffer->stream);
			free(buffer);
			free(mensaje_a_recibir->pokemon);
			free(mensaje_a_recibir);
		}
	}else if(strcmp(argv[2],"CAUGHT_POKEMON") == 0){
		mensaje_struct->id_mensaje_correlativo = atoi(argv[3]);
		if(strcmp(argv[4], "OK")== 0)
			mensaje_struct->resultado = 1;
		buffer = serializar_mensaje_struct(mensaje_struct);
		enviar_mensaje_struct(buffer,conexion,CAUGHT_POKEMON);
		recv(conexion, &cod_op, sizeof(op_code), MSG_WAITALL);
		mensaje_a_recibir = recibir_mensaje_struct(conexion);
		log_info(logger,"Envie un mensaje a la cola %s",argv[2]);
		free(buffer->stream);
		free(buffer);
		free(mensaje_a_recibir->pokemon);
		free(mensaje_a_recibir);
	}
	else if(strcmp(argv[2],"GET_POKEMON") == 0){
		if(strcmp(argv[1],"GAMECARD") == 0){
			mensaje_struct->pokemon = (char *) realloc(mensaje_struct->pokemon, strlen(argv[3]) +1);
			strcpy(mensaje_struct->pokemon,argv[3]);
			mensaje_struct->pokemon_length = strlen(mensaje_struct->pokemon)+1;
			mensaje_struct->id_mensaje = atoi(argv[4]);
			buffer = serializar_mensaje_struct(mensaje_struct);
			enviar_mensaje_struct(buffer,conexion,GET_POKEMON);
			free(buffer->stream);
			free(buffer);
		log_info(logger,"Envie un mensaje a la cola %s",argv[2]);
		} else if(strcmp(argv[1],"BROKER") == 0){
			mensaje_struct->pokemon = (char *) realloc(mensaje_struct->pokemon, strlen(argv[3]) +1);
			strcpy(mensaje_struct->pokemon,argv[3]);
			mensaje_struct->pokemon_length = strlen(mensaje_struct->pokemon)+1;
			buffer = serializar_mensaje_struct(mensaje_struct);
			enviar_mensaje_struct(buffer,conexion,GET_POKEMON);
			recv(conexion, &cod_op, sizeof(op_code), MSG_WAITALL);
			mensaje_a_recibir = recibir_mensaje_struct(conexion);
			log_info(logger,"Envie un mensaje a la cola %s",argv[2]);
			free(buffer->stream);
			free(buffer);
			free(mensaje_a_recibir->pokemon);
			free(mensaje_a_recibir);
		}
	}
	//Este es para hacer las pruebas

	else if(strcmp(argv[2],"SUSCRIBIR") == 0){
		strcpy(mensaje_struct->pokemon,argv[5]);
		mensaje_struct->pokemon_length = strlen(mensaje_struct->pokemon)+1;
		if(strcmp(argv[6], "OK"))
			mensaje_struct->resultado = 1;
		mensaje_struct->posx = atoi(argv[3]);
		mensaje_struct->posy = atoi(argv[4]);
		mensaje_struct->cantidad = atoi(argv[7]);
		mensaje_struct->id_mensaje = atoi(argv[8]);
		mensaje_struct->id_mensaje_correlativo = atoi(argv[9]);
		buffer = serializar_mensaje_struct(mensaje_struct);
		enviar_mensaje_struct(buffer,conexion,SUSCRIBIR);
		log_info(logger,"Envie un mensaje a la cola %s",argv[2]);
	}
	free(ips);
	free(puertos);

	terminar_programa(conexion,logger,config);

	return EXIT_SUCCESS;
}
