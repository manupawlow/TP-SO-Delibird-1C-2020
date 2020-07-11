
#ifndef BROKER_H_
#define BROKER_H_

#include <bibliotec.h>

typedef struct {
	int socket_cliente;

	t_list* SUSCRITOS_GET;

	int SUSCRITOS_LOCALIZED[10];
	int cant_suscritos_localized;

	int SUSCRITOS_CATCH[10];
	int cant_suscritos_catch;

	int SUSCRITOS_CAUGHT[10];
	int cant_suscritos_caught;

	int SUSCRITOS_NEW[10];
	int cant_suscritos_new;

	int SUSCRITOS_APPEARED[10];
	int cant_suscritos_appeared;

	/*	t_list* SUSCRITOS_NEW;

	t_list* SUSCRITOS_LOCALIZED;

	t_list* SUSCRITOS_GET;

	t_list* SUSCRITOS_APPEARED;

	t_list* SUSCRITOS_CATCH;

	t_list* SUSCRITOS_CAUGHT;*/
}Colas;

t_log* logger;
t_config* config;

uint32_t contador_de_id;
pthread_mutex_t mx_id_mensaje;

typedef struct{
	uint32_t id_particion;
	uint32_t size;
	uint32_t offset_init;
	uint32_t offset_end;
	uint32_t tiempo_lru;

	uint8_t id_mensaje;
	char* cola;
	t_list* suscriptores_enviados;
	t_list* suscriptores_ack;
}Particion;

typedef struct{
	uint32_t size;
	uint32_t offset_init;
}ParticionLibre;

typedef struct{
	Particion* particion;

	int size;
	uint32_t offset_init;

	bool esta_libre;
	uint32_t id;
	uint32_t id_buddy;
	uint32_t id_padre;
	uint32_t id_hijo1;
	uint32_t id_hijo2;
}Buddy;

uint32_t contador_id_particiones;
uint32_t contador_id_buddy;
uint32_t tiempo_lru;

t_list *particiones;
t_list *particiones_libres;
t_list* buddies;


void *memoria;
uint32_t memory_size;
pthread_mutex_t mx_memoria;

pthread_mutex_t mx_mostrar;

pthread_mutex_t mx_lru;

int frecuencia_compactacion;
int tamanio_minimo;
char* algoritmo_particion_libre;
char* algoritmo_reemplazo;
char* algoritmo_memoria;

#endif /* BROKER_H_ */

