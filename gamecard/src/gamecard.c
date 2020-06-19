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

	free(mntPokemon);
	free(mntBlocks);
	config_destroy(config);

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
		free(mensaje->pokemon);
		free(mensaje->resultado);
		free(mensaje);
		break;
	case NEW_POKEMON:
		mensaje = recibir_mensaje_struct(socket);
		funcionACK();
		log_info(logger,"Recibi mensaje de contenido pokemon %s y envie confirmacion de su recepcion",mensaje->pokemon);
		pthread_t solicitudNew;
		pthread_create(&solicitudNew, NULL,(void *) nuevoPokemon, mensaje);
		pthread_detach(solicitudNew);
		free(mensaje->pokemon);
		free(mensaje->resultado);
		free(mensaje);
		break;
	case CATCH_POKEMON:
		mensaje = recibir_mensaje_struct(socket);
		funcionACK();
		log_info(logger,"Recibi mensaje de contenido pokemon %s y envie confirmacion de su recepcion",mensaje->pokemon);
		/*pthread_t solicitud;
		pthread_create(&solicitud, NULL,(void*) agarrarPokemon,(gamecard*) mensaje);
		pthread_detach(solicitud);*/
		free(mensaje->pokemon);
		free(mensaje->resultado);
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

void nuevoPokemon(t_mensaje* mensaje){

	FILE * f;

	char* montaje= string_new();
	string_append(&montaje,mntPokemon);
	string_append(&montaje,mensaje->pokemon);
	pthread_mutex_lock(&mxArchivo);
	f = fopen(montaje,"r");

	//Si el fopen devuelve NULL significa que el archivo no existe entonces creamos uno de 0
	if(f==NULL){

		crearBloques(mensaje);

		mkdir(montaje,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		string_append(&montaje,"/Metadata.bin");
		f=fopen(montaje,"w+");
		escribirMeta(f,mensaje);
		contadorBloques++;
		fclose(f);

		}else{
			fclose(f);
			string_append(&montaje,"/Metadata.bin");
			f = fopen(montaje,"r");
			cambiar_meta_blocks(montaje,mensaje);
			fclose(f);

		}

	pthread_mutex_unlock(&mxArchivo);

	/*int socketLoc = crear_conexion(ip, puerto);

		if(socketLoc ==-1){
				log_info(logger,"No se pudo conectar con el broker.");
				//log_info(logger,"%s",datoArchivo);
		}else{
				enviar_mensaje_struct(mensaje, socketLoc,LOCALIZED_POKEMON);
		}*/

	free(f);
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

	free(f);
	free(contador);
	free(bloques);
	free(escribirBloque);
	free(x);
	free(y);
	free(cant);

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
}


void escribirMeta(FILE* f,t_mensaje* mensaje){
	fprintf(f,"DIRECTORY=N\n");
	fprintf(f,"SIZE=%d\n",24);
	fprintf(f,"BLOCKS=[%d]\n",contadorBloques);
	fprintf(f,"OPEN=N");
}


void cambiar_meta_blocks(char* montaje,t_mensaje* mensaje){
    FILE* fblocks;
    char* bloques;
    t_block* block;
    t_config* configBloques;

    char** arrayBloques;
    char** basura;
    char** nroBloque;
    configBloques = config_create(montaje);
    bloques=config_get_string_value(configBloques,"BLOCKS");

    int size= atoi(config_get_string_value(configBloques,"SIZE"));


    basura=string_split(bloques,"[");
    nroBloque=string_split(basura[0],"]"); //En el nroBloques[0] se guardan los bin de esta forma "1,2,3"
    arrayBloques=string_split(nroBloque[0],","); //En la variable arrayBloques se guarda un array de tipo char** de cada .bin
    log_info(logger,"%s", arrayBloques[0]);

    int verificador= 0;
    int i = 0;
    //Con este while recorro cada .bin
    while(arrayBloques[i] != NULL){
        char* montajeBlocks=string_new();
        string_append(&montajeBlocks,mntBlocks);
        string_append_with_format(&montajeBlocks,"/%s.bin",arrayBloques[i]);
        fblocks= fopen(montajeBlocks, "r");

        //Si fblocks es igual a NULL significa que el .bin que trato de abrir no esta en la carpeta Blocks
        if(fblocks == NULL){
            log_info(logger,"El bloque %s no existe", arrayBloques[i]);
        }else{
        	//Si fblocks es direfente de NULL significa que existe el .bin dentro de nuesta carpeta Blocks
            block = buscarCoincidencias(fblocks, mensaje);
            char* mensajePosiciones = string_new();
            string_append(&mensajePosiciones,string_itoa(mensaje->posx));
            string_append_with_format(&mensajePosiciones,"-%s",string_itoa(mensaje->posy));
            string_append_with_format(&mensajePosiciones,"=%s",string_itoa(mensaje->cantidad));
            //Este If() entra si hay alguna coincidencia de las posiciones del mensaje con las que estan en el bin
            if(strcmp(block->blockARenovar,mensajePosiciones) != 0){

            	verificarTamBlockYActualizarlo(size,block,fblocks,montajeBlocks,montaje,nroBloque);

                verificador++;
            	break;
            }
            free(mensajePosiciones);
        }
        i++;
    }

    if(verificador==0){
    	//char* montajeUltimoBlock = string_new();
    	//string_append(&montajeUltimoBlock,mntBlocks);
    	//string_append_with_format(&montajeUltimoBlock,"/%s.bin",arrayBloques[i-1]);
    	//verificarTamBlock(size,block,fblocks,montajeBlocks,montaje,nroBloque);

		//Aca se busca si ya existe un .bin en donde entre la posicion del block->blockARenovar
		//En el caso en el que exista y con ese .bin no exceda el size del block, se lo agrega al final de ese .bin
		//En el caso en el que no exista, se crea un nuevo .bin en el que se lo agrega
    	buscarBinEnDondeEntreElBlockARenovarYRenovarlo(nroBloque,size,fblocks,block,montaje);
    }
    free(bloques);
    free(arrayBloques);
    free(basura);
    config_destroy(config);
}


t_block* buscarCoincidencias(FILE* fblocks, t_mensaje* mensaje){
    t_block* block = malloc(sizeof(t_block));
    int i;
    int contadorDeCoincidencias = 0; //Si aumenta esto, significa que el pokemon que mandamos en mensaje coincide con alguno de los que teniamos en el .bin
    char* blockAComparar = string_new(); //Aca se va a guardar cada linea de texto del .bin que estemos recorriendo
    char* cantidadDePokemon = string_new();
    char** posicionesYCantidad;
    char* mensajePosiciones = string_new(); //Con esto convertimos las posiciones del mensaje recibido por parametro en un string de tipo "1-1"
    char* blockARenovar = string_new(); //Aca se va a guardar la linea de texto(solo las posiciones) del .bin que estemos recorriendo que se va a tener que cambiar(en el caso que encuentre coincidencias)
    char** blocksRenovados;             //o agregar(en el caso en el que no encuentre coincidencias) dentro de ese .bin
    char* basura = string_new(); //En basura se guardan las lineas de texto del .bin y la linea de texto que hay que renovar, separado por espacios
    string_append(&mensajePosiciones,string_itoa(mensaje->posx));
    string_append_with_format(&mensajePosiciones,"-%s",string_itoa(mensaje->posy));

    fscanf(fblocks," %[^\n]",blockAComparar);
    while(!feof(fblocks)){
        posicionesYCantidad = string_split(blockAComparar,"="); //En posicioneYCantidad[0] se guardan solo las posiciones x e y
        if(strcmp(posicionesYCantidad[0], mensajePosiciones) == 0){
            int cant = atoi(posicionesYCantidad[1]); //En posicionesYCantidad[1] se guarda la cantidad de posicioneYCantidad[0]
            cant += mensaje->cantidad;
            cantidadDePokemon = string_itoa(cant);
            string_append(&blockARenovar,mensajePosiciones);
            string_append_with_format(&blockARenovar,"=%s",cantidadDePokemon);
            log_info(logger,"%s",blockARenovar);
            string_append_with_format(&basura,"%s ",blockARenovar);
            contadorDeCoincidencias++;
        }else{
            string_append_with_format(&basura,"%s ",blockAComparar);
        }
        i++;
        fscanf(fblocks," %[^\n]",blockAComparar);
    }
    fclose(fblocks);
    //Si el contadorDeCoindencias es 0 entonces vamos a agregar el blockARenovar junto con la cantidad pasada por t_mensaje* mensaje al char* basura
    if(contadorDeCoincidencias == 0){
        string_append(&blockARenovar,mensajePosiciones);
        string_append_with_format(&blockARenovar,"=%s",string_itoa(mensaje->cantidad));
        string_append_with_format(&basura,"%s",blockARenovar);
    }
    blocksRenovados = string_split(basura," "); //En blocksRenovador guardamos un array de tipo char** de cada linea de texto del .bin con el blockARenovar incluido
    block->blocksRenovados = blocksRenovados;
    block->blockARenovar = blockARenovar;

    free(blockAComparar);
    free(cantidadDePokemon);
    free(posicionesYCantidad);
    free(mensajePosiciones);
    free(basura);
    return block;
}



void verificarTamBlockYActualizarlo(int size,t_block* block, FILE* fblocks, char* montajeBlocks, char* montaje, char** nroBloque){

	int i=0;
	int tamTotal=0; //Es el tamaño total de nuestro .bin, con el blockARenovar incluido
	while(block->blocksRenovados[i]!=NULL){
		tamTotal+= strlen(block->blocksRenovados[i]) +1;
		i++;
	}

	//Si el tamTotal es <= size significa que el blockARenovar se puede agregar a ese .bin
	//En caso contrario, se debe crear un .bin nuevo, que es lo que se hace en el else
	if(tamTotal <= size){

		recrearBlock(fblocks,block->blocksRenovados,montajeBlocks);
		free(fblocks);
		free(block->blocksRenovados);
		free(block->blockARenovar);
		free(block);

	}else{
//////////Si saco lo de abajo queda como antes-----------------------------------------------------------------------------------------------------
		char** copiaBlocks;
		//copiarBlocksMenosElQueSuperaElSize() saca del .bin la posicion que excede los bytes del size y devuelve las otras posiciones que estaban en ese .bin
		copiaBlocks = copiarBlocksMenosElQueSuperaElSize(fblocks,montajeBlocks,block);

		//Aca se recrea ese .bin sin la posicion que excede los bytes del size
		recrearBlock(fblocks,copiaBlocks,montajeBlocks);

		//Aca se busca si ya existe un .bin en donde entre la posicion anteriormente excluida
		//En el caso en el que exista y con ese .bin no exceda el size del block, se lo agrega al final de ese .bin
		//En el caso en el que no exista, se crea un nuevo .bin en el que se lo agrega
		buscarBinEnDondeEntreElBlockARenovarYRenovarlo(nroBloque,size,fblocks,block,montaje);
//////////Si saco lo de arriba queda como antes----------------------------------------------------------------------------------------------------

		/*char* contador=string_itoa(contadorBloques);
		char* bloques= string_new();

		string_append(&bloques,"/home/utnso/Escritorio/TALL_GRASS/Blocks/");
		string_append_with_format(&bloques,"%s.bin",contador);

		fblocks=fopen(bloques,"w+");

		fprintf(fblocks,"%s\n",block->blockARenovar);
		fclose(fblocks);

		reescribirMeta(montaje, contador,nroBloque);

		contadorBloques++;*/
		free(copiaBlocks);
	}


}

void reescribirMeta(char* montaje, char*nuevoBloque,char**nroBloque){
	FILE* fmeta;
	char* todo= string_new();

	string_append(&todo,nroBloque[0]);
	string_append_with_format(&todo,",%s",nuevoBloque);

	fmeta=fopen(montaje,"w+");


	log_info(logger,"%s",todo);
	fprintf(fmeta,"DIRECTORY=N\n");
	fprintf(fmeta,"SIZE=%d\n",24);
	fprintf(fmeta,"BLOCKS=[%s]\n",todo);
	fprintf(fmeta,"OPEN=N");

	fclose(fmeta);
	free(todo);
	free(montaje);
}

void buscarBinEnDondeEntreElBlockARenovarYRenovarlo(char** nroBloque,int size,FILE * fblocks,t_block* block, char* montaje){
	char** arrayBloques; //En la variable arrayBloques se guarda un array de tipo char** de cada .bin
	int encontroBinBool = 0; //Si aumenta esto, significa que existe un .bin en el que si agregamos el block->blockARenovar, no excede el size del block
	int i = 0;

	arrayBloques=string_split(nroBloque[0],",");
	while(arrayBloques[i] != NULL){
		char* montajeBlocks=string_new();
		string_append(&montajeBlocks,mntBlocks);
        string_append_with_format(&montajeBlocks,"/%s.bin",arrayBloques[i]);
        fblocks= fopen(montajeBlocks, "r");

        if(fblocks == NULL){
            log_info(logger,"El bloque %s no existe", arrayBloques[i]);
        }else{
        	char* blockAGuardar = string_new();
        	char* basura = string_new();
        	char** blockTotal;
        	fscanf(fblocks," %[^\n]",blockAGuardar);

        	while(!feof(fblocks)){
        		string_append_with_format(&basura,"%s ",blockAGuardar);
        		fscanf(fblocks," %[^\n]",blockAGuardar);
        	}
        	string_append_with_format(&basura,"%s ",block->blockARenovar);
        	blockTotal = string_split(basura," ");
        	fclose(fblocks);

        	int j=0;
        	int tamTotal=0;

        	while(blockTotal[j]!=NULL){
        		tamTotal+= strlen(blockTotal[j]) +1;
        		j++;
        	}

        	if(tamTotal <= size){
        		recrearBlock(fblocks,blockTotal,montajeBlocks);
        		encontroBinBool++;
        		break;
        	}
        	free(blockAGuardar);
        	free(basura);
        	free(blockTotal);
        }
        i++;
        free(montajeBlocks);
	}
	if(encontroBinBool == 0){
		char* contador=string_itoa(contadorBloques);
		char* bloques= string_new();

		string_append(&bloques,"/home/utnso/Escritorio/TALL_GRASS/Blocks/");
		string_append_with_format(&bloques,"%s.bin",contador);

		fblocks=fopen(bloques,"w+");

		fprintf(fblocks,"%s\n",block->blockARenovar);
		fclose(fblocks);

		reescribirMeta(montaje, contador,nroBloque);

		contadorBloques++;

		free(contador);
		free(bloques);
	}
	free(arrayBloques);
	free(nroBloque);
	free(fblocks);
	free(block->blocksRenovados);
	free(block->blockARenovar);
	free(block);
}

char** copiarBlocksMenosElQueSuperaElSize(FILE * fblocks, char* montajeBlocks,t_block* block){
	char** copiaBlocks;
	char* basura = string_new();
	char* blockACopiar = string_new();
	char** posicionesYCantidadDeBlockACopiar;
	char** posicionesYCantidadDeBlockQueNoQueremosCopiar;

	fblocks = fopen(montajeBlocks,"r");
	fscanf(fblocks," %[^\n]",blockACopiar);
	while(!feof(fblocks)){
		posicionesYCantidadDeBlockACopiar = string_split(blockACopiar,"=");
		posicionesYCantidadDeBlockQueNoQueremosCopiar = string_split(block->blockARenovar,"=");
		if(strcmp(posicionesYCantidadDeBlockACopiar[0],posicionesYCantidadDeBlockQueNoQueremosCopiar[0]) == 0){
			//Literal no tiene que hacer nada, asi no se copia a basura el que hay q renovar(que es el que hace superar el size)
		}else{
			string_append_with_format(&basura,"%s ",blockACopiar);
		}
		fscanf(fblocks," %[^\n]",blockACopiar);
	}
	fclose(fblocks);
	copiaBlocks = string_split(basura," ");

	free(basura);
	free(blockACopiar);
	free(posicionesYCantidadDeBlockACopiar);
	free(posicionesYCantidadDeBlockQueNoQueremosCopiar);

	return copiaBlocks;
}
