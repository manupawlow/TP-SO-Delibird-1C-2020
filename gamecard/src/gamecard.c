#include "gamecard.h"
int main(int argc, char* argv[]) {

	argv[1]="1";
	ID_PROCESO = malloc(strlen(argv[1])+1);
	ID_PROCESO = argv[1];


system("clear");
	logger = log_create("/home/utnso/log_gamecard.txt", "Gamecard", 1, LOG_LEVEL_INFO);
	config = config_create("/home/utnso/tp-2020-1c-NN/gamecard/src/gamecard.config");

	fs(config,blockSize,cantBlocks);


	ip = config_get_string_value(config,"IP_BROKER");
	puerto = config_get_string_value(config,"PUERTO_BROKER");

	ipGamecard = config_get_string_value(config,"IP_GAMECARD");
	puertoGamecard = config_get_string_value(config,"PUERTO_GAMECARD");

	montajePokemon=string_new();
	montajeBlock=string_new();
	string_append(&montajePokemon, config_get_string_value(config,"PUNTO_MONTAJE_TALLGRASS"));
	string_append(&montajePokemon,"/Files/");

	string_append(&montajeBlock,config_get_string_value(config,"PUNTO_MONTAJE_TALLGRASS"));
	string_append(&montajeBlock,"/Blocks");

	tiempoReconexion = config_get_int_value(config,"TIEMPO_DE_REINTENTO_CONEXION");

	tiempoDeReintento = config_get_int_value(config,"TIEMPO_DE_REINTENTO_OPERACION");

	tiempoDeRetardo = config_get_int_value(config,"TIEMPO_RETARDO_OPERACION");


	log_info(logger,"Lei la IP %s y puerto %s", ip, puerto);


	pthread_mutex_init(&mxArchivo, NULL);

//HILOS DE CONEXIONES

	pthread_t conexionGet;
	pthread_create(&conexionGet, NULL,(void*)funcionGet,NULL);

	pthread_t conexionNew;
	pthread_create(&conexionNew, NULL,(void*)funcionNew,NULL);

	pthread_t conexionCatch;
	pthread_create(&conexionCatch, NULL,(void*)funcionCatch, NULL);

	pthread_t conexionGameboy;
	pthread_create(&conexionGameboy, NULL,(void*) conexion_gameboy, NULL);

	pthread_join(conexionGameboy,NULL);
	pthread_join(conexionGet,NULL);
	pthread_join(conexionNew,NULL);
	pthread_join(conexionCatch,NULL);

	pthread_exit(&conexionGet);
	pthread_exit(&conexionNew);
	pthread_exit(&conexionCatch);
	pthread_exit(&conexionGameboy);

	pthread_exit(NULL);

	config_destroy(config);

	return EXIT_SUCCESS;
}
//PARA EL GET
void buscarPokemon(t_mensaje* mensaje){

	FILE * f;
	t_buffer* buffer;

	if(existePokemon(mensaje)){

		char** arrayBloques = agarrarBlocks(mensaje);

		char* montaje = montarPoke(mensaje);
		string_append(&montaje, "/Metadata.bin");
		esperaOpen(montaje);

		char* datosArchivoSinFormato = string_new();

		int i = 0;
		//Con este while recorro cada .bin
		while(arrayBloques[i] != NULL){
			char* montajeBlocks=montarBlocks(arrayBloques,i);
			f= fopen(montajeBlocks, "r");

			if(f == NULL){
	            log_info(logger,"El bloque %s no existe", arrayBloques[i]);
	        }else{
	        	int c;
	        	do {
	        	      c = fgetc(f);
	        	      if( feof(f) ) {
	        	         break ;
	        	      }
	        	      string_append_with_format(&datosArchivoSinFormato,"%c",c);
	        	   } while(1);
	        	//esperar por consigna antes de cerrar archivo
	        	sleep(tiempoDeRetardo);
	        	fclose(f);
	        }
	        free(montajeBlocks);
	        i++;
	        }

	    freeDoblePuntero(arrayBloques);


	    char** arrayDatos= string_split(datosArchivoSinFormato, "\n");

	    free(datosArchivoSinFormato);

	    int socketLoc = crear_conexion(ip, puerto);

	    if(socketLoc ==-1){
	    	log_info(logger,"No se pudo conectar con el broker.");
	    }else{

	    	t_mensaje_get* mensajeGet = malloc(sizeof(t_mensaje_get));
	    	mensajeGet->pokemon = mensaje->pokemon;
	    	mensajeGet->pokemon_length = mensaje->pokemon_length;
	    	char* posiciones = string_new();
	    	mensajeGet->id_mensaje = 0;
			mensajeGet->id_mensaje_correlativo = mensaje->id_mensaje;
			mensajeGet->cantidad = 0;
			int j = 0;

	    	while(arrayDatos[j] != NULL){
	    		char** partirPosicionesYCantidad;

	    		partirPosicionesYCantidad = string_split(arrayDatos[j],"=");

	    		mensajeGet->cantidad++;

	    		if(j==0){
	    			string_append(&posiciones,partirPosicionesYCantidad[0]);
	    		}else{
	    			string_append_with_format(&posiciones,".%s",partirPosicionesYCantidad[0]);
	    		}

	    		freeDoblePuntero(partirPosicionesYCantidad);
	    		j++;
	    	}

	    	freeDoblePuntero(arrayDatos);

	    	mensajeGet->posiciones_length = strlen(posiciones)+1;
	    	mensajeGet->posiciones = malloc(mensajeGet->posiciones_length);
	    	strcpy(mensajeGet->posiciones,posiciones);

	    	log_info(logger, "<GET> %s encontrado. Mandando mensaje LOCALIZED.",mensaje->pokemon);
	    	buffer = serializar_mensaje_struct_get(mensajeGet);
	    	enviar_mensaje_struct(buffer,socketLoc,LOCALIZED_POKEMON);
	    	free(posiciones);
	    	free(buffer->stream);
	    	free(buffer);

	    }
	    free(montaje);
	}else{

		int socketLoc = crear_conexion(ip, puerto);

		 if(socketLoc ==-1){
		   	log_info(logger,"No se pudo conectar con el broker.");
		}else{

			t_mensaje_get* mensajeGet = malloc(sizeof(t_mensaje_get));
			mensajeGet->pokemon = mensaje->pokemon;
			mensajeGet->pokemon_length = mensaje->pokemon_length;
			mensajeGet->posiciones =string_new();
			mensajeGet->posiciones_length = strlen(mensajeGet->posiciones)+1;
			mensajeGet->id_mensaje = 0;
			mensajeGet->id_mensaje_correlativo = mensaje->id_mensaje;
			mensajeGet->cantidad = 0;
	    	log_info(logger, "<GET> %s NO encontrado. Mandando mensaje LOCALIZED vacio.", mensaje->pokemon);
	    	log_info(logger,"<GET> Finaliza busqueda de %s.\n\n", mensaje->pokemon);
			buffer = serializar_mensaje_struct_get(mensajeGet);
			enviar_mensaje_struct(buffer,socketLoc,LOCALIZED_POKEMON);
			free(buffer->stream);
			free(buffer);
		}
	}
}


