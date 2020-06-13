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


