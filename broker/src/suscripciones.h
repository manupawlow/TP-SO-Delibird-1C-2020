
#ifndef SUSCRIPCIONES_H_
#define SUSCRIPCIONES_H_

#include "broker.h"

//MENSAJES
void enviar_a_todos(int suscritos[], int cant, char* msg,int size, int cod_op);
void process_request(Colas *colas);
uint8_t asignar_id();

//MEMORIA
void cachear_mensaje_get(t_mensaje*, int);
int indice_particion_libre_para_almacenar(uint32_t);
void borrar_particion(int );
void borrar_particion_FIFO();
void juntar_libres_adyacentes();
void unificar_libres();
int indice_menor_offset_mayor_a_n(int);
int mover_particiones();
void compactar_memoria();

#endif /* SUSCRIPCIONES_H_ */