//TODO
//PARA EL CATCH
void agarrarPokemon(t_mensaje* mensaje){

char* montaje = montarPoke(mensaje);

string_append(&montaje,"/Metadata.bin" );
t_buffer* buffer;


int socketCaugth = crear_conexion(ip, puerto);
	if(socketCaugth ==-1){
		log_info(logger,"No se pudo conectar con el broker");
	}else{
		pthread_mutex_lock(&mxArchivo);
		if(existePokemon(mensaje)){
			log_info(logger,"<CATCH> %s encontrado! Analizando posiciones...", mensaje->pokemon);
			char* x=string_itoa(mensaje->posx);
			char* y=string_itoa(mensaje->posy);
			int existePos=0;
			int ultimoPoke=0;
			int ultimoPokePos=0;
			char* posicionMensaje= string_new();
			string_append(&posicionMensaje, x);
			string_append_with_format(&posicionMensaje, "-%s",y);

			esperaOpen(montaje);
			char* arrayTodo=guardarDatosBins(mensaje);
			log_info(logger,"<CATCH> Actualmente en los bloques hay: %s", arrayTodo);
			char** arrayBloques=agarrarBlocks(mensaje);

			char** datosSeparados=string_split(arrayTodo, "\n");
			free(arrayTodo);
			int j=0;

 			char* datosNuevos= string_new();

			while(datosSeparados[j]!=NULL){

				char** posiciones= string_split(datosSeparados[j], "=");

				ultimoPoke++;

				if(strcmp(posicionMensaje,posiciones[0])==0){
					//verificador q existe la pos
					existePos=1;
					if(atoi(posiciones[1])==1){
						//no hago nada xd
						ultimoPokePos++;
					}else{
						char* cantidad = string_itoa(atoi(posiciones[1])-1);
						char*nuevoDato = string_new();
						string_append(&nuevoDato, posiciones[0]);
						string_append_with_format(&nuevoDato,"=%s\n",cantidad);

						string_append(&datosNuevos, nuevoDato);
						free(cantidad);
						free(nuevoDato);
					}
				}else{
					string_append_with_format(&datosNuevos,"%s\n" ,datosSeparados[j]);
				}
				freeDoblePuntero(posiciones);
				j++;
			}

			free(posicionMensaje);
			free(x);
			free(y);


//si no existe informa error
			if(existePos==0){
				log_info(logger,"<CATCH> ERROR. No existe la posicion %d-%d.",mensaje->posx, mensaje->posy);
				//manda a broker q no existe.
				mensaje->resultado= 0;
				mensaje->id_mensaje_correlativo= mensaje->id_mensaje;
				buffer = serializar_mensaje_struct(mensaje);
				enviar_mensaje_struct(buffer, socketCaugth, CAUGHT_POKEMON);
				free(buffer->stream);
				free(buffer);
				free(montaje);
				free(datosNuevos);
			}else{
				log_info(logger,"<CATCH> Posicion %d-%d encontrada!", mensaje->posx, mensaje->posy);
				FILE* f;
				char* montajeBlocks = string_new();
				char **listaBloquesUsados;
				if(ultimoPoke==1 && ultimoPokePos==1){
					string_append(&montajeBlocks,montajeBlock);
					string_append_with_format(&montajeBlocks,"/%s.bin",arrayBloques[0]);

					f =fopen(montajeBlocks, "w");
					fclose(f);
					bitarray_clean_bit(bitmap,atoi(arrayBloques[0]) );

					char* basura=string_new();
					string_append(&basura, ",");

					listaBloquesUsados= string_split(basura, ",");

				}else{
					off_t offsetVacio = primerBloqueDisponible();

					string_append(&montajeBlocks,montajeBlock);
					string_append_with_format(&montajeBlocks,"/%d.bin",offsetVacio);
					f = fopen(montajeBlocks,"w");
					listaBloquesUsados = agregarBloquesAPartirDeString(datosNuevos,f,offsetVacio);

				}

				escrituraDeMeta(f,mensaje,listaBloquesUsados,montaje,datosNuevos);
				freeDoblePuntero(listaBloquesUsados);
				escribirBitmap();


				free(datosNuevos);
				free(montajeBlocks);
				//mandar CAUGTH
				mensaje->resultado= 1;
				mensaje->id_mensaje_correlativo= mensaje->id_mensaje;
				log_info(logger,"<CATCH> %s capturado!",mensaje->pokemon);
				buffer = serializar_mensaje_struct(mensaje);
				enviar_mensaje_struct(buffer, socketCaugth, CAUGHT_POKEMON);
				free(buffer->stream);
				free(buffer);
				}
			freeDoblePuntero(arrayBloques);
			freeDoblePuntero(datosSeparados);
		}else{
			log_info(logger,"<CATCH> ERROR. %s No existente.", mensaje->pokemon);
			//mandar a broker q no existe
			mensaje->resultado= 0;
			mensaje->id_mensaje_correlativo= mensaje->id_mensaje;
			buffer = serializar_mensaje_struct(mensaje);
			enviar_mensaje_struct(buffer, socketCaugth, CAUGHT_POKEMON);
			free(buffer->stream);
			free(buffer);
			free(montaje);
		}
	pthread_mutex_unlock(&mxArchivo);
	}
	log_info(logger,"<CATCH> Finalizo la busqueda y captura de pokemon.\n\n");
}


