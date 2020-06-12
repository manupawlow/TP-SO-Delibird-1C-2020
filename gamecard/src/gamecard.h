#include "fs.h"
#include <bibliotec.h>
#include <sys/stat.h>
#include <sys/types.h>
t_log* logger;
t_config* config;

pthread_mutex_t mxArchivo;


char* ip;
char* puerto;
int tiempoReconexion;

typedef struct{
	int socket;
	char* msg;
}gamecard;

typedef struct{
	int x;
	int y;
	int cant;
	char* nombre;
}t_pokemon;

void buscarPokemon(gamecard* mensaje);
void nuevoPokemon(gamecard* mensaje);
void agarrarPokemon(gamecard* mensaje);
char* funcionACK(int socket);
void funcionGet();
void funcionNew();
void funcionCatch();

char* obtenerPokemonString(FILE* fp, t_pokemon pokemon);
int igualPosicion(FILE *fp, gamecard* mensaje, t_pokemon pokemon);
t_pokemon obtenerDatosPokemon(FILE* fp);

