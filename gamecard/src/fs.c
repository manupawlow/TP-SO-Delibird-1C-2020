#include "fs.h"

void fs(t_config* config){

	char* mnt;
	char* Files;
	char* Bloques;
	char* Meta;


FILE * fdata;
FILE * ffiles;

char* cadena[3];
	mnt=config_get_string_value(config,"PUNTO_MONTAJE_TALLGRASS");
	mkdir(mnt,  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

//----------------FILES-------------------------------------
	Files="/home/utnso/Escritorio/TALL_GRASS/Files";
	mkdir(Files,  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	Files="/home/utnso/Escritorio/TALL_GRASS/Files/Metadata.bin";

	ffiles= fopen(Files,"w+");
	fprintf(ffiles,"DIRECTORY=Y");
	fclose(ffiles);
//----------------BLOQUES-----------------------------------
	Bloques="/home/utnso/Escritorio/TALL_GRASS/Blocks";

	mkdir(Bloques,  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
//----------------METADATA-----------------------------------
	Meta="/home/utnso/Escritorio/TALL_GRASS/Metadata";
	mkdir(Meta,  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	Meta="/home/utnso/Escritorio/TALL_GRASS/Metadata/Metadata.bin";

	fdata= fopen(Meta,"w+");
	cadena[0]="BLOCK_SIZE=64";
	cadena[1]="BLOCKS=5192";
	cadena[2]="MAGIC_NUMBER=TALL_GRASS";
	for(int i=0; i<3;i++){
		fprintf(fdata,"%s\n",cadena[i]);
	}
	fclose(fdata);

}


/*

//TODO
//PARA EL GET
void* buscarPokemon(void* mensaje){
FILE * f;
char* mntPokemon;




//------------tdv no se donde ponerlo
pthread_mutex_init(&mxArchivo, NULL);

//--------------------------------------


//mntPokemon="/home/utnso/Escritorio/TALL_GRASS/Files/MENSAJE.POKEMON";

f = fopen(mntPokemon,"r");

	if(f==NULL){
	enviar_mensaje("0 0 0",socket,op_code);
	}else{
	if(mxArchivo==0){
			esperarReintentar(tiempoConfig)
		}else{
void datoArchivo = obtenerPokemon(archivo);

	enviar_mensaje(datosArchivo, socketLoc)
/*	datosArchivo:
	 * 		id mensaje original
	 * 		pos x y
	 * 		cant
	 *
	 *
	 * 	si no me puedo conectar al broker
	 * 	informo por logs y lo mando directo
	 */
/*return mensaje;
}

//TODO
//PARA EL CATCH
void* agarrarPokemon(void* mensaje){

FILE * f;
char* mntPokemon;

char* resultado;
	//mntPokemon="/home/utnso/Escritorio/TALL_GRASS/Files/MENSAJE.POKEMON";

	f = fopen(mntPokemon,"r");

	if(f==NULL){
	  	resultado="ERROR";
	}else{
if(checkMutex==0){
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
/*return mensaje;
}


void* nuevoPokemon(void* mensaje){


FILE * f;
char* mntPokemon;

	//mntPokemon="/home/utnso/Escritorio/TALL_GRASS/Files/MENSAJE.POKEMON";

f = fopen(mntPokemon,"r");

	if(f==NULL){

	}else{


	}
return mensaje;

}


*/

/*void esperarReintentar(void tiempoConfig){

}
*/