void conexion_gameboy(){
	int socket_gamecard = iniciar_servidor(ipGamecard,puertoGamecard);

	while(1){
		int socket_cliente = esperar_cliente(socket_gamecard);
		while(socket_cliente==-1)
			socket_cliente = esperar_cliente(socket_gamecard);
		socket_cliente=process_request(socket_cliente);
    }
}

void funcionNew(int socket){

    int conexionNew = crear_conexion(ip,puerto);
    while(1){

    	conexionNew = crear_conexion_broker(ID_PROCESO,conexionNew,ip,puerto,logger, tiempoReconexion, SUS_NEW);


        log_info(logger,"Me suscribi a la cola NEW!");

        while(conexionNew != -1){
        	conexionNew = process_request(conexionNew);

        }
    }
}

void funcionCatch(int socket){

    int conexionCatch =	crear_conexion(ip,puerto);
    while(1){

    	conexionCatch = crear_conexion_broker(ID_PROCESO,conexionCatch,ip,puerto,logger, tiempoReconexion, SUS_CATCH);

        log_info(logger,"Me suscribi a la cola CATCH!");

        while(conexionCatch != -1){
        	conexionCatch = process_request(conexionCatch);

        }
    }
}

void funcionGet(){
	int conexionGet = crear_conexion(ip,puerto);
	while(1){

		conexionGet = crear_conexion_broker(ID_PROCESO,conexionGet,ip,puerto,logger, tiempoReconexion, SUS_GET);

		log_info(logger,"Me suscribi a la cola GET!");

		while(conexionGet != -1){

			conexionGet = process_request(conexionGet);
		}

	}

}


