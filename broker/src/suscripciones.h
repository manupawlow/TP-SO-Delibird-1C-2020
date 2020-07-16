#ifndef SUSCRIPCIONES_H_
#define SUSCRIPCIONES_H_

#include "broker.h"

//MENSAJES
void enviar_a_todos(int suscritos[], int cant, char* msg,int size, int cod_op);
void process_request(Colas *colas);
uint32_t asignar_id();
void actualizar_lista_suscritos(t_list*, Proceso*);

//MEMORIA
void cachear_mensaje_new(t_mensaje*, int);
void cachear_mensaje_get(t_mensaje*, int);
void cachear_mensaje_appeared_or_catch(t_mensaje*, int, char*);
void cachear_mensaje_caught(t_mensaje*, int);
void cachear_mensaje_localized(t_mensaje_get*, int);

void almacenar_mensaje_localized(t_mensaje_get*, char*);
void almacenar_particion_localized(t_mensaje_get*, char*, int);
void almacenar_buddy_localized(t_mensaje_get*, char*, int);
void almacenar_en_hijo_si_corresponde_localized(int, t_mensaje_get*, char*, int);


int indice_particion_libre_para_almacenar(uint32_t);
void delete_particion(int);
void borrar_particion();
void consolidacion();
void unificar_libres();
int indice_menor_offset_mayor_a_n(int);
int indice_libre_menor_offset_mayor_a_n(int);
int mover_particiones();
void compactar_memoria();
void almacenar_mensaje(t_mensaje*, char*);
void almacenar_particion(t_mensaje*, char*, int);
void enviar_mensajes_en_memoria(Proceso*, char*);
void* leer_particion(Particion*);
void guardar_ACK(int, int);
void agregar_enviados(t_mensaje*, t_list*);
bool se_le_envio_el_mensaje(Proceso*, Particion*);
bool devolvio_ack(Proceso*, Particion*);
int buscar_particion_por_id(int);
int buscar_particion_por_id_mensaje(int);
int buscar_proceso_en_enviados_por_id(int, t_list*);
void agregar_enviados_localized(t_mensaje_get*, t_list*);

//buddy
int potencia_dos_mas_cercana(int);
int buscar_buddy_por_id(int );
int dividir_buddy(Buddy*);
void almacenar_buddy(t_mensaje*, char*, int);
int indice_buddy_libre_para_almacenar(int);
void almacenar_en_hijo_si_corresponde(int, t_mensaje*, char*, int);
void borrar_buddy();
void delete_buddy(Buddy*);
void consolidar_buddies(Buddy*);
int tiene_hijos_libres_con_espacio(Buddy*, int);
int buscar_buddy_por_id_particion(int);

//uint64_t timestamp();
void dump_cache(int);
void mostrar();

#endif /* SUSCRIPCIONES_H_ */
