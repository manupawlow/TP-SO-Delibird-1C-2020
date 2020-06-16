#include <bibliotec.h>
#include <sys/stat.h>
#include <sys/types.h>






void fs(t_config* config,int block_size,int blocks);

void crearBloques(t_mensaje* mensaje);
void escribirMeta(FILE* f,t_mensaje* mensaje);
void cambiar_meta_blocks(char* montaje);
//void esperarReintentar(void tiempoConfig);