void funcionACK(int id_mensaje){
	int conexionRespuesta;
	conexionRespuesta = crear_conexion(ip,puerto);
	char* ack = string_new();
	string_append_with_format(&ack,"%s-",ID_PROCESO);
	string_append_with_format(&ack,"%d",id_mensaje);
	enviar_mensaje(ack,conexionRespuesta,ACK);
	free(ack);
	liberar_conexion(conexionRespuesta);
}

int process_request(int socket){
	int cod_op;
	t_mensaje* mensaje;
	int recibidor = recv(socket, &cod_op, sizeof(op_code), MSG_WAITALL);

	if(recibidor == -1 || recibidor==0)
		cod_op = -1;

	switch (cod_op){
	case GET_POKEMON:
		mensaje = recibir_mensaje_struct(socket);
		funcionACK(mensaje->id_mensaje);
		log_info(logger,"Mensaje GET %s y confirme recepcion.\n", mensaje->pokemon);
		log_info(logger,"<GET> Iniciando busqueda de %s.",mensaje->pokemon);
		pthread_t solicitudGet;
		pthread_create(&solicitudGet, NULL,(void *) buscarPokemon, mensaje);
		pthread_detach(solicitudGet);
		return socket;
		break;
	case NEW_POKEMON:
		mensaje = recibir_mensaje_struct(socket);
		funcionACK(mensaje->id_mensaje);
		log_info(logger,"Mensaje NEW %s y confirme recepcion.\n", mensaje->pokemon);
		log_info(logger,"<NEW> Creando nuevo %s...",mensaje->pokemon);
		pthread_t solicitudNew;
		pthread_create(&solicitudNew, NULL,(void *) nuevoPokemon, mensaje);
		pthread_detach(solicitudNew);
		//free(mensaje->pokemon);
		//free(mensaje->resultado);
		//free(mensaje);
		return socket;
		break;
	case CATCH_POKEMON:

		mensaje = recibir_mensaje_struct(socket);
		funcionACK(mensaje->id_mensaje);
		log_info(logger,"Mensaje CATCH %s y confirme su recepcion.\n", mensaje->pokemon);
		log_info(logger,"<CATCH> Buscando %s para agarrar...",mensaje->pokemon);
		pthread_t solicitud;
		pthread_create(&solicitud, NULL,(void*) agarrarPokemon, mensaje);
		pthread_detach(solicitud);
		//free(mensaje->pokemon);
		//free(mensaje->resultado);
		return socket;

		break;
	case ACK:
		log_info(logger,"hola");
		break;
	case -1:
		log_info(logger,"Codigo de operacion invalido, iniciando servidor gamecard");

		socket = crear_conexion(ip, puerto);
		return socket;
		break;
	default:
		log_info(logger,"Que paso rey, te caiste? O quiza...");

		socket = crear_conexion(ip, puerto);
		return socket;
		break;
	}
	return -1;
}

