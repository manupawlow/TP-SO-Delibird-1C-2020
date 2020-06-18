#include "fs.h"
t_log* logger;
t_config* config;

pthread_mutex_t mxArchivo;

char* mntPokemon = "/home/utnso/Escritorio/TALL_GRASS/Pokemon/";
char* mntBlocks = "/home/utnso/Escritorio/TALL_GRASS/Blocks";

int contadorBloques = 1;



typedef struct{
	uint8_t id_mensaje;
	char* pokemon;
	uint8_t posx;
	uint8_t posy;
	uint8_t cantidad;
}t_pokeNew;




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
    char ** blockRenovado;
    char * blockARenovar;
}t_block;


typedef struct{
	int socket;
	char* msg;
}gamecard;



void buscarPokemon(t_mensaje* mensaje);
void nuevoPokemon(t_mensaje* mensaje);
void agarrarPokemon(char* mensaje);
void funcionACK();
void funcionGet();
void funcionNew();
void funcionCatch();

char* obtenerPokemonString(t_poke* pokemon);
int igualPosicion(FILE *fp, char* mensaje, t_poke* pokemon);
t_mensaje* obtenerDatosPokemon(FILE* fp, t_mensaje* mensaje);



void crearBloques(t_mensaje* mensaje);
void escribirMeta(FILE* f,t_mensaje* mensaje);
void cambiar_meta_blocks(char* montaje,t_mensaje* mensaje);
t_block* compararBlocksYCambiar(FILE* fblocks, t_mensaje* mensaje,char* montajeBlocks);
void recrearBlocks(FILE* fblocks,char** blockRenovado,char* montajeBlocks);
void verificarTamBlock(int size,t_block* block, FILE* fblocks, char* montajeBlocks, char* montaje, char** nroBloque);
void reescribirMeta(char* montaje, char*nuevoBloque,char**nroBloque);



void process_request(int socket_cliente);
void conexion_gameboy();




