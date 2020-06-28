
#include "broker.h"
#include "suscripciones.h"

int main(void) {

	char* ip;
	char* puerto;
	int i=0;

	t_log* logger;
	t_config* config;

	fflush(stdout);

	char* conf = "/home/utnso/tp-2020-1c-NN/broker/src/broker.config";

	logger = log_create("/home/utnso/broker.txt", "Broker", 1, LOG_LEVEL_INFO);

	config=config_create(conf);

	ip= config_get_string_value(config,"IP_BROKER");
	puerto= config_get_string_value(config,"PUERTO_BROKER");

	log_info(logger,"Servidor con IP %s y puerto %s", ip, puerto);

	//INIT MEMORY

		uint32_t memory_size = config_get_int_value(config,"TAMANO_MEMORIA");
		log_info(logger,"Lei el tamaño de la memoria: %d", memory_size);
		memoria = malloc(memory_size);
		contador_id_particiones = 0;
		particiones = list_create();
		particiones_libres = list_create();
		ParticionLibre *primera_particion_libre = malloc(sizeof(ParticionLibre));
		primera_particion_libre->size = 1800;
		primera_particion_libre->offset_init = 224;
		list_add(particiones_libres, primera_particion_libre);



		//Pruebas juntar particiones libres adyacentes
		/*ParticionLibre *pl1 = malloc(sizeof(ParticionLibre));
		ParticionLibre *pl2 = malloc(sizeof(ParticionLibre));
		pl1->offset_init = 0;
		pl1->size = 100;
		pl2->offset_init = 101;
		pl2->size = 100;
		ParticionLibre *pl4 = malloc(sizeof(ParticionLibre));
		pl4->offset_init = 302;
		pl4->size = 100;
		ParticionLibre *pl3 = malloc(sizeof(ParticionLibre));
		pl3->offset_init = 201;
		pl3->size = 100;
		list_add(particiones_libres, pl1);
		list_add(particiones_libres, pl2);
		list_add(particiones_libres, pl3);
		list_add(particiones_libres, pl4);
		*///

	//Pruebas compactacion
	/*Particion *p = malloc(sizeof(Particion));
	p->offset_init = 100;
	p->size = 100;
	list_add(particiones, p);
	Particion *p2 = malloc(sizeof(Particion));
	p2->offset_init = 50;
	p2->size = 49;
	list_add(particiones, p2);
	compactar_memoria();
	log_info(logger, "-----offset init: %d", p2->offset_init);
	ParticionLibre *l = list_get(particiones_libres, 0);
	log_info(logger, "-----libre init: %d", l->offset_init);
*/


	int socketero[100];
	int socket_servidor = iniciar_servidor(ip,puerto);

	Colas *colas = malloc(sizeof(Colas));
	colas->logger = logger;
	colas->cant_suscritos_get = 0;
	colas->cant_suscritos_appeared = 0;
	colas->cant_suscritos_localized = 0;
	colas->cant_suscritos_caught =0;
	colas->cant_suscritos_catch =0;
	colas->cant_suscritos_new =0;

    while(1){
    	colas->socket_cliente = esperar_cliente(socket_servidor);

    	socketero[i]= colas->socket_cliente;
    	log_info(logger,"socketero: %d", socketero[i]);
    	i++;

    	process_request(colas);
    }

	//char *mensaje = recibir_mensaje(conexion);

	return EXIT_SUCCESS;
}

