#ifndef SUSCRIPCIONES_H_
#define SUSCRIPCIONES_H_

#include "broker.h"

void enviar_a_todos(int suscritos[], int cant, int msg,int size, int cod_op);
void process_request(Colas *colas);

#endif /* SUSCRIPCIONES_H_ */
