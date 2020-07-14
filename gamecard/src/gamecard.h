#include "fs.h"
t_config* config;
char* ID_PROCESO;

pthread_mutex_t mxArchivo;

char* montajePokemon;
char* montajeBlock;

char* ip;
char* puerto;
int tiempoReconexion,tiempoDeRetardo,tiempoDeReintento;

int blockSize = 24;
int cantBlocks = 5192;

void buscarPokemon(t_mensaje* mensaje);
void nuevoPokemon(t_mensaje* mensaje);
void agarrarPokemon(t_mensaje* mensaje);
void funcionACK();
void funcionGet();
void funcionNew();
void funcionCatch();


void asignarBloqueYcrearMeta(t_mensaje* mensaje,char* montaje);
void escribirMeta(FILE* f,t_mensaje* mensaje,char** lista,char* bloquesActualizados);
void cambiar_meta_blocks(char* montaje,t_mensaje* mensaje);


char** agregarBloquesAPartirDeString(char* escribirBloque,FILE* f,off_t offset);
int calcularCantidadDeBLoques(char* escribirBloque);
char* verificarCoincidenciasYsumarCantidad(char* datosBins, t_mensaje* mensaje);
off_t primerBloqueDisponible();
void escrituraDeMeta(FILE* f,t_mensaje* mensaje,char** listaBloquesUsados,char* montaje,char*bloquesActualizados);

char* guardarDatosBins(t_mensaje* mensaje);

//func auxiliares no borrar q muere el programa dale gracias
char* montarBlocks(char** arrayBloques, int i);
char** agarrarBlocks(t_mensaje* mensaje);
int existePokemon(t_mensaje* mensaje);
char* montarPoke(t_mensaje* mensaje);
void esperaOpen(char* montaje);
void borrarDatosBlock(t_mensaje* mensaje);
void escrituraMetaBlanco( t_mensaje* mensaje, char* montaje);
void escribirMetaBlanco(FILE*f,t_mensaje* mensaje);

int process_request(int socket_cliente);
void conexion_gameboy();




