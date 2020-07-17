#include <bibliotec.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <commons/bitarray.h>

t_bitarray* bitmap;
t_log* logger;


void fs(t_config* config,int block_size,int blocks);

void crearBlocks(int blocks,char* mnt);
void crearFiles(char* mnt);
void crearMetadata(int block_size, int blocks,char* mnt);
void escribirBitmap();
void escribirBitmapInicio();


//void esperarReintentar(void tiempoConfig);
