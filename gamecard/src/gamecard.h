#include <sys/stat.h>
#include <sys/types.h>
#include "fs.h"
t_log* logger;
t_config* config;

pthread_mutex_t mxArchivo;

char* mntPokemon="/home/utnso/Escritorio/TALL_GRASS/Files/";






char* ip;
char* puerto;
int tiempoReconexion;

typedef struct{
	int x;
	int y;
	int cant;
	char* nombre;
}t_poke;




typedef struct{
	int socket;
	char* msg;
}gamecard;



void buscarPokemon(char* mensaje);
void nuevoPokemon(char* mensaje);
void agarrarPokemon(char* mensaje);
char* funcionACK(int socket);
void funcionGet();
void funcionNew();
void funcionCatch();

char* obtenerPokemonString(t_poke* pokemon);
int igualPosicion(FILE *fp, char* mensaje, t_poke* pokemon);
t_poke* obtenerDatosPokemon(FILE* fp);

void process_request(int socket_cliente);
void conexion_gameboy();





