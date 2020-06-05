#ifndef BROKER_H_
#define BROKER_H_

#include <bibliotec.h>

typedef struct {
	int socket_cliente;
	int SUSCRITOS_GET[10];
	int cant_suscritos_get;
	t_log *logger;
}Colas;


#endif /* BROKER_H_ */
