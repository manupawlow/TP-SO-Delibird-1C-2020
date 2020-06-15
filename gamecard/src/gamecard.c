#include "gamecard.h"
int main() {

	char* conf = "/home/utnso/tp-2020-1c-NN/gamecard/src/gamecard.config";

	logger =log_create("gamecard.log", "Gamecard", 1, LOG_LEVEL_INFO);
	config=config_create(conf);

	fs(config);


	ip = config_get_string_value(config,"IP_BROKER");
	puerto = config_get_string_value(config,"PUERTO_BROKER");

	tiempoReconexion=config_get_int_value(config,"TIEMPO_DE_REINTENTO_CONEXION");

	log_info(logger,"Lei la IP %s y puerto %s", ip, puerto);


	pthread_mutex_init(&mxArchivo, NULL);
	//ip = config_get_string_value(config,"IP_GAMECARD");
	//puerto = config_get_string_value(config,"PUERTO_GAMECARD");

	//iniciar_servidor(ip,puerto);


//HILOS DE CONEXIONES


	pthread_t conexionGet;
	pthread_create(&conexionGet, NULL,(void*)funcionGet,NULL);

	pthread_t conexionNew;
	pthread_create(&conexionNew, NULL,(void*)funcionNew,NULL);


	pthread_t conexionCatch;
	pthread_create(&conexionCatch, NULL,(void*)funcionCatch, NULL);

	pthread_join(conexionGet,NULL);
	pthread_join(conexionNew,NULL);
	pthread_join(conexionCatch,NULL);

	/*pthread_t conexionGameboy;
	pthread_create(&conexionGameboy, NULL,(void*) conexion_gameboy, NULL);
	pthread_join(conexionGameboy,NULL);*/



return EXIT_SUCCESS;
}
//PARA EL GET
void buscarPokemon(char* mensaje){

FILE * f;

char* pokeNombre=malloc(sizeof(char*));
char* datoArchivo=string_new();

		pokeNombre=strtok(mensaje," ");
//TODO
	// NO ANDA UN STRING APPEND ANDA A SABER POR K
		//string_append_with_format(&mntPokemon,"%s",pokeNombre);
		//string_append(&mntPokemon,pokeNombre);
		pthread_mutex_lock(&mxArchivo);
		f = fopen(mntPokemon,"r");
		t_poke* pokemon=	obtenerDatosPokemon(f);
		fclose(f);
		pthread_mutex_unlock(&mxArchivo);


		datoArchivo= obtenerPokemonString(pokemon);


int socketLoc = crear_conexion(ip, puerto);

	if(socketLoc ==-1){
			log_info(logger,"No se pudo conectar con el broker.");
			log_info(logger,"%s",datoArchivo);

	}else{
//TODO
//Agregar el id del mensaje original a datoArchivo
		enviar_mensaje(datoArchivo, socketLoc,LOCALIZED_POKEMON);
	}

}





//TODO
//PARA EL CATCH
void agarrarPokemon(char* mensaje){

FILE * f;
char* pokemon;
int socketCaugth = crear_conexion(ip, puerto);

if(socketCaugth ==-1){
		log_info(logger,"No se pudo conectar con el broker");

		//TODO
		//MANDAR DIRECTO
	}else{
		pokemon=strtok(mensaje," ");
		strcat(mntPokemon,pokemon);

		f = fopen(mntPokemon,"r");

		if(f==NULL){
			log_info(logger,"ERROR: no hay pokemon.");
		}else{
t_poke* pokemon= obtenerDatosPokemon(f);

	fclose(f);
			if(igualPosicion(f, mensaje,pokemon)!=1){
				log_info(logger,"ERROR: no hay pokemon.");
			}else{
				if(pokemon->y==1){
					f=fopen(mntPokemon,"r+");
					eliminarLinea();
					fclose(f);
				}else if(pokemon->y<1){
					log_info(logger,"ERROR: no hay pokemon.");
				}else{
					f=fopen(mntPokemon,"r+");
					eliminarLinea();
					fclose(f);
				}
			}
		}
}

/*
 *void resultado;             no se q es todavia
 	 * if (msg.nombre != directorio.archivo[i]){
	 * 	resultado=ERROR;
	 * }else{
	 * 	 if(checkMutex==0){
	 * 		esperarReintentar(tiempoConfig)
	 * 	}else{
	 * 		if(msg.pos != archivo.pos){
	 * 			resultado=ERROR;
	 * 		}else{
	 * 			if(archivo.cant == 1){
	 * 				eliminarLinea(archivo);
	 * 			}elseif(archivo.cant <1){
	 * 				resultado=ERROR;
	 * 			}else{
	 * 				archivo.cant -=1;
	 * 			}
	 * 	esperar(tiempoConfig);    espera y cierra el archivo
	 *
	 * 	mensaje= resultado y ID mensaje original;
	 *
	 * 	int conexionCaugth = crear_conexion(ip, puerto);
	 * 	enviar_mensaje(mensaje, a la cola CAUGTH);
	 *
	 *
	 *
	 * 	si no me puedo conectar al broker informo por logs y lo mando directo
	 * }
	 *
*/

}
















//TODO


void nuevoPokemon(char* mensaje){

}




char* obtenerPokemonString(t_poke* pokemon){
	char* poke;
	char* x=string_new();
	char* y=string_new();
	char* cantPoke=string_new();

		poke= pokemon->nombre;
		x= pokemon->x;
		y= pokemon->y;
		cantPoke= pokemon->cant;


		strcat(poke, " ");
		strcat(poke, x);
		strcat(poke, " ");
		strcat(poke, y);
		strcat(poke, " ");
		strcat(poke, cantPoke);

	return poke;


}

