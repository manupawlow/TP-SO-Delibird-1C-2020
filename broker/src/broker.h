
#ifndef BROKER_H_
#define BROKER_H_

#include <bibliotec.h>
#include<inttypes.h>

typedef struct {

	int socket_cliente;

	t_list* SUSCRITOS_NEW;

	t_list* SUSCRITOS_LOCALIZED;

	t_list* SUSCRITOS_GET;

	t_list* SUSCRITOS_APPEARED;

	t_list* SUSCRITOS_CATCH;

	t_list* SUSCRITOS_CAUGHT;

}Colas;


t_list* SUSCRITOS_NEW;

t_list* SUSCRITOS_LOCALIZED;

t_list* SUSCRITOS_GET;

t_list* SUSCRITOS_APPEARED;

t_list* SUSCRITOS_CATCH;

t_list* SUSCRITOS_CAUGHT;

pthread_mutex_t mx_lista_new;
pthread_mutex_t mx_lista_localized;
pthread_mutex_t mx_lista_get;
pthread_mutex_t mx_lista_appeared;
pthread_mutex_t mx_lista_catch;
pthread_mutex_t mx_lista_caught;

t_log* logger;
t_config* config;

uint32_t contador_de_id;
pthread_mutex_t mx_id_mensaje;

typedef struct{
	uint32_t id_particion;
	uint32_t size;
	uint32_t offset_init;
	uint32_t offset_end;
	uint64_t tiempo_lru;

	uint32_t id_mensaje;
	uint32_t id_mensaje_correlativo;
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

typedef struct{
	int id_proceso;
	int socket;
}Proceso;

typedef struct{
	uint32_t x;
	uint32_t y;
}Coordenadas;


uint32_t contador_id_particiones;
uint32_t contador_id_buddy;

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

