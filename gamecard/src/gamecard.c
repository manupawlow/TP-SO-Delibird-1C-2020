#include "gamecard.h"
int main() {

	char* conf = "/home/utnso/tp-2020-1c-NN/gamecard/src/gamecard.config";

	logger =log_create("/home/utnso/log_gamecard.txt", "Gamecard", 1, LOG_LEVEL_INFO);
	config=config_create(conf);


	//fs(config,block_size,blocks);
	fs(config,64,5192);


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
void buscarPokemon(t_mensaje* mensaje){

FILE * f;/*
uint32_t pokemon_length;
    char* pokemon;
    uint8_t posx;
    uint8_t posy;
	uint8_t cantidad;
	uint8_t id_mensaje;
	uint8_t id_mensaje_correlativo;
	uint32_t resultado_length;
	char* resultado;*/


t_mensaje* pokemon;

char* montaje= string_new();
//TODO
	// NO ANDA UN STRING APPEND ANDA A SABER POR K

		string_append(&montaje,mntPokemon);
		string_append(&montaje,mensaje->pokemon);

		pthread_mutex_lock(&mxArchivo);
		f = fopen(montaje,"r");

		pokemon=obtenerDatosPokemon(f,mensaje);
		fclose(f);

		pthread_mutex_unlock(&mxArchivo);


		//datoArchivo= obtenerPokemonString(pokemon);


int socketLoc = crear_conexion(ip, puerto);

	if(socketLoc ==-1){
			log_info(logger,"No se pudo conectar con el broker.");
			//log_info(logger,"%s",datoArchivo);

	}else{
//TODO

			enviar_mensaje_struct(mensaje, socketLoc,LOCALIZED_POKEMON);

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
t_mensaje* pokemon= obtenerDatosPokemon(f,mensaje);

	fclose(f);
			if(igualPosicion(f, mensaje,pokemon)!=1){
				log_info(logger,"ERROR: no hay pokemon.");
			}else{
				if(pokemon->posy==1){
					f=fopen(mntPokemon,"r+");
					eliminarLinea();
					fclose(f);
				}else if(pokemon->posy<1){
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


void nuevoPokemon(t_mensaje* mensaje){

	FILE * f;/*
	uint32_t pokemon_length;
	    char* pokemon;
	    uint8_t posx;
	    uint8_t posy;
		uint8_t cantidad;
		uint8_t id_mensaje;
		uint8_t id_mensaje_correlativo;
		uint32_t resultado_length;
		char* resultado;*/



	char* montaje= string_new();
	string_append(&montaje,mntPokemon);
	string_append(&montaje,mensaje->pokemon);
	pthread_mutex_lock(&mxArchivo);
	f = fopen(montaje,"r");
/* si open = y -> esperarReintentar
 * else
 * mandale mecha
 */
	if(f==NULL){

		crearBloques(mensaje);


		mkdir(montaje,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		string_append(&montaje,"/Metadata.bin");
		f=fopen(montaje,"w+");
		escribirMeta(f,mensaje);
		fclose(f);

		contadorBloques++;
		}else{
			//yes
			cambiar_meta_blocks(montaje);
			//no
			fclose(f);

		}

	//pokemon=obtenerDatosPokemon(f,mensaje);


	pthread_mutex_unlock(&mxArchivo);


			//datoArchivo= obtenerPokemonString(pokemon);


	int socketLoc = crear_conexion(ip, puerto);

		if(socketLoc ==-1){
				log_info(logger,"No se pudo conectar con el broker.");
				//log_info(logger,"%s",datoArchivo);

		}else{
	//TODO

				enviar_mensaje_struct(mensaje, socketLoc,LOCALIZED_POKEMON);

		}



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

t_mensaje* obtenerDatosPokemon(FILE* fp, t_mensaje* mensaje){

uint8_t posx;
uint8_t posy;
uint8_t cant;

	if(fp==NULL){
		mensaje->posx= 0;
		mensaje->posy =0;
		mensaje->cantidad=0;
	}else{

		fscanf(fp,"%d-%d=%d", posx,posy,cant );

		mensaje->posx= posx;
		mensaje->posy=posy;
		mensaje->cantidad=cant;

	}
	return mensaje;
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
void funcionACK(){
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
		funcionACK();
		log_info(logger,"Recibi mensaje de contenido pokemon %s y envie confirmacion de su recepcion",mensaje->pokemon);
		pthread_t solicitudGet;
		pthread_create(&solicitudGet, NULL,(void *) buscarPokemon, mensaje);
		pthread_detach(solicitudGet);
		free(mensaje);
		break;
	case NEW_POKEMON:
		mensaje = recibir_mensaje_struct(socket);
		funcionACK();
		log_info(logger,"Recibi mensaje de contenido pokemon %s y envie confirmacion de su recepcion",mensaje->pokemon);
		pthread_t solicitudNew;
		pthread_create(&solicitudNew, NULL,(void *) nuevoPokemon, mensaje);
		pthread_detach(solicitudNew);
		free(mensaje);
		break;
	case CATCH_POKEMON:
		mensaje = recibir_mensaje_struct(socket);
		funcionACK();
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



//-------------------------------------------------------------------------------------


void crearBloques(t_mensaje* mensaje){

FILE* f;
char* contador;
char* bloques=string_new();
char* escribirBloque=string_new();


char* x=string_itoa(mensaje->posx);
char* y=string_itoa(mensaje->posy);
char* cant=string_itoa(mensaje->cantidad);

contador=string_itoa(contadorBloques);
string_append(&bloques,"/home/utnso/Escritorio/TALL_GRASS/Blocks/");
string_append(&bloques,contador);
string_append(&bloques,".bin");

f=fopen(bloques,"w+");

string_append(&escribirBloque, x);
string_append_with_format(&escribirBloque, "-%s",y);
string_append_with_format(&escribirBloque, "=%s\n",cant);


fprintf(f,"%s",escribirBloque);
fclose(f);



}


void escribirMeta(FILE* f,t_mensaje* mensaje){

	fprintf(f,"DIRECTORY=N\n");
	fprintf(f,"SIZE=%d\n",64);
	fprintf(f,"BLOCKS=%d\n",contadorBloques);
	fprintf(f,"OPEN=N");


}


void cambiar_meta_blocks(char* montaje,t_mensaje* mensaje){
	FILE* fblocks;
	char* bloques;

	//falta liberar
	t_config* configBloques;

	char** arrayBloques;
	int arrayBloquesInt[10];
	char** basura;
	char** nroBloque;

	configBloques= config_create(montaje);
	bloques=config_get_string_value(configBloques,"BLOCKS");
	log_info(logger,"%s",bloques);

	basura=string_split(bloques,"[");
	nroBloque=string_split(basura[0],"]");
	arrayBloques=string_split(nroBloque[0],",");


 /*
	while(arrayBloques[i]!= NULL){
		arrayBloquesInt[i]=atoi(arrayBloques[i]);
		log_info(logger,"%d",arrayBloquesInt[i]);
		i++;

	}*/

	for(int i=0;i<strlen(arrayBloques);i++){
		char* montajeBlocks=string_new();
		string_append(montajeBlocks,montaje);
		string_append_with_format(montajeBlocks,"/%s.bin",arrayBloques[i]);
		fblocks= fopen(montajeBlocks, "r+");

		if(fblocks== NULL){
			//fclose(fblocks);
			log_info(logger,"El bloque %s no existe", arrayBloques[i]);
		}else{
			compararBlocks(fblocks, mensaje);
		}
	}




void compararBlocks(FILE* f, t_mensaje* mensaje){
int x;
int y;
int cant;

	fscanf(f,"%d-%d=%d",x,y,cant);

	if(x==mensaje->posx){
		if(y==mensaje->posy){

		}
	}

}







	free(bloques);
	free(arrayBloques);
	free(basura);
	free(nroBloque);



}