void nuevoPokemon(t_mensaje* mensaje){

	FILE * f;
	t_buffer* buffer;
	char* montaje= montarPoke(mensaje);
	pthread_mutex_lock(&mxArchivo);
	f = fopen(montaje,"r");

	//Si el fopen devuelve NULL significa que el archivo no existe entonces creamos uno de 0
	if(f==NULL){
		log_info(logger, "<NEW> Nuevo %s, creando directorio y metadata.", mensaje->pokemon);
		asignarBloqueYcrearMeta(mensaje,montaje);
	}
	pthread_mutex_unlock(&mxArchivo);
	if(f!=NULL){
		log_info(logger,"<NEW> %s ya existente!",mensaje->pokemon);
			fclose(f);
			string_append(&montaje,"/Metadata.bin");
			f = fopen(montaje,"r");
			cambiar_meta_blocks(montaje,mensaje);
			fclose(f);

	}

	int socketAppeared = crear_conexion(ip, puerto);

	if(socketAppeared ==-1){

		log_info(logger,"No se pudo conectar con el broker.");
	}else{
		mensaje->id_mensaje_correlativo = mensaje->id_mensaje;
		log_info(logger,"<NEW> %s añadido al File System.\n\n", mensaje->pokemon);
		buffer = serializar_mensaje_struct(mensaje);
		enviar_mensaje_struct(buffer, socketAppeared,APPEARED_POKEMON);
		free(buffer->stream);
		free(buffer);
	}
}

//-------------------------------------------------------------------------------------
off_t primerBloqueDisponible(){
	off_t offset = 1;

	while(bitarray_test_bit(bitmap,offset)){
		offset++;
	}
	if(offset>cantBlocks)
		log_info(logger,"todos los bloques estan llenos");

	return offset;
}

char* montarPoke(t_mensaje* mensaje){
	char* montaje= string_new();
	string_append(&montaje,montajePokemon);
	string_append(&montaje,mensaje->pokemon);

	return montaje;
}


