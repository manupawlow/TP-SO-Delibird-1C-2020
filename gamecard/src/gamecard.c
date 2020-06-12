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



	//ip = config_get_string_value(config,"IP_GAMECARD");
	//puerto = config_get_string_value(config,"PUERTO_GAMECARD");

	//iniciar_servidor(ip,puerto);


//HILOS DE CONEXIONES cambiar por mati
	pthread_t conexionGet;
	pthread_create(&conexionGet, NULL,(void*)funcionGet,NULL);

	pthread_t conexionNew;
	pthread_create(&conexionNew, NULL,(void*)funcionNew,NULL);


	pthread_t conexionCatch;
	pthread_create(&conexionCatch, NULL,(void*)funcionCatch, NULL);

	pthread_join(conexionGet,NULL);
	pthread_join(conexionGet,NULL);
	pthread_join(conexionCatch,NULL);

return EXIT_SUCCESS;
}

void funcionGet(){
char* mensaje;
int conexionGet =	crear_conexion(ip,puerto);

	if(conexionGet ==-1){
		log_info(logger,"Reintenando reconectar cada %d segundos",tiempoReconexion);
        conexionGet= reintentar_conexion(ip,puerto,tiempoReconexion);
	}

	enviar_mensaje("Suscribime",conexionGet, SUS_GET);
	log_info(logger,"Me suscribi a la cola GET!");


	while(1){
		mensaje=funcionACK(conexionGet);
		pthread_t solicitud;
		pthread_create(&solicitud, NULL,(void *)buscarPokemon,(gamecard*) mensaje);
		pthread_detach(solicitud);
	}

}


void funcionCatch(int socket){
char* mensaje;

int conexionCatch =	crear_conexion(ip,puerto);

	if(conexionCatch ==-1){
		log_info(logger,"Reintenando reconectar cada %d segundos",tiempoReconexion);
        conexionCatch= reintentar_conexion(ip,puerto,10);
	}


	enviar_mensaje("Suscribime",conexionCatch, SUS_CATCH);
	log_info(logger,"Me suscribi a la cola CATCH!");


	while(1){
		mensaje=funcionACK(conexionCatch);
		pthread_t solicitud;
		pthread_create(&solicitud, NULL,(void*)agarrarPokemon,(gamecard*) mensaje);
		pthread_detach(solicitud);
	}

}

//TODO
void funcionNew(int socket){
char* mensaje;

int conexionNew =	crear_conexion(ip,puerto);

	if(conexionNew ==-1){
		log_info(logger,"Reintenando reconectar cada %d segundos",tiempoReconexion);
	    conexionNew= reintentar_conexion(ip,puerto,10);
	}

	enviar_mensaje("Suscribime",conexionNew, SUS_NEW);
	log_info(logger,"Me suscribi a la cola NEW!");


	while(1){
		mensaje=funcionACK(conexionNew);
		pthread_t solicitud;
		pthread_create(&solicitud, NULL,(void *)nuevoPokemon, (gamecard*)mensaje);
		pthread_detach(solicitud);
	}

}





//PARA EL GET
void buscarPokemon(gamecard* mensaje){

FILE * f;
char* mntPokemon;
char* pokemon;

//------------tdv no se donde ponerlo
pthread_mutex_init(&mxArchivo, NULL);

//--------------------------------------


int socketLoc = crear_conexion(ip, puerto);

if(socketLoc ==-1){
		log_info(logger,"No se pudo conectar con el broker");
//TODO
		//MANDAR DIRECTO
	}else{

		mntPokemon="/home/utnso/Escritorio/TALL_GRASS/Files/";
		pokemon=strtok(mensaje->msg," ");
		strcat(mntPokemon,pokemon);
			f = fopen(mntPokemon,"r");

				if(f==NULL){
					enviar_mensaje("0 0 0",socketLoc,LOCALIZED_POKEMON);
//TODO
				/*}else{
				if(mxArchivo==0){
						esperarReintentar(tiempoConfig)
					*/}else{
					t_pokemon pokemon=	obtenerDatosPokemon(f);
			char* datoArchivo = obtenerPokemonString(f, pokemon);
//TODO
			//Agregar el id del mensaje original a datoArchivo
				enviar_mensaje(datoArchivo, socketLoc,LOCALIZED_POKEMON);
					}


	}

}

//TODO
//PARA EL CATCH
void agarrarPokemon(gamecard* mensaje){

FILE * f;
char* mntPokemon;
char* pokemon;
int socketCaugth = crear_conexion(ip, puerto);

if(socketCaugth ==-1){
		log_info(logger,"No se pudo conectar con el broker");

		//TODO
		//MANDAR DIRECTO
	}else{
		mntPokemon="/home/utnso/Escritorio/TALL_GRASS/Files/";
		pokemon=strtok(mensaje->msg," ");
		strcat(mntPokemon,pokemon);

		f = fopen(mntPokemon,"r");

		if(f==NULL){
			log_info(logger,"ERROR: no hay pokemon.");
		}else{
t_pokemon pokemon= obtenerDatosPokemon(f);
			if(igualPosicion(f, mensaje,pokemon)!=1){
				log_info(logger,"ERROR: no hay pokemon.");
			}else{
				if(pokemon.y==1){
					eliminarLinea();
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


void nuevoPokemon(gamecard* mensaje){

}



char* funcionACK(int socket ){
	char* msg;
			msg = recibir_mensaje_cliente(socket);

			devolver_mensaje(msg,sizeof(msg),socket,ACK);
			return msg;
}




char* obtenerPokemonString(FILE* fp, t_pokemon pokemon){
	char* x=string_new();
	char* y=string_new();
	char* cantPoke=string_new();

		x= pokemon.x;
		y= pokemon.y;
		cantPoke= pokemon.cant;


		strcat(x, " ");
		strcat(x, y);
		strcat(x, " ");
		strcat(x, cantPoke);

	return x;


}

int igualPosicion(FILE *fp, gamecard* mensaje, t_pokemon pokemon){

char* poke;
int posCant[3];
	poke=strtok(mensaje->msg," ");
int i= 0;

	while(poke!= NULL){
		posCant[i]=poke;
		poke= strtok(NULL, " ");
		i++;
	}

	if(pokemon.x== posCant[0] && pokemon.y== posCant[1] && pokemon.cant== posCant[2]){
		return 1;
	}else{
		return 0;
	}

}

t_pokemon obtenerDatosPokemon(FILE* fp){

t_pokemon pokemon;
int posx;
int posy;
int cant;
char* poki=string_new();

		fscanf(fp,"%s %d %d %d",poki, posx,posy,cant );

		pokemon.x= posx;
		pokemon.y=posy;
		pokemon.cant=cant;
		pokemon.nombre=poki;


	return pokemon;
}

void eliminarLinea(FILE* fp){





}





