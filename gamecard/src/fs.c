#include "fs.h"

void fs(t_config* config, int block_size, int blocks){

	FILE* f;
	char* mnt = string_new();
	string_append(&mnt,config_get_string_value(config,"PUNTO_MONTAJE_TALLGRASS"));
	f = fopen(mnt,"r");

	if(f==NULL){


		log_info(logger, "File System inexistente.");
		log_info(logger,"Creando File System...");

		mkdir(mnt,  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	//----------------FILES-------------------------------------
		crearFiles(mnt);
	//----------------METADATA-----------------------------------
		crearMetadata(block_size,blocks,mnt);
	//----------------BLOQUES-----------------------------------
		crearBlocks(blocks, mnt);
	//----------------BITMAP------------------------------------
		char dataLoco[blocks];
		for(int i=1; i< blocks+1;i++){
			//data[i]=0;
			dataLoco[i]=0;
		}
		dataLoco[3]=0;
		bitmap = bitarray_create(dataLoco,sizeof(dataLoco));
		escribirBitmap();
		log_info(logger, "File System creado!\n\n");
	}else{
		log_info(logger,"File System encontrado!\n\n");

		fclose(f);



		char* data=malloc(blocks);
		int length;
		for(int i=1;i<blocks;i++){
			char* mntBlocks = string_new();
			string_append(&mntBlocks,mnt);
			string_append(&mntBlocks,"/Blocks");
			string_append_with_format(&mntBlocks,"/%d.bin",i);
			f = fopen(mntBlocks,"r");
			fseek(f,0L,SEEK_END);
			length = ftell(f);
			fclose(f);
			if(length>0)
				data[i] = 1;
			else if(length==-1)
				log_info(logger,"aiudaaaaaaaaaaaaaaaaaaa");
			else if(length==0)
				data[i] = 0;
			free(mntBlocks);
		}
		bitmap = bitarray_create(data,sizeof(data));
		escribirBitmap();
	}

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


	for(int i=1; i<blocks+1;i++){
		char* montajeBlocks=string_new();
		string_append(&montajeBlocks,Bloques);
		string_append_with_format(&montajeBlocks,"/%d.bin",i);
		f = fopen(montajeBlocks,"w+");
		fclose(f);

		free(montajeBlocks);
	}

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


