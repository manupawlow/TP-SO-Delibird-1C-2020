#include "fs.h"

void fs(t_config* config, int block_size, int blocks){

	char* mnt;
	char* Files;
	char* Bloques;
	char* Meta;


FILE * fdata;
FILE * ffiles;

char* cadena[3];
cadena[0]=string_new();
cadena[1]=string_new();

	mnt=config_get_string_value(config,"PUNTO_MONTAJE_TALLGRASS");
	mkdir(mnt,  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

//----------------FILES-------------------------------------
	Files="/home/utnso/Escritorio/TALL_GRASS/Pokemon";
	mkdir(Files,  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	Files="/home/utnso/Escritorio/TALL_GRASS/Pokemon/Metadata.bin";

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

	char* sizeBlock=string_itoa(block_size);
	char*blockString=string_itoa(blocks);
	string_append_with_format(&cadena[0],"BLOCK_SIZE=%s",sizeBlock);
	string_append_with_format(&cadena[1],"BLOCKS=%s",blockString);
	cadena[2]="MAGIC_NUMBER=TALL_GRASS";
	for(int i=0; i<3;i++){
		fprintf(fdata,"%s\n",cadena[i]);
	}
	fclose(fdata);

}




