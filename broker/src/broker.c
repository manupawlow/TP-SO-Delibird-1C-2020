#include "broker.h"
#include "suscripciones.h"
#include <signal.h>


int main(void) {

	signal(SIGUSR1, dump_cache);

	char* ip;
	char* puerto;
	int i=0;

	fflush(stdout);

	char* conf = "/home/utnso/tp-2020-1c-NN/broker/src/broker.config";

	logger = log_create("/home/utnso/broker.txt", "Broker", 1, LOG_LEVEL_INFO);

	config=config_create(conf);

	ip= config_get_string_value(config,"IP_BROKER");
	puerto= config_get_string_value(config,"PUERTO_BROKER");

	log_info(logger,"Servidor con IP %s y puerto %s", ip, puerto);

	contador_de_id = 1;

	pthread_mutex_init(&mx_id_mensaje, NULL);

	pthread_mutex_init(&mx_lista_new, NULL);
	pthread_mutex_init(&mx_lista_localized, NULL);
	pthread_mutex_init(&mx_lista_get, NULL);
	pthread_mutex_init(&mx_lista_appeared, NULL);
	pthread_mutex_init(&mx_lista_catch, NULL);
	pthread_mutex_init(&mx_lista_caught, NULL);

	//INIT MEMORY

		pthread_mutex_init(&mx_memoria, NULL);
		pthread_mutex_init(&mx_mostrar, NULL);

		frecuencia_compactacion = config_get_int_value(config,"FRECUENCIA_COMPACTACION");
		tamanio_minimo = config_get_int_value(config,"TAMANO_MINIMO_PARTICION");
		memory_size = config_get_int_value(config,"TAMANO_MEMORIA");
		algoritmo_memoria = config_get_string_value(config,"ALGORITMO_MEMORIA");
		algoritmo_particion_libre = config_get_string_value(config,"ALGORITMO_PARTICION_LIBRE");
		algoritmo_reemplazo = config_get_string_value(config,"ALGORITMO_REEMPLAZO");

		memoria = malloc(memory_size);
		contador_id_particiones = 1;
		particiones = list_create();
		if(strcmp(algoritmo_memoria, "PARTICIONES") == 0){
			particiones_libres = list_create();
			ParticionLibre *primera_particion_libre = malloc(sizeof(ParticionLibre));
			primera_particion_libre->size = memory_size;
			primera_particion_libre->offset_init = 0;
			list_add(particiones_libres, primera_particion_libre);
		}

		if(strcmp(algoritmo_memoria, "BS") == 0){
			contador_id_buddy = 0;
			buddies = list_create();
			Buddy* primer_buddy = malloc(sizeof(Buddy));
			primer_buddy->size = memory_size;
			primer_buddy->id = contador_id_buddy;
			primer_buddy->id_padre = -1;
			primer_buddy->id_buddy = -1;
			primer_buddy->id_hijo1 = -1;
			primer_buddy->id_hijo2 = -1;
			primer_buddy->esta_libre = 1;
			primer_buddy->offset_init = 0;
			contador_id_buddy++;
			list_add(buddies, primer_buddy);
		}

		log_info(logger, "<BROKER CONFIGURATION> %s | %s | %s | %d | %d | %d ",
			algoritmo_memoria, algoritmo_particion_libre, algoritmo_reemplazo, memory_size, tamanio_minimo, frecuencia_compactacion);


	int socketero[100];
	int socket_servidor = iniciar_servidor(ip,puerto);

	SUSCRITOS_NEW = list_create();
	SUSCRITOS_LOCALIZED = list_create();
	SUSCRITOS_GET = list_create();
	SUSCRITOS_APPEARED = list_create();
	SUSCRITOS_CATCH = list_create();
	SUSCRITOS_CAUGHT = list_create();


    while(1){
    	int socket_cliente = esperar_cliente(socket_servidor);

    	if(socket_cliente != -1){
        	socketero[i]= socket_cliente;
        	log_info(logger,"socketero: %d", socketero[i]);
        	i++;

        	pthread_t cliente_thread;
        	pthread_create(&cliente_thread, NULL,(void*) process_request,(void*)socket_cliente);

        }
    }



	return EXIT_SUCCESS;
}