void asignarBloqueYcrearMeta(t_mensaje* mensaje,char* montaje){

	FILE* f;
	off_t offset;
	char* bloques = string_new();
	char* escribirBloque = string_new();

	char* x = string_itoa(mensaje->posx);
	char* y = string_itoa(mensaje->posy);
	char* cant = string_itoa(mensaje->cantidad);

	offset = primerBloqueDisponible();

	string_append(&bloques,montajeBlock);
	string_append_with_format(&bloques,"/%d.bin",offset);
	//log_info(logger,"El offset es %d",offset);

	f=fopen(bloques,"w");

	char** listaBloquesUsados;

	if(f!=NULL){
//Bloque
		//En escribirBloque se guarda formato 1-2=3
		string_append(&escribirBloque, x);
		string_append_with_format(&escribirBloque, "-%s",y);
		string_append_with_format(&escribirBloque, "=%s\n",cant);

		listaBloquesUsados = agregarBloquesAPartirDeString(escribirBloque,f,offset);
	}
//Metadata

	mkdir(montaje,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	string_append(&montaje,"/Metadata.bin");


	escrituraDeMeta(f,mensaje,listaBloquesUsados,montaje,escribirBloque);

	escribirBitmap();

	free(bloques);
	free(escribirBloque);
	free(x);
	free(y);
	free(cant);

	freeDoblePuntero(listaBloquesUsados);

}

char** agregarBloquesAPartirDeString(char* escribirBloque,FILE* f,off_t offset){

	off_t desplazamiento = 0;
	char** bloquesUsados;
	char* contadores = string_new();
	//log_info(logger,contadores);
	char* loQueNoEntraEnUnBloque;
	char* loQueEntraEnUnBloque = string_substring_until(escribirBloque,blockSize);
	if(strlen(escribirBloque)>blockSize)
		loQueNoEntraEnUnBloque = string_substring_from(escribirBloque,blockSize);
	else
		loQueNoEntraEnUnBloque = string_new();
	int cantidadDeBloques = calcularCantidadDeBLoques(escribirBloque);
	char* cantidadDeBloquesString = string_itoa(cantidadDeBloques);
	//log_info(logger,cantidadDeBloquesString);
	if(cantidadDeBloques==0){
		cantidadDeBloques++;
	}else{
		string_append_with_format(&contadores,"%d",offset);
	}
	int i = 1;

	fprintf(f,"%s",loQueEntraEnUnBloque);
	fclose(f);
	free(loQueEntraEnUnBloque);
	char* copia = malloc(strlen(loQueNoEntraEnUnBloque)+1);
	memcpy(copia,loQueNoEntraEnUnBloque,strlen(loQueNoEntraEnUnBloque)+1);
	free(loQueNoEntraEnUnBloque);
	bitarray_set_bit(bitmap,offset);

	while(i != cantidadDeBloques){

		char * loQueEntraEnUnBloque = string_substring_until(copia,blockSize);
		if(strlen(copia)>blockSize){
			char* loQueNoEntraEnUnBloque = string_substring_from(copia,blockSize);
			copia = realloc(copia,strlen(loQueNoEntraEnUnBloque)+1);
			memcpy(copia,loQueNoEntraEnUnBloque,strlen(loQueNoEntraEnUnBloque)+1);
			free(loQueNoEntraEnUnBloque);
		}

		desplazamiento = primerBloqueDisponible();
		string_append_with_format(&contadores,",%d",desplazamiento);
		//log_info(logger,contadores);
		char* blocks = string_new();
		string_append(&blocks,montajeBlock);
		string_append_with_format(&blocks,"/%d.bin",desplazamiento);


		log_info(logger,"Escribiendo en bloque: %d.", desplazamiento);

		f=fopen(blocks,"w");
		fprintf(f,"%s",loQueEntraEnUnBloque);
		fclose(f);
		free(loQueEntraEnUnBloque);
		bitarray_set_bit(bitmap,desplazamiento);

		i++;

		free(blocks);
	}
	bloquesUsados = string_split(contadores,",");

	free(contadores);
	free(copia);
	free(cantidadDeBloquesString);

	return bloquesUsados;
}

int calcularCantidadDeBLoques(char* escribirBloque){
	int cantidadDeBloques;
	int cant = strlen(escribirBloque)/blockSize;
	int resto = strlen(escribirBloque)%blockSize;

	if(resto != 0)
		cantidadDeBloques = cant+1;
	else
		cantidadDeBloques = cant;

	return cantidadDeBloques;
}

void recrearBlock(FILE* fblocks,char** blocksRenovados,char* montajeBlocks){
	fblocks= fopen(montajeBlocks, "w+");
	int i = 0;
	while(blocksRenovados[i] != NULL){
		fprintf(fblocks,"%s\n",blocksRenovados[i]);
		i++;
	}
	fclose(fblocks);
	free(montajeBlocks);
	sleep(tiempoDeRetardo);
}

void escrituraDeMeta(FILE* f,t_mensaje* mensaje,char** listaBloquesUsados,char* montaje,char* bloquesActualizados){
	f = fopen(montaje,"w+");
	escribirMeta(f,mensaje,listaBloquesUsados,bloquesActualizados);
	fprintf(f,"OPEN=Y");
	fclose(f);
	log_info(logger,"Espera de:%d segundos.",tiempoDeRetardo);
	sleep(tiempoDeRetardo);
	f=fopen(montaje,"w+");
	escribirMeta(f,mensaje,listaBloquesUsados,bloquesActualizados);
	fprintf(f,"OPEN=N");
	fclose(f);
	log_info(logger,"Termino de esperar");

	free(montaje);
}

void escribirMeta(FILE* f,t_mensaje* mensaje,char** lista,char* bloquesActualizados){

	char* blocks = string_new();
	int i=0;
	int lengthBlocks = string_length(bloquesActualizados);
	if(lista[i]!=NULL){
		string_append(&blocks,lista[i]);
		i++;
		while(lista[i] != NULL){
			string_append_with_format(&blocks,",%s",lista[i]);
			i++;
		}
	}


	fprintf(f,"DIRECTORY=N\n");
	fprintf(f,"SIZE=%d\n",lengthBlocks);
	fprintf(f,"BLOCKS=[%s]\n",blocks);

	free(blocks);
}

void cambiar_meta_blocks(char* montaje,t_mensaje* mensaje){
    FILE* fblocks;

    char* datosBins;
    char** listaBloquesUsados;
    char* bloquesActualizados;

    pthread_mutex_lock(&mxArchivo);
    esperaOpen(montaje);

    datosBins = guardarDatosBins(mensaje);

    log_info(logger,"<NEW> Actualmente en los bloques hay: %s", datosBins);


    log_info(logger,"<NEW> Verifiando posiciones.");
    bloquesActualizados = verificarCoincidenciasYsumarCantidad(datosBins,mensaje);

    off_t offsetVacio = primerBloqueDisponible();

    char* montajeBlocks = string_new();
    string_append(&montajeBlocks,montajeBlock);
    string_append_with_format(&montajeBlocks,"/%d.bin",offsetVacio);
    fblocks = fopen(montajeBlocks,"w");
    listaBloquesUsados = agregarBloquesAPartirDeString(bloquesActualizados,fblocks,offsetVacio);
    log_info(logger,"<NEW> Reescribiendo bloques y metadata.");
    escrituraDeMeta(fblocks,mensaje,listaBloquesUsados,montaje,bloquesActualizados);
    escribirBitmap();
    pthread_mutex_unlock(&mxArchivo);

    free(datosBins);
    free(bloquesActualizados);
    free(montajeBlocks);

    freeDoblePuntero(listaBloquesUsados);


}

char* verificarCoincidenciasYsumarCantidad(char* datosBins, t_mensaje* mensaje){

    char* mensajePosiciones = string_new();

	char* x = string_itoa(mensaje->posx);
	char* y = string_itoa(mensaje->posy);
	char* cant = string_itoa(mensaje->cantidad);

	string_append(&mensajePosiciones, x);
	string_append_with_format(&mensajePosiciones, "-%s",y);

	char** bloques = string_split(datosBins,"\n");
	char* bloquesActualizados = string_new();

	int i = 0;
	int contadorDeCoincidencias = 0;
	while(bloques[i]!=NULL){
		char** posicionDeBloque;
		posicionDeBloque = string_split(bloques[i],"=");
		if(strcmp(posicionDeBloque[0],mensajePosiciones) == 0){
			char** dividirPosicionCantidad = string_split(bloques[i],"=");
			int cantidadActualizada = mensaje->cantidad + atoi(dividirPosicionCantidad[1]);
			char* cantidadActualizadaString = string_itoa(cantidadActualizada);
			string_append_with_format(&mensajePosiciones, "=%s",cantidadActualizadaString);//Aca el mensajePosicones equivaldria a las posiciones con la cantidad actualizada
			string_append_with_format(&bloquesActualizados,"%s\n",mensajePosiciones);
			contadorDeCoincidencias++;
			freeDoblePuntero(dividirPosicionCantidad);
			free(cantidadActualizadaString);
		}else{
			string_append_with_format(&bloquesActualizados,"%s\n",bloques[i]);
		}
		i++;
		freeDoblePuntero(posicionDeBloque);
	}

	if(contadorDeCoincidencias == 0){
		string_append_with_format(&mensajePosiciones, "=%s",cant);//Aca el mensajePosicones equivaldria al nuevo mensaje que agregamos al final del char* bloquesActualizados
		string_append_with_format(&bloquesActualizados,"%s\n",mensajePosiciones);
	}

	free(mensajePosiciones);
	free(x);
	free(y);
	free(cant);

	freeDoblePuntero(bloques);

	return bloquesActualizados;
}

char* guardarDatosBins(t_mensaje* mensaje){
	char* datosBins = string_new();
	FILE* fblocks;
    int i = 0;
    char** arrayBloques = agarrarBlocks(mensaje);

    //Con este while recorro cada .bin
    while(arrayBloques[i] != NULL){
    	char* montajeBlocks = montarBlocks(arrayBloques, i);
    	fblocks= fopen(montajeBlocks, "r");
    	//Si fblocks es igual a NULL significa que el .bin que trato de abrir no esta en la carpeta Blocks
    	if(fblocks==NULL){
    		log_info(logger,"El bloque %s no existe", arrayBloques[i]);
    	}else{
    		int c;
    		do {
    			c = fgetc(fblocks);
    			if( feof(fblocks) ) {
    				break ;
    			}
    			string_append_with_format(&datosBins,"%c",c);
    		} while(1);

    		fclose(fblocks);

    		off_t offset = atoi(arrayBloques[i]);

    		bitarray_clean_bit(bitmap, offset);

    		//log_info(logger,datosBins);
    	}
    	i++;
    	free(montajeBlocks);
    }

    freeDoblePuntero(arrayBloques);

    return datosBins;
}

//FUNCIONES AUXILIARES, MAS VALE QUE SE USAN PA, TE EMBELLECEN TODO EL CODIGO

char* montarBlocks(char** arrayBloques, int i){

	char* montajeBlocks = string_new();
	string_append(&montajeBlocks,montajeBlock);
	string_append_with_format(&montajeBlocks,"/%s.bin",arrayBloques[i]);

	return montajeBlocks;
}


char** agarrarBlocks(t_mensaje* mensaje){

	char* montaje = string_new();
	string_append(&montaje,montajePokemon);
	string_append_with_format(&montaje,"%s/Metadata.bin",mensaje->pokemon);

	char* bloques;
	t_config* configBloques = config_create(montaje);
	bloques = config_get_string_value(configBloques,"BLOCKS");


	if(strcmp(bloques,"[]")!=0){
		char**basura = string_split(bloques,"[");
		char**nroBloque = string_split(basura[0],"]"); //En el nroBloques[0] se guardan los bin de esta forma "1,2,3"
		char**arrayBloques = string_split(nroBloque[0],","); //En la variable arrayBloques se guarda un array de tipo char** de cada .bin


		freeDoblePuntero(basura);
		freeDoblePuntero(nroBloque);
		config_destroy(configBloques);
		free(montaje);
		return arrayBloques;
	}else{
		char** arrayBloques= string_split("",",");
		config_destroy(configBloques);
		free(montaje);
		return arrayBloques;
	}


}

void esperaOpen(char* montaje){
	char * valorOpen;
	t_config* configBloques;

	configBloques=config_create(montaje);

	valorOpen = config_get_string_value(configBloques,"OPEN");
	//reintento volver a abrir
	while(strcmp(valorOpen,"Y") == 0){
		log_info(logger,"Estoy esperando");
		//log_info(logger,"%s",valorOpen);
		sleep(tiempoDeReintento);

		config_destroy(configBloques);
	    configBloques = config_create(montaje);

		valorOpen = config_get_string_value(configBloques,"OPEN");
	}
	config_destroy(configBloques);
}

int existePokemon(t_mensaje* mensaje){
	FILE* fp;

	char* montajePoke = string_new();

	string_append(&montajePoke, montajePokemon);
	string_append(&montajePoke, mensaje->pokemon);

	fp= fopen(montajePoke, "r");
	if(fp==NULL){
		free(montajePoke);
		return 0;
	}else{
		fclose(fp);
		free(montajePoke);
		return 1;
	}

}

void escribirBitmap(){

    char* datosBitmap= string_new();
t_config* config=config_create("/home/utnso/tp-2020-1c-NN/gamecard/src/gamecard.config");

    for(off_t i=1; i<cantBlocks+1;i++){
        if(bitarray_test_bit(bitmap, i))
            string_append(&datosBitmap, "1");
        else
            string_append(&datosBitmap, "0");
    }

    FILE* f;
    char* mnt = string_new();
    string_append(&mnt,config_get_string_value(config,"PUNTO_MONTAJE_TALLGRASS"));
    string_append(&mnt, "/Metadata/Bitmap.bin");

    f=fopen(mnt,"w+");

    fprintf(f,"%s", datosBitmap);
    fclose(f);
    free(datosBitmap);
    free(mnt);
    config_destroy(config);

}

void escribirBitmapInicio(){

    char* datosBitmap= string_new();
t_config* config=config_create("/home/utnso/tp-2020-1c-NN/gamecard/src/gamecard.config");

    for(off_t i=1; i<cantBlocks+1;i++){
        if(bitarray_test_bit(bitmap, i)){
        	bitarray_clean_bit(bitmap, i);
            string_append(&datosBitmap, "0");
        }else
            string_append(&datosBitmap, "0");
    }

    FILE* f;
    char* mnt = string_new();
    string_append(&mnt,config_get_string_value(config,"PUNTO_MONTAJE_TALLGRASS"));
    string_append(&mnt, "/Metadata/Bitmap.bin");

    f=fopen(mnt,"w+");

    fprintf(f,"%s", datosBitmap);
    fclose(f);
    free(datosBitmap);
    free(mnt);
    config_destroy(config);

}


