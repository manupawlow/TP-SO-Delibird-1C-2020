
#ifndef BROKER_H_
#define BROKER_H_

#include <bibliotec.h>

typedef struct {
	int socket_cliente;

	int SUSCRITOS_GET[10];
	int cant_suscritos_get;

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


	t_log *logger;
}Colas;

uint32_t contador_de_id;

typedef struct{
	uint32_t id_particion;
	uint32_t size;
	uint32_t offset_init;
}Particion;

typedef struct{
	uint32_t size;
	uint32_t offset_init;
}ParticionLibre;

typedef struct{
	uint32_t longitud_nombre;
	char *nombre_pokemon;
}msg_get;

uint32_t contador_id_particiones;

t_list *particiones;
t_list *particiones_libres;


void *memoria;

/*
int cant_mensajes_en_broker = 0;
char* mensajes_en_broker[cant_mensajes_en_broker];

int cant_recibidos_ACK = 0;
int recibidos_ACK[cant_recibidos_ACK];
*/

#endif /* BROKER_H_ */