int igualPosicion(FILE *fp, char* mensaje, t_poke* pokemon){

char* poke;
int posCant[3];

	poke=strtok(mensaje," ");
int i= 0;

	while(poke!= NULL){
		posCant[i]=atoi(poke);
		poke= strtok(NULL, " ");
		i++;
	}

	if(pokemon->x== posCant[0] && pokemon->y== posCant[1] && pokemon->cant== posCant[2]){
		return 1;
	}else{
		return 0;
	}

}

t_poke* obtenerDatosPokemon(FILE* fp){

t_poke* pokemon = malloc(sizeof(t_poke));
int posx;
int posy;
int cant;
char* poki=string_new();


	if(fp==NULL){
		pokemon->x= 0;
		pokemon->y=0;
		pokemon->cant=0;
		pokemon->nombre="";
	}else{

		fscanf(fp,"%s %d %d %d",poki, posx,posy,cant );

		pokemon->x= posx;
		pokemon->y=posy;
		pokemon->cant=cant;
		pokemon->nombre=poki;

	}
	return pokemon;
}

void eliminarLinea(FILE* fp){





}

void conexion_gameboy(){
	char *ip = "127.0.0.3";
	char *puerto = "5001";
	int socket_gamecard = iniciar_servidor(ip,puerto);

    while(1){
    	int socket_cliente = esperar_cliente(socket_gamecard);

    	process_request(socket_cliente);
    }
}
void funcionNew(int socket){

    int conexionNew = crear_conexion(ip,puerto);

    if(conexionNew == -1){
    	log_info(logger,"Reintenando reconectar cada %d segundos",tiempoReconexion);
        conexionNew= reintentar_conexion(ip,puerto,tiempoReconexion);
    }

    enviar_mensaje("Suscribime",conexionNew, SUS_NEW);
    log_info(logger,"Me suscribi a la cola NEW!");


    while(conexionNew != -1){
    	process_request(conexionNew);
    }
}
void funcionCatch(int socket){

    int conexionCatch =	crear_conexion(ip,puerto);

    if(conexionCatch == -1){
    	log_info(logger,"Reintenando reconectar cada %d segundos",tiempoReconexion);
        conexionCatch= reintentar_conexion(ip,puerto,tiempoReconexion);
    }


    enviar_mensaje("Suscribime",conexionCatch, SUS_CATCH);
    log_info(logger,"Me suscribi a la cola CATCH!");


    while(conexionCatch != -1){
    	process_request(conexionCatch);
    }

}
void funcionGet(){
	int conexionGet = crear_conexion(ip,puerto);

	if(conexionGet == -1){
		log_info(logger,"Reintenando reconectar cada %d segundos",tiempoReconexion);
        conexionGet= reintentar_conexion(ip,puerto,tiempoReconexion);
	}

	enviar_mensaje("Suscribime",conexionGet, SUS_GET);
	log_info(logger,"Me suscribi a la cola GET!");


	while(conexionGet != -1){
		process_request(conexionGet);
	}

}
void funcionACK(int socket){
	int conexionRespuesta;
	conexionRespuesta = crear_conexion(ip,puerto);
	enviar_mensaje("Llego a Destino",conexionRespuesta,ACK);
}

void process_request(int socket){
	int cod_op;
	t_mensaje* mensaje = malloc(sizeof(t_mensaje));

	if(recv(socket, &cod_op, sizeof(op_code), MSG_WAITALL) == -1)
			cod_op = -1;

	switch (cod_op){
	case GET_POKEMON:
		mensaje = recibir_mensaje_struct(socket);
		funcionACK(socket);
		log_info(logger,"Recibi mensaje de contenido pokemon %s y envie confirmacion de su recepcion",mensaje->pokemon);
		/*pthread_t solicitud;
		pthread_create(&solicitud, NULL,(void *) buscarPokemon, mensaje);
		pthread_detach(solicitud);*/
		free(mensaje);
		break;
	case NEW_POKEMON:
		mensaje = recibir_mensaje_struct(socket);
		funcionACK(socket);
		log_info(logger,"Recibi mensaje de contenido pokemon %s y envie confirmacion de su recepcion",mensaje->pokemon);
		/*pthread_t solicitud;
		pthread_create(&solicitud, NULL,(void *) nuevoPokemon,(gamecard*) mensaje);
		pthread_detach(solicitud);*/
		free(mensaje);
		break;
	case CATCH_POKEMON:
		mensaje = recibir_mensaje_struct(socket);
		funcionACK(socket);
		log_info(logger,"Recibi mensaje de contenido pokemon %s y envie confirmacion de su recepcion",mensaje->pokemon);
		/*pthread_t solicitud;
		pthread_create(&solicitud, NULL,(void*) agarrarPokemon,(gamecard*) mensaje);
		pthread_detach(solicitud);*/
		free(mensaje);
		break;
	case ACK:
		log_info(logger,"hola");
		break;
	case -1:
		//ip = config_get_string_value(config,"IP_GAMECARD");
		//puerto = config_get_string_value(config,"PUERTO_GAMECARD");
		//socket = iniciar_servidor(ip,puerto);
		socket = crear_conexion(ip,puerto);
		log_info(logger,"Codigo de operacion invalido, iniciando servidor gamecard");
		break;
	default:
		exit(1);
		break;
	}
}



