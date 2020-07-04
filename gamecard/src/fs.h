#include <bibliotec.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <commons/bitarray.h>

t_bitarray* bitmap;



void fs(t_config* config,int block_size,int blocks);

void crearBlocks(int blocks,char* mnt);
void crearFiles(char* mnt);
void crearMetadata(int block_size, int blocks,char* mnt);



//void esperarReintentar(void tiempoConfig);
