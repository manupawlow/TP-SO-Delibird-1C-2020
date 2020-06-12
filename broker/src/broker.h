
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

int contador_de_id;

/*
int cant_mensajes_en_broker = 0;
char* mensajes_en_broker[cant_mensajes_en_broker];

int cant_recibidos_ACK = 0;
int recibidos_ACK[cant_recibidos_ACK];
*/

#endif /* BROKER_H_ */