void dump_cache(int n){

	FILE* f;

	int contador = 1;

	f=fopen("dump.txt","w+");

	fprintf(f, "-----------------------------------------------------------------------------------------------------------------\n");

	time_t tiempo = time(0);
	struct tm *tlocal = localtime(&tiempo);
	char output[128];
	strftime(output, 128, "%d/%m/%y %H:%M:%S", tlocal);

	char* dump = string_new();
	string_append_with_format(&dump, "Dump: %s ",output);
	fprintf(f, "%s\n", dump);
	free(dump);


if(strcmp(algoritmo_memoria, "PARTICIONES") == 0){

	for(int i= 0; i<list_size(particiones);i++){
	char* particion=string_new();
	Particion* p= list_get(particiones,i);
			string_append_with_format(&particion, "Particion %s: ",string_itoa(contador));
			string_append_with_format(&particion, "%#05X - ", p->offset_init);
			string_append_with_format(&particion, "%#05X.", p->offset_end);
			string_append_with_format(&particion, "	[X] Size: %sb ",string_itoa(p->size) );
			string_append_with_format(&particion, "LRU: %" PRIu64 " ", p->tiempo_lru);
			string_append_with_format(&particion, "Cola: %s ", p->cola);
			string_append_with_format(&particion, "ID: %s", string_itoa(p->id_mensaje));
			fprintf(f, "%s\n\n",particion );
			contador++;
			free(particion);
		}

	for(int i=0; i<list_size(particiones_libres); i++){
		char* particion = string_new();
		ParticionLibre* libre = list_get(particiones_libres, i);
		string_append_with_format(&particion, "Particion %s: ",string_itoa(contador));
		string_append_with_format(&particion, "%#05X - ", libre->offset_init);
		string_append_with_format(&particion, "%#05X.", libre->offset_init + libre->size - 1);
		string_append_with_format(&particion, "	[L] Size: %sb ",string_itoa(libre->size) );
		fprintf(f, "%s\n",particion );
		contador++;
		free(particion);
	}

}

if(strcmp(algoritmo_memoria, "BS") == 0){
	for(int i= 0; i<list_size(buddies);i++){

		Buddy* p= list_get(buddies,i);

		if(!p->esta_libre && p->id_hijo1 == -1 && p->id_hijo2 == -1){
			char* particion=string_new();

			string_append_with_format(&particion, "Particion %d: ",contador);
			string_append_with_format(&particion, "%#05X - ", p->offset_init);
			string_append_with_format(&particion, "#%05X.", p->offset_init + p->size - 1);
			string_append_with_format(&particion, "	[X] Size: %db ",p->particion->size );
			string_append_with_format(&particion, "LRU: %" PRIu64 " ", p->particion->tiempo_lru);
			string_append_with_format(&particion, "Cola: %s ", p->particion->cola);
			string_append_with_format(&particion, "ID: %d", p->particion->id_mensaje);
			fprintf(f, "%s\n\n",particion );
			contador++;
			free(particion);
		}

		if(p->esta_libre && p->id_hijo1 == -1 && p->id_hijo2 == -1){
			char* particion = string_new();
			string_append_with_format(&particion, "Particion %d: ",contador);
			string_append_with_format(&particion, "%#05X - ", p->offset_init);
			string_append_with_format(&particion, "%#05X.", p->offset_init + p->size - 1);
			string_append_with_format(&particion, "	[L] Size: %db ",p->size);
			fprintf(f, "%s\n",particion );
			contador++;
			free(particion);
		}
	}

}


	fprintf(f, "-----------------------------------------------------------------------------------------------------------------\n");
	fclose(f);

	log_info(logger, "<CACHE> Actualize el dump de la cache!");
}


