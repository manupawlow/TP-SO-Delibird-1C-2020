#include "fs.h"

void fs(t_config* config, int block_size, int blocks){

	char* mnt = string_new();
	string_append(&mnt,config_get_string_value(config,"PUNTO_MONTAJE_TALLGRASS"));
	mkdir(mnt,  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
//----------------FILES-------------------------------------
	crearFiles(mnt);

//----------------METADATA-----------------------------------
	crearMetadata(block_size,blocks,mnt);
//----------------BLOQUES-----------------------------------
	crearBlocks(blocks, mnt);

	free(mnt);
}


void crearFiles (char* mnt){
	FILE* f;
	char* Files = string_new();

	string_append(&Files, mnt);
	string_append(&Files, "/Files");
	mkdir(Files,  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	string_append(&Files,"/Metadata.bin");

		f= fopen(Files,"w+");

		fprintf(f,"DIRECTORY=Y");
		fclose(f);
		free(Files);
}


void crearBlocks(int blocks,char* mnt){
	FILE* f;
	char* Bloques = string_new();
	string_append(&Bloques,mnt);
	string_append(&Bloques,"/Blocks");
	mkdir(Bloques,  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);


	/*for(int i=1; i<blocks+1;i++){
		char* montajeBlocks=string_new();
		string_append(&montajeBlocks,Bloques);
		string_append_with_format(&montajeBlocks,"/%s.bin",string_itoa(i));
		f = fopen(montajeBlocks,"w+");
		fclose(f);

		free(montajeBlocks);
	}
*/
	free(Bloques);
}


void crearMetadata(int block_size, int blocks, char* mnt){
	FILE* f;
	char* cadena[3];
	cadena[0] = string_new();
	cadena[1] = string_new();
	cadena[2] = string_new();

	string_append(&cadena[2],"MAGIC_NUMBER=TALL_GRASS");

	char* Meta = string_new();
	string_append(&Meta,mnt);
	string_append(&Meta,"/Metadata");
	mkdir(Meta,  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	string_append(&Meta,"/Metadata.bin");

	f = fopen(Meta,"w+");
	free(Meta);
	char* sizeBlock=string_itoa(block_size);
	char*blockString=string_itoa(blocks);
	string_append_with_format(&cadena[0],"BLOCK_SIZE=%s",sizeBlock);
	string_append_with_format(&cadena[1],"BLOCKS=%s",blockString);

	for(int i=0; i<3;i++){
		fprintf(f,"%s\n",cadena[i]);
		free(cadena[i]);
	}
	free(sizeBlock);
	free(blockString);
	fclose(f);
}


