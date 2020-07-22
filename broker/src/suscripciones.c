#include "suscripciones.h"


void process_request(int socket_cliente) {
    int cod_op;
	t_mensaje* mensaje;
	t_mensaje_get* mensajeGet;
	char* mensajeACK;
	t_buffer *buffer;
	Proceso* nuevo_proceso;
	Proceso* proceso;
	int id;
	int indice;
	bool es_misma;

	if(recv(socket_cliente, &cod_op, sizeof(op_code), MSG_WAITALL) == -1)
			cod_op = -1;

		switch (cod_op) {


//--------------------GET-----------------------------------
		case SUS_GET:
			pthread_mutex_lock(&mx_lista_get);

			//LLEGA UN NUEVO PROCESO, ME FIJO SI ES UNO QUE SE CAYO Y SE RECONECTO A TRAVES DEL ID QUE ME MANDAN
			mensajeACK = recibir_mensaje(socket_cliente);

			nuevo_proceso = malloc(sizeof(Proceso));
			nuevo_proceso->id_proceso = atoi(mensajeACK);
			id = nuevo_proceso->id_proceso;
			nuevo_proceso->socket = socket_cliente;

			log_info(logger, "<SUSCRIPCION> Se suscribio a la cola GET el proceso con id %d con socket %d ", nuevo_proceso->id_proceso, nuevo_proceso->socket);

			if(id != 99){
				actualizar_lista_suscritos(SUSCRITOS_GET, nuevo_proceso);

				indice = buscar_proceso_en_enviados_por_id(id, SUSCRITOS_GET);//el nombre de la funcion esta mal :(
				proceso = list_get(SUSCRITOS_GET, indice);

				enviar_mensajes_en_memoria(proceso, "GET");
			}else
				enviar_mensajes_en_memoria(nuevo_proceso, "GET");

			free(mensajeACK);

			pthread_mutex_unlock(&mx_lista_get);
			break;

		case GET_POKEMON:
			log_info(logger,"<MENSAJE> Llego un nuevo mensaje a la cola GET.");
			pthread_mutex_lock(&mx_lista_get);

			mensaje = recibir_mensaje_struct(socket_cliente);

			mensaje->id_mensaje = asignar_id();

			es_misma = es_misma_respuesta(mensaje->id_mensaje_correlativo);

			almacenar_mensaje(mensaje, "GET");

			agregar_enviados(mensaje, SUSCRITOS_GET);

			buffer = serializar_mensaje_struct(mensaje);
			//Notifico el id del mensaje
			enviar_mensaje_struct(buffer, socket_cliente, GET_POKEMON);

			if(!es_misma)
				for(int i=0; i<list_size(SUSCRITOS_GET); i++){
					Proceso* suscripto = list_get(SUSCRITOS_GET, i);
					enviar_mensaje_struct(buffer, suscripto->socket, GET_POKEMON);
					log_info(logger,"<MENSAJE> Envie mensaje al proceso %d", suscripto->id_proceso);
				}

			free(buffer->stream);
			free(buffer);

			//log_info(logger,"Se envio mensaje a todos los suscriptos!");

			pthread_mutex_unlock(&mx_lista_get);
			break;


//----------------------------------------------------------------
//--------------------LOCALIZED-----------------------------------
		case SUS_LOC:
			pthread_mutex_lock(&mx_lista_localized);
			//Suscribir a la cola Localized
			mensajeACK = recibir_mensaje(socket_cliente);

			nuevo_proceso = malloc(sizeof(Proceso));
			nuevo_proceso->id_proceso = atoi(mensajeACK);
			id = nuevo_proceso->id_proceso;
			nuevo_proceso->socket = socket_cliente;

			log_info(logger, "<SUSCRIPCION> Se suscribio a la cola LOCALIZED el proceso con id %d con socket %d ", nuevo_proceso->id_proceso, nuevo_proceso->socket);

			if(id != 99){
				actualizar_lista_suscritos(SUSCRITOS_LOCALIZED, nuevo_proceso);

				indice = buscar_proceso_en_enviados_por_id(id, SUSCRITOS_LOCALIZED);
				proceso = list_get(SUSCRITOS_LOCALIZED, indice);

				enviar_mensajes_en_memoria(proceso, "LOCALIZED");
			}else
				enviar_mensajes_en_memoria(nuevo_proceso, "LOCALIZED");

			free(mensajeACK);
			pthread_mutex_unlock(&mx_lista_localized);
			break;

		case LOCALIZED_POKEMON:
			log_info(logger,"<MENSAJE> Llego un nuevo mensaje a la cola LOCALIZED.");
			pthread_mutex_lock(&mx_lista_localized);
			//Agrega el mensaje a la cola localized

			mensajeGet = recibir_mensaje_struct_get(socket_cliente);

			mensajeGet->id_mensaje = asignar_id();

			es_misma = es_misma_respuesta(mensajeGet->id_mensaje_correlativo);

			almacenar_mensaje_localized(mensajeGet, "LOCALIZED");

			agregar_enviados_localized(mensajeGet, SUSCRITOS_LOCALIZED);

			buffer = serializar_mensaje_struct_get(mensajeGet);
			//Notifico el id del mensaje
			enviar_mensaje_struct(buffer, socket_cliente, LOCALIZED_POKEMON);

			if(!es_misma)
				for(int i=0; i<list_size(SUSCRITOS_LOCALIZED); i++){
					Proceso* suscripto = list_get(SUSCRITOS_LOCALIZED, i);
					enviar_mensaje_struct(buffer, suscripto->socket, LOCALIZED_POKEMON);
					log_info(logger,"<MENSAJE> Envie mensaje al proceso %d", suscripto->id_proceso);
				}

			free(buffer->stream);
			free(buffer);

			//log_info(logger,"Se envio mensaje a todos los suscriptos!");


			pthread_mutex_unlock(&mx_lista_localized);
			break;

//----------------------------------------------------------------
//--------------------CATCH---------------------------------------
		case SUS_CATCH:
			pthread_mutex_lock(&mx_lista_catch);
			//Suscribir a la cola catch
			mensajeACK = recibir_mensaje(socket_cliente);

			nuevo_proceso = malloc(sizeof(Proceso));
			nuevo_proceso->id_proceso = atoi(mensajeACK);
			id = nuevo_proceso->id_proceso;
			nuevo_proceso->socket = socket_cliente;

			log_info(logger, "<SUSCRIPCION> Se suscribio a la cola CATCH el proceso con id %d con socket %d ", nuevo_proceso->id_proceso, nuevo_proceso->socket);

			if(id != 99){
				actualizar_lista_suscritos(SUSCRITOS_CATCH, nuevo_proceso);

				indice = buscar_proceso_en_enviados_por_id(id, SUSCRITOS_CATCH);
				proceso = list_get(SUSCRITOS_CATCH, indice);

				enviar_mensajes_en_memoria(proceso, "CATCH");
			}else
				enviar_mensajes_en_memoria(nuevo_proceso, "CATCH");

			free(mensajeACK);

			pthread_mutex_unlock(&mx_lista_catch);
			break;

		case CATCH_POKEMON:
			log_info(logger,"<MENSAJE> Llego un nuevo mensaje a la cola CATCH.");
			pthread_mutex_lock(&mx_lista_catch);
			//Agrega el mensaje a la cola catch

			mensaje = recibir_mensaje_struct(socket_cliente);

			mensaje->id_mensaje = asignar_id();

			es_misma = es_misma_respuesta(mensaje->id_mensaje_correlativo);

			almacenar_mensaje(mensaje, "CATCH");

			agregar_enviados(mensaje, SUSCRITOS_CATCH);

			buffer = serializar_mensaje_struct(mensaje);

			//Notifico el id del mensaje
			enviar_mensaje_struct(buffer, socket_cliente, CATCH_POKEMON);

			if(!es_misma)
				for(int i=0; i<list_size(SUSCRITOS_CATCH); i++){
				Proceso* suscripto = list_get(SUSCRITOS_CATCH, i);
					enviar_mensaje_struct(buffer, suscripto->socket, CATCH_POKEMON);
					log_info(logger,"<MENSAJE> Envie mensaje al proceso %d", suscripto->id_proceso);
				}
			free(buffer->stream);
			free(buffer);
			//log_info(logger,"Se envio mensaje a todos los suscriptos!");

			pthread_mutex_unlock(&mx_lista_catch);
			break;

//----------------------------------------------------------------
//--------------------CAUGHT--------------------------------------
		case SUS_CAUGHT:
			pthread_mutex_lock(&mx_lista_caught);
			//Suscribir a la cola caught
			mensajeACK = recibir_mensaje(socket_cliente);

			nuevo_proceso = malloc(sizeof(Proceso));
			nuevo_proceso->id_proceso = atoi(mensajeACK);
			id = nuevo_proceso->id_proceso;
			nuevo_proceso->socket = socket_cliente;

			log_info(logger, "<SUSCRIPCION> Se suscribio a la cola CAUGHT el proceso con id %d con socket %d ", nuevo_proceso->id_proceso, nuevo_proceso->socket);

			if(id != 99){
				actualizar_lista_suscritos(SUSCRITOS_CAUGHT, nuevo_proceso);
				indice = buscar_proceso_en_enviados_por_id(id, SUSCRITOS_CAUGHT);
				proceso = list_get(SUSCRITOS_CAUGHT, indice);

				enviar_mensajes_en_memoria(proceso, "CAUGHT");
			}else
				enviar_mensajes_en_memoria(nuevo_proceso, "CAUGHT");


			free(mensajeACK);

			pthread_mutex_unlock(&mx_lista_caught);
			break;

		case CAUGHT_POKEMON:
			log_info(logger,"<MENSAJE> Llego un nuevo mensaje a la cola CAUGHT.");
			pthread_mutex_lock(&mx_lista_caught);
			//Agrega el mensaje a la cola caugth

			mensaje = recibir_mensaje_struct(socket_cliente);

			mensaje->id_mensaje = asignar_id();

			es_misma = es_misma_respuesta(mensaje->id_mensaje_correlativo);

			almacenar_mensaje(mensaje, "CAUGHT");

			agregar_enviados(mensaje, SUSCRITOS_CAUGHT);

			buffer = serializar_mensaje_struct(mensaje);

			//Notifico el id del mensaje
			enviar_mensaje_struct(buffer, socket_cliente, CAUGHT_POKEMON);

			if(!es_misma)
				for(int i=0; i<list_size(SUSCRITOS_CAUGHT); i++){
					Proceso* suscripto = list_get(SUSCRITOS_CAUGHT, i);
					enviar_mensaje_struct(buffer, suscripto->socket, CAUGHT_POKEMON);
					log_info(logger,"<MENSAJE> Envie mensaje al proceso %d", suscripto->id_proceso);
				}

			free(buffer->stream);
			free(buffer);
			//log_info(logger,"Se envio mensaje a todos los suscriptos!");

			pthread_mutex_unlock(&mx_lista_caught);
			break;

//----------------------------------------------------------------
//--------------------NEW-----------------------------------------
		case SUS_NEW:
			pthread_mutex_lock(&mx_lista_new);
			//Suscribir a la cola new
			mensajeACK = recibir_mensaje(socket_cliente);

			nuevo_proceso = malloc(sizeof(Proceso));
			nuevo_proceso->id_proceso = atoi(mensajeACK);
			id = nuevo_proceso->id_proceso;
			nuevo_proceso->socket = socket_cliente;

			log_info(logger, "<SUSCRIPCION> Se suscribio a la cola NEW el proceso con id %d con socket %d ", nuevo_proceso->id_proceso, nuevo_proceso->socket);

			if(id != 99){
				actualizar_lista_suscritos(SUSCRITOS_NEW, nuevo_proceso);
			indice = buscar_proceso_en_enviados_por_id(id, SUSCRITOS_NEW);
			proceso = list_get(SUSCRITOS_NEW, indice);

			enviar_mensajes_en_memoria(proceso, "NEW");
			}else
				enviar_mensajes_en_memoria(nuevo_proceso, "NEW");


			free(mensajeACK);

			pthread_mutex_unlock(&mx_lista_new);
			break;

		case NEW_POKEMON:
			log_info(logger,"<MENSAJE> Llego un nuevo mensaje a la cola NEW.");
			pthread_mutex_lock(&mx_lista_new);
			//Agrega el mensaje a la cola new

			mensaje = recibir_mensaje_struct(socket_cliente);

			mensaje->id_mensaje = asignar_id();

			es_misma = es_misma_respuesta(mensaje->id_mensaje_correlativo);

			almacenar_mensaje(mensaje, "NEW");

			agregar_enviados(mensaje, (void*) SUSCRITOS_NEW);

			buffer = serializar_mensaje_struct(mensaje);

			//Notifico el id del mensaje
			enviar_mensaje_struct(buffer, socket_cliente, NEW_POKEMON);

			if(!es_misma)
				for(int i=0; i<list_size(SUSCRITOS_NEW); i++){
					Proceso* suscripto = list_get(SUSCRITOS_NEW, i);
					enviar_mensaje_struct(buffer, suscripto->socket, NEW_POKEMON);
					log_info(logger,"<MENSAJE> Envie mensaje al proceso %d", suscripto->id_proceso);
				}

			free(buffer->stream);
			free(buffer);

			pthread_mutex_unlock(&mx_lista_new);
			break;

//----------------------------------------------------------------
//--------------------APPEARED------------------------------------
		case SUS_APP:
			pthread_mutex_lock(&mx_lista_appeared);
			//Suscribir a la cola appeared
			mensajeACK = recibir_mensaje(socket_cliente);

			nuevo_proceso = malloc(sizeof(Proceso));
			nuevo_proceso->id_proceso = atoi(mensajeACK);
			id = nuevo_proceso->id_proceso;
			nuevo_proceso->socket = socket_cliente;


			log_info(logger, "<SUSCRIPCION> Se suscribio a la cola APPEARED el proceso con id %d con socket %d ", nuevo_proceso->id_proceso, nuevo_proceso->socket);

			if(id != 99){
				actualizar_lista_suscritos(SUSCRITOS_APPEARED, nuevo_proceso);
				indice = buscar_proceso_en_enviados_por_id(id, SUSCRITOS_APPEARED);
				proceso = list_get(SUSCRITOS_APPEARED, indice);

				enviar_mensajes_en_memoria(nuevo_proceso, "APPEARED");
			}else
				enviar_mensajes_en_memoria(nuevo_proceso, "APPEARED");


			pthread_mutex_unlock(&mx_lista_appeared);
			free(mensajeACK);


			break;

		case APPEARED_POKEMON:
			log_info(logger,"<MENSAJE> Llego un nuevo mensaje a la cola APPEARED.");
			pthread_mutex_lock(&mx_lista_appeared);

			mensaje = recibir_mensaje_struct(socket_cliente);

			mensaje->id_mensaje = asignar_id();

			es_misma = es_misma_respuesta(mensaje->id_mensaje_correlativo);

			almacenar_mensaje(mensaje, "APPEARED");

			agregar_enviados(mensaje, SUSCRITOS_APPEARED);

			buffer = serializar_mensaje_struct(mensaje);

			//Notifico el id del mensaje
			enviar_mensaje_struct(buffer, socket_cliente, APPEARED_POKEMON);

			if(!es_misma)
				for(int i=0; i<list_size(SUSCRITOS_APPEARED); i++){
					Proceso* suscripto = list_get(SUSCRITOS_APPEARED, i);
					enviar_mensaje_struct(buffer, suscripto->socket, APPEARED_POKEMON);
					log_info(logger,"MENSAJE> Envie mensaje al proceso %d", suscripto->id_proceso);
				}

			free(buffer->stream);
			free(buffer);

			pthread_mutex_unlock(&mx_lista_appeared);
			break;

//----------------------------------------------------------------
//--------------------ACK-----------------------------------------
		case ACK:
			mensajeACK = recibir_mensaje(socket_cliente);
			char** ids = string_split(mensajeACK, "-");
			int id_proceso = atoi(ids[0]);
			int id_mensaje = atoi(ids[1]);
			log_info(logger, "<ACK> Recibi el ack del proceso %d del mensaje %d", id_proceso, id_mensaje);
			guardar_ACK(id_proceso, id_mensaje);
			free(mensajeACK);
			freeDoblePuntero(ids);
			break;
//-----------------------------------------------------------------
		case SUSCRIBIR:
			mensajeACK = recibir_mensaje(socket_cliente);
			log_info(logger,"<CONEXION> Se conecto el proceso %s al broker", mensajeACK);
			free(mensajeACK);
			break;
		case 0:
			break;
		case -1:
			pthread_exit(NULL);
			break;
		default:
			break;
		}

}


uint32_t asignar_id(){
pthread_mutex_lock(&mx_id_mensaje);

	uint32_t id = contador_de_id;
	contador_de_id++;

pthread_mutex_unlock(&mx_id_mensaje);

	return id;
}

bool es_misma_respuesta(int id_correlativo){
	for(int i=0; i<list_size(particiones); i++){
		Particion* p = list_get(particiones, i);
		if(id_correlativo != 0 && p->id_mensaje_correlativo == id_correlativo)
			return 1;
	}

	return 0;
}

t_list* mensajes_a_enviar(){

	t_list* a_enviar = list_create();

	for(int i=0; i<list_size(particiones); i++){
		Particion* p = list_get(particiones, i);
		bool ya_se_envio = 0;

		for(int j=0; j<list_size(a_enviar); j++){
			Particion* enviar = list_get(a_enviar, j);
			if(p->id_mensaje_correlativo != 0 && p->id_mensaje_correlativo == enviar->id_mensaje_correlativo){
				ya_se_envio = 1;
			}
		}

		if(!ya_se_envio)
			list_add(a_enviar, p);

	}

	return a_enviar;
}


void actualizar_lista_suscritos(t_list* lista, Proceso* nuevo_proceso){

	bool se_reconecto = 0;

	for(int i=0; i<list_size(lista); i++){
		Proceso* suscrito = list_get(lista, i);
		if(suscrito->id_proceso == nuevo_proceso->id_proceso){
			log_info(logger, "<RECONEXION> Se volio a conectar el proceso %d! Socket anterior: %d)", suscrito->id_proceso, suscrito->socket);
			suscrito->socket = nuevo_proceso->socket;
			se_reconecto = 1;
		}
	}

	if(se_reconecto)
		free(nuevo_proceso);
	else
		list_add(lista, nuevo_proceso);
}

void actualizar_lista_ack(t_list* lista, Proceso* proceso){
//TODO
}


//ADMINISTRACION DE MEMORIA

int indice_particion_libre_para_almacenar(uint32_t size){ // REOTRNA -1 SI NO HAY UNA PARTICION LIBRE QUE PUEDA ALMACENAR EL MENSAJE

	if(strcmp(algoritmo_particion_libre, "FF") == 0){
		int menor_offset = 0;
		for(int i = 0; i < list_size(particiones_libres); i++){

			int indice = indice_libre_menor_offset_mayor_a_n(menor_offset);

			ParticionLibre *particion = list_get(particiones_libres, indice);

			if(particion->size >= size)
				return indice;

			menor_offset = particion->offset_init + particion->size;

		}

	}

	if(strcmp(algoritmo_particion_libre, "BF") == 0){
//TODO TESTEAR BFFFFFFFFF
		int indice_best = -1;
		int menor_offset = 0;
		int indice;

		for(int  i=0; i < list_size(particiones_libres); i++){

			indice = indice_libre_menor_offset_mayor_a_n(menor_offset);
			ParticionLibre *particion = list_get(particiones_libres, indice);

			if(particion->size == size)
				return indice;

			if(indice_best == -1){

				if(particion->size > size)
					indice_best = indice;

			} else {

				ParticionLibre* best =  list_get(particiones_libres, indice_best);

				int resto = particion->size - size;
				int best_resto = best->size - size;

				if(particion->size > size && resto < best_resto)
					indice_best = i;

			}
			menor_offset = particion->offset_init + particion->size;
			indice = indice_libre_menor_offset_mayor_a_n(menor_offset);
		}

		return indice_best;
	}

	return -1;

}

void almacenar_mensaje(t_mensaje* mensaje, char* cola){

pthread_mutex_lock(&mx_memoria);

	int size;
	if(strcmp(cola, "NEW") == 0)
		size = sizeof(uint32_t) + mensaje->pokemon_length - 1 + 3 * sizeof(uint32_t);
	if(strcmp(cola, "GET") == 0)
		size =  sizeof(uint32_t) + mensaje->pokemon_length - 1;
	if(strcmp(cola, "APPEARED") == 0 || strcmp(cola, "CATCH") == 0 )
		size = sizeof(uint32_t) + mensaje->pokemon_length - 1 + 2 * sizeof(uint32_t);
	if(strcmp(cola, "CAUGHT") == 0)
		size = sizeof(uint32_t);

	if(size < tamanio_minimo)
		size = tamanio_minimo;


	if(strcmp(algoritmo_memoria, "PARTICIONES") == 0)
		almacenar_particion(mensaje, cola, size);

	if(strcmp(algoritmo_memoria, "BS") == 0){
		size = potencia_dos_mas_cercana(size);
		almacenar_buddy(mensaje, cola, size);
	}

pthread_mutex_unlock(&mx_memoria);

}

void almacenar_mensaje_localized(t_mensaje_get* mensaje, char* cola){

pthread_mutex_lock(&mx_memoria);

	int size;

	size = sizeof(uint32_t) + mensaje->pokemon_length - 1 + sizeof(uint32_t) + mensaje->cantidad * 2 * sizeof(uint32_t);

	if(size < tamanio_minimo)
		size = tamanio_minimo;

	if(strcmp(algoritmo_memoria, "PARTICIONES") == 0)
		almacenar_particion_localized(mensaje, cola, size);

	if(strcmp(algoritmo_memoria, "BS") == 0){
		size = potencia_dos_mas_cercana(size);
		almacenar_buddy_localized(mensaje, cola, size);
	}

pthread_mutex_unlock(&mx_memoria);

}


void almacenar_particion_localized(t_mensaje_get* mensaje, char* cola, int size){

	int contador_elminados = 0;

	int i = indice_particion_libre_para_almacenar(size);

	while(i == -1){
		if(frecuencia_compactacion != -1 && contador_elminados >= frecuencia_compactacion){
				compactar_memoria();
				contador_elminados = 0;
				i = indice_particion_libre_para_almacenar(size);
			}

		if(i == -1){
			borrar_particion();
			contador_elminados++;
			i = indice_particion_libre_para_almacenar(size);
		}
	}

	cachear_mensaje_localized(mensaje, i);
}

void almacenar_buddy_localized(t_mensaje_get* mensaje, char* cola, int size){

	int indice = indice_buddy_libre_para_almacenar(size);

	while(indice == -1){

		borrar_buddy();

		indice = indice_buddy_libre_para_almacenar(size);
	}

	//Buddy* buddy = list_get(buddies, indice);

	almacenar_en_hijo_si_corresponde_localized(indice, mensaje, cola, size);


}

void almacenar_en_hijo_si_corresponde_localized(int indice_buddy, t_mensaje_get* mensaje, char* cola, int size){

	Buddy* buddy = list_get(buddies, indice_buddy);

	if( size != buddy->size ){
		int id_hijo = dividir_buddy(buddy);
		int i_hijo = buscar_buddy_por_id(id_hijo);
		//Buddy* hijo = list_get(buddies, i_hijo);
		almacenar_en_hijo_si_corresponde_localized(i_hijo, mensaje, cola, size);
	}else{
	cachear_mensaje_localized(mensaje, indice_buddy);
	}


}

void almacenar_particion(t_mensaje* mensaje, char* cola, int size){

	int contador_elminados = 0;

	int i = indice_particion_libre_para_almacenar(size);

	while(i == -1){
		if(frecuencia_compactacion != -1 && contador_elminados >= frecuencia_compactacion){
				compactar_memoria();
				contador_elminados = 0;
				i = indice_particion_libre_para_almacenar(size);
			}

		if(i == -1){
			borrar_particion();
			contador_elminados++;
			i = indice_particion_libre_para_almacenar(size);
		}
	}


	if(strcmp(cola, "NEW") == 0)
		cachear_mensaje_new(mensaje, i);
	/*if(strcmp(cola,"LOCALIZED") == 0)
		cachear_mensaje_localized(mensaje, i);*/
	if(strcmp(cola, "GET") == 0)
		cachear_mensaje_get(mensaje, i);
	if(strcmp(cola, "APPEARED") == 0 || strcmp(cola, "CATCH") == 0 )
		cachear_mensaje_appeared_or_catch(mensaje, i, cola);
	if(strcmp(cola, "CAUGHT") == 0)
		cachear_mensaje_caught(mensaje, i);


}

void borrar_particion(){
	int indice_victima = 0;

	if(strcmp(algoritmo_reemplazo, "FIFO") == 0)
		delete_particion(indice_victima);

	if(strcmp(algoritmo_reemplazo, "LRU") == 0){

		for(int i=0; i<list_size(particiones); i++){
			Particion* p = list_get(particiones, i);
			Particion* lru = list_get(particiones, indice_victima);

			if(p->tiempo_lru < lru->tiempo_lru)
				indice_victima = i;
		}

		delete_particion(indice_victima);

	}

}

void delete_particion(int i){ //Borra la particion y crea una libre en su lugar

		Particion *p = list_remove(particiones, i);
		bool consolido = 0;
		int indice_consolidado = -1;

		//CONSOLIDAR A DERECHA
		for(int i=0; i<list_size(particiones_libres); i++){
			ParticionLibre *libre = list_get(particiones_libres, i);
			int suma_derecha = p->offset_end + 1;
			if( suma_derecha == libre->offset_init ){
				libre->offset_init = p->offset_init;
				libre->size += p->offset_end - p->offset_init + 1;
				indice_consolidado = i;
				consolido = 1;
				break;
			}
		}

		//CONSOLIDAR A IZQUIERDA
		for(int i=0; i<list_size(particiones_libres); i++){
			ParticionLibre *libre = list_get(particiones_libres, i);
			int suma_izquierda = libre->offset_init + libre->size - 1 + 1;
			if( suma_izquierda == p->offset_init ){
				if(consolido){
					ParticionLibre *l = list_remove(particiones_libres, indice_consolidado);
					libre->size += l->size;
					free(l);
				}else
					libre->size += p->offset_end - p->offset_init + 1;

				consolido = 1;
				break;
			}
		}

		//SI NO CONSOLIDO SE CREA UNA PARTICION LIBRE
		if(!consolido){
			ParticionLibre *new_pl = malloc(sizeof(ParticionLibre));
			new_pl->offset_init = p->offset_init;

			if(p->size < tamanio_minimo)
					new_pl->size = tamanio_minimo;
			else
				new_pl->size = p->size;

			list_add(particiones_libres, new_pl);
		}

		log_info(logger,"<DELETE> Se elimino la particion %d con base %d ", p->id_particion, p->offset_init);

		list_destroy(p->suscriptores_ack);
		list_destroy(p->suscriptores_enviados);
		//free(p->cola);
		free(p);

}


void unificar_libres(){  //Se utiliza en compactacion

	for(int i=0; i < list_size(particiones_libres)-1; i++){
			ParticionLibre *libre = list_get(particiones_libres, i);
			ParticionLibre *next_libre = list_get(particiones_libres, i+1);
			libre->size += next_libre->size;
			list_remove(particiones_libres, i+1);
			free(next_libre);
			i--;
	}

}


int indice_menor_offset_mayor_a_n(int n){

	int indice;

	for(int i=0; i<list_size(particiones); i++){
		Particion *p2 = list_get(particiones, i);
		if(p2->offset_init >= n){
			indice = i;
			break;
		}
	}

	for(int i=0; i<list_size(particiones); i++){

		Particion *p = list_get(particiones, indice);
		Particion *p2 = list_get(particiones, i);

		if(p2->offset_init >= n &&  p2->offset_init <= p->offset_init)
			indice = i;
	}

	return indice;
}

int indice_libre_menor_offset_mayor_a_n(int n){

	int indice;

	for(int i=0; i<list_size(particiones_libres); i++){
		ParticionLibre *p2 = list_get(particiones_libres, i);
		if(p2->offset_init >= n){
			indice = i;
			break;
		}
	}

	for(int i=0; i<list_size(particiones_libres); i++){

		ParticionLibre* p = list_get(particiones_libres, indice);
		ParticionLibre* p2 = list_get(particiones_libres, i);

		if(p2->offset_init >= n &&  p2->offset_init < p->offset_init)
			indice = i;
	}

	return indice;
}

int mover_particiones(){ //Retorna el offset donde deberian comenzar la particiones libres

	int menor_offset = 0;

	for(int i=0; i < list_size(particiones); i++){

		int indice = indice_menor_offset_mayor_a_n(menor_offset);

		Particion *p = list_get(particiones, indice);

		memcpy(memoria + menor_offset, memoria + p->offset_init, p->size);

		p->offset_init = menor_offset;

		if(p->size < tamanio_minimo)
			p->offset_end = p->offset_init + tamanio_minimo - 1;
		else
			p->offset_end = p->offset_init + p->size - 1;

		menor_offset = p->offset_end + 1;
	}

	return menor_offset;
}


void compactar_memoria(){
	unificar_libres();
	int offset = mover_particiones();
	ParticionLibre *libre = list_get(particiones_libres, 0);
	libre->offset_init = offset;

	log_info(logger,"<COMPACTACION> Se compacto la memoria!");
}




//------------------------------------------//--BUDDY SYSTEM--//----------------------------------------

int potencia_dos_mas_cercana(int size){

	int potencia = 1;

	while( size > potencia ){
		potencia *= 2;
	}

	return potencia;
}

void almacenar_buddy(t_mensaje* mensaje, char* cola, int size){

	int indice = indice_buddy_libre_para_almacenar(size);

	while(indice == -1){

		borrar_buddy();

		indice = indice_buddy_libre_para_almacenar(size);
	}

	//Buddy* buddy = list_get(buddies, indice);

	almacenar_en_hijo_si_corresponde(indice, mensaje, cola, size);


}

void almacenar_en_hijo_si_corresponde(int indice_buddy, t_mensaje* mensaje, char* cola, int size){

	Buddy* buddy = list_get(buddies, indice_buddy);

	if( size != buddy->size ){
		int id_hijo = dividir_buddy(buddy);
		int i_hijo = buscar_buddy_por_id(id_hijo);
		//Buddy* hijo = list_get(buddies, i_hijo);
		almacenar_en_hijo_si_corresponde(i_hijo, mensaje, cola, size);
	}else{

		if(strcmp(cola, "NEW") == 0)
			cachear_mensaje_new(mensaje, indice_buddy);
		if(strcmp(cola, "GET") == 0)
			cachear_mensaje_get(mensaje, indice_buddy);
		if(strcmp(cola, "APPEARED") == 0 || strcmp(cola, "CATCH") == 0 )
			cachear_mensaje_appeared_or_catch(mensaje, indice_buddy, cola);
		if(strcmp(cola, "CAUGHT") == 0)
			cachear_mensaje_caught(mensaje, indice_buddy);

	}


}


void borrar_buddy(){

	if(strcmp(algoritmo_reemplazo, "FIFO") == 0){

		Particion* p = list_get(particiones, 0);
		for(int i=0; i<list_size(buddies); i++){
			Buddy* b = list_get(buddies, i);
			if(!b->esta_libre && b->id_hijo1 == -1 && b->id_hijo2 == -1)
				if(b->particion->id_particion == p->id_particion){
					delete_buddy(b);
					break;
				}
		}
	}


	if(strcmp(algoritmo_reemplazo, "LRU") == 0){
		int indice_victima = 0;
		for(int i=0; i<list_size(particiones); i++){
			Particion* p = list_get(particiones, i);
			Particion* lru = list_get(particiones, indice_victima);

			if(p->tiempo_lru < lru->tiempo_lru)
				indice_victima = i;
		}

		Particion* p = list_get(particiones, indice_victima);

		int id_buddy = buscar_buddy_por_id_particion(p->id_particion);

		Buddy* b = list_get(buddies, id_buddy);

		delete_buddy(b);
	}


}

void delete_buddy(Buddy* buddy){

	int indice = buscar_particion_por_id(buddy->particion->id_particion);

	Particion* p = list_remove(particiones, indice);
	log_info(logger,"<DELETE> Se elimino la particion %d con base %d ", p->id_particion, p->offset_init);
	list_destroy(p->suscriptores_ack);
	list_destroy(p->suscriptores_enviados);
	//free(p->cola);


	free(p);

	buddy->esta_libre = 1;

	consolidar_buddies(buddy);
}

void consolidar_buddies(Buddy* buddy1){
	// CONSOLIDAR LOS BUDDIES SI CORRESPONDE
	if(buddy1->id_buddy != -1){
		int i_buddy2= buscar_buddy_por_id( buddy1->id_buddy );
		Buddy* buddy2 = list_get(buddies, i_buddy2);

		if(buddy1->esta_libre && buddy2->esta_libre){
			int i_padre = buscar_buddy_por_id(buddy1->id_padre);
			Buddy* padre = list_get(buddies, i_padre);

			padre->esta_libre = 1;

			padre->id_hijo1 = -1;
			padre->id_hijo2 = -1;

			list_remove(buddies, i_buddy2);
			int i_buddy = buscar_buddy_por_id(buddy1->id);
			list_remove(buddies, i_buddy);

			log_info(logger,"<CONSOLIDACION BUDDY> Se consolidaron el buddy con base %d y el buddy con base %d", buddy1->offset_init, buddy2->offset_init);

			free(buddy1);
			free(buddy2);

			if(padre->id != 0)
				consolidar_buddies(padre);

		}

	}

}

int dividir_buddy(Buddy* buddy){ // DIVIDE EL BUDDY Y DEVUELVE EL ID DEL PRIMER HIJO

	buddy->esta_libre = 0;

	Buddy* hijo1 = malloc(sizeof(Buddy));
	hijo1->size = buddy->size / 2;
	hijo1->esta_libre = 1;
	hijo1->offset_init = buddy->offset_init;
	hijo1->id = contador_id_buddy;
	contador_id_buddy++;

	Buddy* hijo2 = malloc(sizeof(Buddy));
	hijo2->size = buddy->size / 2;
	hijo2->esta_libre = 1;
	hijo2->offset_init = buddy->offset_init + buddy->size / 2;
	hijo2->id = contador_id_buddy;
	contador_id_buddy++;

	hijo1->id_buddy = hijo2->id;
	hijo1->id_padre = buddy->id;
	hijo1->id_hijo1 = -1;
	hijo1->id_hijo2 = -1;

	hijo2->id_buddy = hijo1->id;
	hijo2->id_padre = buddy->id;
	hijo2->id_hijo1 = -1;
	hijo2->id_hijo2 = -1;


	buddy->id_hijo1 = hijo1->id;
	buddy->id_hijo2 = hijo2->id;

	list_add(buddies, hijo1);
	list_add(buddies, hijo2);


	return hijo1->id;
}


int indice_buddy_libre_para_almacenar(int size){

	if(strcmp(algoritmo_particion_libre, "FF") == 0)
		for(int i=0; i<list_size(buddies); i++){
			Buddy* b = list_get(buddies, i);
			if(b->esta_libre && b->size >= size)
				return i;

			int id = tiene_hijos_libres_con_espacio(b, size);
			if(id != -1)
				return buscar_buddy_por_id(id);

		}

	if(strcmp(algoritmo_particion_libre, "BF") == 0){

		int indice_best_buddy = -1; // SE FIJA CUAL EL ES BUDDY QUE MENOS DIVISIONES TIENE QUE HACER PARA ALMACENAR EL MENSAJE

		for(int i=0; i<list_size(buddies); i++){

			Buddy* b = list_get(buddies, i);

			if(b->esta_libre && b->size == memory_size)
				return 0;

			Buddy* best;
			if(indice_best_buddy == -1)
				best = list_get(buddies, 0);
			else
				best = list_get(buddies, indice_best_buddy);

			if(b->esta_libre && b->size == size)
				return i;

			if(b->esta_libre && b->size >= size && b->size < best->size)
				indice_best_buddy = i;

		}

		return indice_best_buddy;
	}


	return -1;
}

int tiene_hijos_libres_con_espacio(Buddy* padre, int size){ // RETRONA -1 SI NO TIENE, SI TIENE RETORNA EL ID

	int respuesta;

	if(padre->id_hijo1 == -1 && padre->id_hijo2 == -1)
		return -1;


// EVALUO EN EL PRIMER HIJO
	int i_hijo1 = buscar_buddy_por_id(padre->id_hijo1);
	Buddy* hijo1  = list_get(buddies, i_hijo1);

	if( hijo1->esta_libre && hijo1->size >= size)
		return hijo1->id;


	if( !hijo1->esta_libre ){
		respuesta = tiene_hijos_libres_con_espacio(hijo1, size);
		if( respuesta != -1)
			return respuesta;
	}


// EVALUO EN EL SEGUNDO HIJO
	int i_hijo2 = buscar_buddy_por_id(padre->id_hijo2);
	Buddy* hijo2  = list_get(buddies, i_hijo2);

	if( hijo2->esta_libre && hijo2->size >= size)
		return hijo2->id;

	if( !hijo2->esta_libre ){
		respuesta = tiene_hijos_libres_con_espacio(hijo2, size);
		if(respuesta != -1)
			return respuesta;
	}


	return -1;
}


int buscar_buddy_por_id(int id_buddy){
	for(int i=0; i<list_size(buddies); i++){
		Buddy* b = list_get(buddies, i);
		if(b->id == id_buddy)
			return i;
	}

	return -1;
}

int buscar_particion_por_id(int id){
	for(int i=0; i<list_size(particiones); i++){
			Particion* p = list_get(particiones, i);
			if(p->id_particion == id)
				return i;
	}

	return -1;
}

int buscar_buddy_por_id_particion(int id_particion){

	for(int i=0; i<list_size(buddies); i++){
		Buddy* b = list_get(buddies, i);
		if(!b->esta_libre && b->id_hijo1 == -1 && b->id_hijo2 == -1)
			if(b->particion->id_particion == id_particion)
				return i;
	}

	return -1;
}

/////////////////////









// ENVIO DE MENSAJES

int mensaje_mas_antiguo_mayor_a_n(int n, t_list* lista){ //RETORNA EL i DE LA PARTICION CON EL ID_MANSAJE MAS CHICO
	int i_fifo ;
	for(int i=0; i<list_size(lista); i++){
		Particion* p = list_get(lista, i);
		if(p->id_mensaje > n){
			i_fifo = i;
			break;
		}
	}


	for(int i=0; i<list_size(lista); i++){
		Particion* p = list_get(lista, i);
		Particion* fifo = list_get(lista, i_fifo);
		if(p->id_mensaje > n && p->id_mensaje < fifo->id_mensaje){
			i_fifo = i;
		}
	}

	return i_fifo;
}

void enviar_mensajes_en_memoria(Proceso* proceso, char* cola){
	pthread_mutex_lock(&mx_memoria);
	int n = 0;

	t_list* a_enviar = mensajes_a_enviar();

	/*for(int i=0; i<list_size(particiones); i++){

		int indice = mensaje_mas_antiguo_mayor_a_n(n);

		Particion* p = list_get(particiones, indice);

		n = p->id_mensaje;
		*/
	for(int i=0; i<list_size(a_enviar); i++){

		int indice = mensaje_mas_antiguo_mayor_a_n(n, a_enviar);

		Particion* p = list_get(a_enviar, indice);

		n = p->id_mensaje;

		if(strcmp(p->cola, cola) == 0)

			if( !devolvio_ack(proceso,p) ){

				list_add(p->suscriptores_enviados, proceso);

				t_buffer* buffer;

				if(strcmp(cola, "LOCALIZED") == 0){
					t_mensaje_get* mensaje = leer_particion(p);
					buffer = serializar_mensaje_struct_get(mensaje);
				}else{
					t_mensaje* mensaje = leer_particion(p);
					buffer = serializar_mensaje_struct(mensaje);
				}


				if(strcmp(cola, "NEW") == 0)
					enviar_mensaje_struct(buffer, proceso->socket, NEW_POKEMON);
				if(strcmp(cola, "LOCALIZED") == 0)
					enviar_mensaje_struct(buffer, proceso->socket, LOCALIZED_POKEMON);//TODO
				if(strcmp(cola, "GET") == 0)
					enviar_mensaje_struct(buffer, proceso->socket, GET_POKEMON);
				if(strcmp(cola, "APPEARED") == 0)
					enviar_mensaje_struct(buffer, proceso->socket, APPEARED_POKEMON);
				if(strcmp(cola, "CATCH") == 0)
					enviar_mensaje_struct(buffer, proceso->socket, CATCH_POKEMON);
				if(strcmp(cola, "CAUGHT") == 0)
					enviar_mensaje_struct(buffer, proceso->socket, CAUGHT_POKEMON);

				free(buffer->stream);
				free(buffer);

				log_info(logger, "<MENSAJE> Envie el mensaje %d al proceso %d", p->id_mensaje, proceso->id_proceso);

				p->tiempo_lru = timestamp();

			}

	}

	list_destroy(a_enviar);

	pthread_mutex_unlock(&mx_memoria);
}

bool se_le_envio_el_mensaje(Proceso* proceso, Particion* particion){


	for(int i=0; i<list_size(particion->suscriptores_enviados); i++){
		Proceso* enviado = list_get(particion->suscriptores_enviados, i);
		if(enviado->id_proceso == proceso->id_proceso)
			return 1;
	}

	return 0;
}

bool devolvio_ack(Proceso* proceso, Particion* particion){
	for(int i=0; i<list_size(particion->suscriptores_ack); i++){
		Proceso* enviado = list_get(particion->suscriptores_ack, i);
		if(enviado->id_proceso == proceso->id_proceso)
			return 1;
	}

	return 0;
}




void* leer_particion(Particion* p){

//pthread_mutex_lock(&mx_memoria);

char* barra_cero = "\0";

if(strcmp(p->cola, "LOCALIZED") == 0 ){

	t_mensaje_get* mensaje = malloc(sizeof(t_mensaje_get));
	mensaje->pokemon = malloc(sizeof(char));
	mensaje->id_mensaje = p->id_mensaje;
	mensaje->id_mensaje_correlativo = p->id_mensaje_correlativo;


	memcpy(&mensaje->pokemon_length, memoria + p->offset_init, sizeof(uint32_t)); //LENGTH
	mensaje->pokemon = realloc(mensaje->pokemon, mensaje->pokemon_length + 1);
	memcpy(mensaje->pokemon, memoria + p->offset_init + sizeof(uint32_t), mensaje->pokemon_length); //POKEMON
	memcpy(mensaje->pokemon + mensaje->pokemon_length, barra_cero, 1); // (agrego el \0)
	memcpy(&mensaje->cantidad, memoria + p->offset_init + sizeof(uint32_t) + mensaje->pokemon_length, sizeof(uint32_t)); // CANTIDAD

	char* posiciones = string_new();
	int i = 0;
	int offset_init = p->offset_init + sizeof(uint32_t) + mensaje->pokemon_length + sizeof(uint32_t);
	Coordenadas* coordenadas = malloc(sizeof(Coordenadas));

	while( i < mensaje->cantidad){


		memcpy(&coordenadas->x, memoria + offset_init, sizeof(uint32_t)); // POS X
		offset_init += sizeof(uint32_t);
		memcpy(&coordenadas->y, memoria + offset_init, sizeof(uint32_t)); // POS Y
		offset_init += sizeof(uint32_t);

		if(i==0){
			string_append_with_format(&posiciones, "%d", coordenadas->x );
			string_append_with_format(&posiciones, "-%d", coordenadas->y );
		}else{
			string_append_with_format(&posiciones, ".%d", coordenadas->x );
			string_append_with_format(&posiciones, "-%d", coordenadas->y );
		}
		i++;
	}

	free(coordenadas);

	mensaje->posiciones_length = strlen(posiciones) + 1;
	mensaje->posiciones = malloc(mensaje->posiciones_length);
	memcpy(mensaje->posiciones, posiciones, mensaje->posiciones_length);

	mensaje->pokemon_length += 1;

//pthread_mutex_unlock(&mx_memoria);
	return mensaje;

}

	t_mensaje* mensaje = malloc(sizeof(t_mensaje));
	mensaje->pokemon = malloc(sizeof(char));
	mensaje->id_mensaje = p->id_mensaje;
	mensaje->id_mensaje_correlativo = p->id_mensaje_correlativo;


	if(strcmp(p->cola, "GET") == 0){
		memcpy(&mensaje->pokemon_length, memoria + p->offset_init, sizeof(uint32_t)); //LENGTH
		mensaje->pokemon = realloc(mensaje->pokemon, mensaje->pokemon_length + 1);
		memcpy(mensaje->pokemon, memoria + p->offset_init + sizeof(uint32_t), mensaje->pokemon_length); // POKEMON
		memcpy(mensaje->pokemon + mensaje->pokemon_length, barra_cero, 1); // (agrego el \0)
		mensaje->pokemon_length += 1;

		mensaje->cantidad = 0;
		mensaje->posx = 0;
		mensaje->posy = 0;
		mensaje->resultado = 0;
	}

	if(strcmp(p->cola, "NEW") == 0){
		memcpy(&mensaje->pokemon_length, memoria + p->offset_init, sizeof(uint32_t)); //LENGTH
		mensaje->pokemon = realloc(mensaje->pokemon, mensaje->pokemon_length + 1);
		memcpy(mensaje->pokemon, memoria + p->offset_init + sizeof(uint32_t), mensaje->pokemon_length); //POKEMON
		memcpy(mensaje->pokemon + mensaje->pokemon_length, barra_cero, 1); // (agrego el \0)
		memcpy(&mensaje->posx, memoria + p->offset_init + sizeof(uint32_t) + mensaje->pokemon_length, sizeof(uint32_t)); // POS X
		memcpy(&mensaje->posy, memoria + p->offset_init + 2 * sizeof(uint32_t) + mensaje->pokemon_length, sizeof(uint32_t)); // POS Y
		memcpy(&mensaje->cantidad, memoria + p->offset_init + 3 * sizeof(uint32_t) + mensaje->pokemon_length, sizeof(uint32_t)); //CANTIDAD
		mensaje->pokemon_length += 1;

		mensaje->resultado = 0;
	}

	if(strcmp(p->cola, "APPEARED") == 0 || strcmp(p->cola, "CATCH") == 0){
		memcpy(&mensaje->pokemon_length, memoria + p->offset_init, sizeof(uint32_t)); //LENGTH
		mensaje->pokemon = realloc(mensaje->pokemon, mensaje->pokemon_length + 1);
		memcpy(mensaje->pokemon, memoria + p->offset_init + sizeof(uint32_t), mensaje->pokemon_length); //POKEMON
		memcpy(mensaje->pokemon + mensaje->pokemon_length, barra_cero, 1); // (agrego el \0)
		memcpy(&mensaje->posx, memoria + p->offset_init + sizeof(uint32_t) + mensaje->pokemon_length, sizeof(uint32_t)); //POS X
		memcpy(&mensaje->posy, memoria + p->offset_init + 2 * sizeof(uint32_t) + mensaje->pokemon_length, sizeof(uint32_t)); // POS Y
		mensaje->pokemon_length += 1;

		mensaje->cantidad = 0;
		mensaje->resultado = 0;
	}

	if(strcmp(p->cola,"CAUGHT") == 0){
		memcpy(&mensaje->resultado, memoria + p->offset_init, sizeof(uint32_t));//RESPUESTA

		memcpy(mensaje->pokemon, barra_cero, 1); // (agrego el \0 para que este inicializada la variable a la hora de enviarla)
		mensaje->pokemon_length = 1;
		mensaje->cantidad = 0;
		mensaje->posx = 0;
		mensaje->posy = 0;
	}

//pthread_mutex_unlock(&mx_memoria);

	return mensaje;
}


void mostrar(){

pthread_mutex_lock(&mx_mostrar);

	log_info(logger,"//////////////////////////////////////////////////////////////////////////");

	if(strcmp(algoritmo_memoria, "PARTICIONES") == 0){
		for(int i=0; i<list_size(particiones_libres); i++){
			ParticionLibre *l = list_get(particiones_libres, i);
			log_info(logger,"<LIBRE> size(%d) offset init (%d) ", l->size, l->offset_init);
		}
		log_info(logger,"----------------------------------------------------------------------");
	}

	if(strcmp(algoritmo_memoria, "BS") == 0){
		for(int i=0; i<list_size(buddies); i++){
			Buddy* b = list_get(buddies, i);
			if(!b->esta_libre && b->id_hijo1 == -1 && b->id_hijo2 == -1)
				log_info(logger, "<BUDDY> id(%d <- %d) -- size(%d) --%d-- id_particion(%d)", b->id, b->id_padre, b->size, b->esta_libre, b->particion->id_particion);
			else
				log_info(logger, "<BUDDY> id(%d <- %d) -- size(%d) --%d-- ", b->id, b->id_padre, b->size, b->esta_libre);
		}
		log_info(logger,"----------------------------------------------------------------------");
	}


	for(int i=0; i<list_size(particiones); i++){

		Particion *p = list_get(particiones, i);

		if(strcmp(p->cola, "LOCALIZED") == 0){
			t_mensaje_get* mostrar = leer_particion(p);

			log_info(logger, "<MENSAJE> cola(%s) pokemon(%s) longitud(%d) cantidad_posiciones(%d) posiciones(%s)-- lru(%" PRIu64 ") -- Particion %d offset_init(%d)",
					p->cola, mostrar->pokemon, mostrar->pokemon_length-1, mostrar->cantidad, mostrar->posiciones, p->tiempo_lru, p->id_particion, p->offset_init);
			free(mostrar->pokemon);
			free(mostrar->posiciones);
			free(mostrar);

		}else{

			t_mensaje* mostrar = leer_particion(p);

			if(strcmp(p->cola, "GET") == 0)
				log_info(logger,"<MENSAJE> cola(%s) pokemon(%s) longitud(%d) -- lru(%" PRIu64 ") -- Particion %d offset_init(%d)",
						p->cola, mostrar->pokemon, mostrar->pokemon_length-1, p->tiempo_lru, p->id_particion, p->offset_init);

			if(strcmp(p->cola, "NEW") == 0)
				log_info(logger,"<MENSAJE> cola (%s) pokemon(%s) longitud(%d) cantidad(%d) pos(%d,%d) -- lru(%" PRIu64 ")-- Particion %d offset_init(%d)",
						p->cola, mostrar->pokemon, mostrar->pokemon_length-1, mostrar->cantidad, mostrar->posx, mostrar->posy, p->tiempo_lru, p->id_particion, p->offset_init);

			if(strcmp(p->cola, "APPEARED") == 0 || strcmp(p->cola, "CATCH") == 0)
				log_info(logger,"<MENSAJE> cola (%s) -> nombre pokemon(%s)  longitud(%d) pos(%d,%d) -- lru(%" PRIu64 ") -- Particion offset_init(%d)",
						p->cola, mostrar->pokemon, mostrar->pokemon_length-1, mostrar->posx, mostrar->posy, p->tiempo_lru, p->offset_init);

			if(strcmp(p->cola,"CAUGHT") == 0)
				log_info(logger,"<MENSAJE> cola (%s) -> respuesta(%d) -- lru(%" PRIu64 ") -- Particion offset_init(%d)",
						p->cola, mostrar->resultado, p->tiempo_lru, p->offset_init);

			free(mostrar->pokemon);
			free(mostrar);
		}


	}

	log_info(logger,"//////////////////////////////////////////////////////////////////////////");

pthread_mutex_unlock(&mx_mostrar);

}


void guardar_ACK(int id_proceso, int id_mensaje){
//QUE PASA SI ME LLEGA UN ACK DE UN MENSAJE QUE NO TENGO EN MEMORIA??

	for(int i=0; i<list_size(particiones); i++){

		Particion* p = list_get(particiones, i);

		if(id_mensaje == p->id_mensaje){

			int indice = buscar_proceso_en_enviados_por_id(id_proceso, p->suscriptores_enviados);
			Proceso* proceso = list_get(p->suscriptores_enviados, indice);
			list_add(p->suscriptores_ack, proceso);

		}
	}
}

int buscar_proceso_en_enviados_por_id(int id_proceso, t_list* enviados){
	for(int i=0; i<list_size(enviados); i++){
		Proceso* proceso = list_get(enviados, i);
		if(proceso->id_proceso == id_proceso)
			return i;
	}
	return -1;
}

void agregar_enviados(t_mensaje* mensaje, t_list* lista){

	int indice = buscar_particion_por_id_mensaje(mensaje->id_mensaje);
	Particion* p = list_get(particiones, indice);

	for(int i=0; i<list_size(lista); i++){
		Proceso* proceso = list_get(lista,i);
		list_add(p->suscriptores_enviados, proceso);
	}
}

void agregar_enviados_localized(t_mensaje_get* mensaje, t_list* lista){

	int indice = buscar_particion_por_id_mensaje(mensaje->id_mensaje);
	Particion* p = list_get(particiones, indice);

	for(int i=0; i<list_size(lista); i++){
		Proceso* proceso = list_get(lista,i);
		list_add(p->suscriptores_enviados, proceso);
	}
}

int buscar_particion_por_id_mensaje(int id_mensaje){

	for(int i=0; i<list_size(particiones); i++){
			Particion* p = list_get(particiones, i);
			if(p->id_mensaje == id_mensaje)
				return i;
		}

	return -1;
}


///////////////////////////




// CACHEADAS

void cachear_mensaje_new(t_mensaje *msg, int indice_libre){ //uint32 largo, nombre, 3 * uint32 cantiad y posicion

	int offset_init;

	if(strcmp(algoritmo_memoria, "PARTICIONES") == 0){
		ParticionLibre *libre = list_get(particiones_libres, indice_libre);
		offset_init = libre->offset_init;
	}

	if(strcmp(algoritmo_memoria, "BS") == 0){
		Buddy* buddy = list_get(buddies, indice_libre);
		offset_init = buddy->offset_init;
	}


//Crea una nueva particion
	Particion *new_particion = malloc(sizeof(Particion));
	//new_particion->cola = malloc(sizeof(char));

	new_particion->cola = "NEW";
	new_particion->id_mensaje = msg->id_mensaje;
	new_particion->offset_init = offset_init;
	new_particion->size = sizeof(uint32_t) + msg->pokemon_length - 1 + 3 * sizeof(uint32_t);
	new_particion->suscriptores_ack = list_create();
	new_particion->suscriptores_enviados = list_create();
	if(new_particion->size < tamanio_minimo)
			new_particion->offset_end = new_particion->offset_init + tamanio_minimo - 1;
		else
			new_particion->offset_end = new_particion->offset_init + new_particion->size - 1;
	new_particion->id_particion = contador_id_particiones;
	contador_id_particiones++;
	new_particion->tiempo_lru = timestamp();
	new_particion->id_mensaje_correlativo = msg->id_mensaje_correlativo;
//Termina de crear la particion

	msg->pokemon_length -=1;

	memcpy(memoria + offset_init, &msg->pokemon_length, sizeof(uint32_t)); // LENGTH
	offset_init += sizeof(uint32_t);
	memcpy(memoria + offset_init, msg->pokemon, msg->pokemon_length); // NOMBRE
	offset_init += msg->pokemon_length;
	memcpy(memoria + offset_init, &msg->posx, sizeof(uint32_t)); // POS X
	offset_init += sizeof(uint32_t);
	memcpy(memoria + offset_init, &msg->posy, sizeof(uint32_t)); // POS Y
	offset_init += sizeof(uint32_t);
	memcpy(memoria + offset_init, &msg->cantidad, sizeof(uint32_t)); // CANTIDAD
	offset_init += sizeof(uint32_t);

	msg->pokemon_length += 1;

	log_info(logger,"<ALMACENAMIENTO> Se almaceno el mensaje, la particion inicia en la posicion %d", new_particion->offset_init);

	if(strcmp(algoritmo_memoria, "PARTICIONES") == 0){
		ParticionLibre *libre = list_get(particiones_libres, indice_libre);
		libre->offset_init = new_particion->offset_end + 1;

		if(new_particion->size < tamanio_minimo)
			libre->size -= tamanio_minimo;
		else
			libre->size -= new_particion->size;

		if( libre->size <= 0 ){
			list_remove(particiones_libres, indice_libre);
			free(libre);
		}
	}


	if(strcmp(algoritmo_memoria, "BS") == 0){
		Buddy* buddy = list_get(buddies, indice_libre);
		buddy->particion = new_particion;
		buddy->esta_libre = 0;
	}

	list_add(particiones, new_particion);

}

void cachear_mensaje_get(t_mensaje *msg, int indice_libre){

	//char* cola = "GET";

	int offset_init;

	if(strcmp(algoritmo_memoria, "PARTICIONES") == 0){
		ParticionLibre *libre = list_get(particiones_libres, indice_libre);
		offset_init = libre->offset_init;
	}

	if(strcmp(algoritmo_memoria, "BS") == 0){
		Buddy* buddy = list_get(buddies, indice_libre);
		offset_init = buddy->offset_init;
	}


//Crea una nueva particion
	Particion *new_particion = malloc(sizeof(Particion));
	//new_particion->cola = string_new();
	//new_particion->cola = malloc(sizeof(char));

	new_particion->offset_init = offset_init;
	new_particion->size = sizeof(uint32_t) + msg->pokemon_length - 1;
	new_particion->id_mensaje = msg->id_mensaje;
	//new_particion->cola = realloc(new_particion, strlen(cola)-1);
	//memcpy(new_particion->cola, cola, strlen(cola));
	new_particion->cola = "GET";
	new_particion->suscriptores_ack = list_create();
	new_particion->suscriptores_enviados = list_create();
	if(new_particion->size < tamanio_minimo)
			new_particion->offset_end = new_particion->offset_init + tamanio_minimo - 1;
		else
			new_particion->offset_end = new_particion->offset_init + new_particion->size - 1;
	new_particion->id_particion = contador_id_particiones;
	contador_id_particiones++;
	new_particion->tiempo_lru = timestamp();
	new_particion->id_mensaje_correlativo = msg->id_mensaje_correlativo;
//Termina de crear la particion

	msg->pokemon_length -= 1;

	memcpy(memoria + offset_init, &msg->pokemon_length, sizeof(uint32_t));
	offset_init += sizeof(uint32_t);
	memcpy(memoria + offset_init, msg->pokemon, msg->pokemon_length);

	msg->pokemon_length += 1;

	log_info(logger,"<ALMACENAMIENTO> Se almaceno el mensaje, la particion inicia en la posicion %d", new_particion->offset_init);

	if(strcmp(algoritmo_memoria, "PARTICIONES") == 0){
		ParticionLibre *libre = list_get(particiones_libres, indice_libre);
		libre->offset_init = new_particion->offset_end + 1;

		if(new_particion->size < tamanio_minimo)
			libre->size -= tamanio_minimo;
		else
			libre->size -= new_particion->size;

		if( libre->size <= 0 ){
			list_remove(particiones_libres, indice_libre);
			free(libre);
		}
	}


	if(strcmp(algoritmo_memoria, "BS") == 0){
		Buddy* buddy = list_get(buddies, indice_libre);
		buddy->particion = new_particion;
		buddy->esta_libre = 0;
	}

	list_add(particiones, new_particion);

}

void cachear_mensaje_localized(t_mensaje_get *msg, int indice_libre){

int offset_init;

if(strcmp(algoritmo_memoria, "PARTICIONES") == 0){
	ParticionLibre *libre = list_get(particiones_libres, indice_libre);
	offset_init = libre->offset_init;
}

if(strcmp(algoritmo_memoria, "BS") == 0){
	Buddy* buddy = list_get(buddies, indice_libre);
	offset_init = buddy->offset_init;
}


//Crea una nueva particion
Particion *new_particion = malloc(sizeof(Particion));
//new_particion->cola = malloc(sizeof(char));

new_particion->cola = "LOCALIZED";
new_particion->id_mensaje = msg->id_mensaje;
new_particion->offset_init = offset_init;
new_particion->size = sizeof(uint32_t) + msg->pokemon_length - 1 + sizeof(uint32_t) + msg->cantidad * 2 * sizeof(uint32_t);
new_particion->suscriptores_ack = list_create();
new_particion->suscriptores_enviados = list_create();
if(new_particion->size < tamanio_minimo)
		new_particion->offset_end = new_particion->offset_init + tamanio_minimo - 1;
	else
		new_particion->offset_end = new_particion->offset_init + new_particion->size - 1;
new_particion->id_particion = contador_id_particiones;
contador_id_particiones++;
new_particion->tiempo_lru = timestamp();
new_particion->id_mensaje_correlativo = msg->id_mensaje_correlativo;
//Termina de crear la particion


msg->pokemon_length -= 1;

memcpy(memoria + offset_init, &msg->pokemon_length, sizeof(uint32_t)); // LENGTH
offset_init += sizeof(uint32_t);
memcpy(memoria + offset_init, msg->pokemon, msg->pokemon_length); // NOMBRE
offset_init += msg->pokemon_length;
memcpy(memoria + offset_init, &msg->cantidad, sizeof(uint32_t)); // CANTIDAD
offset_init += sizeof(uint32_t);

msg->pokemon_length += 1;

log_info(logger,"<ALMACENAMIENTO> Se almaceno el mensaje, la particion inicia en la posicion %d", new_particion->offset_init);

int i = 0;
char** aux = string_split(msg->posiciones, ".");
while(aux[i] != NULL){
	char** posiciones = string_split(aux[i], "-");

	int posx = atoi(posiciones[0]);
	int posy = atoi(posiciones[1]);

	memcpy(memoria + offset_init, &posx, sizeof(uint32_t)); // POS X
	offset_init += sizeof(uint32_t);
	memcpy(memoria + offset_init, &posy, sizeof(uint32_t)); // POS Y
	offset_init += sizeof(uint32_t);

	freeDoblePuntero(posiciones);
	i++;
}

freeDoblePuntero(aux);

if(strcmp(algoritmo_memoria, "PARTICIONES") == 0){
	ParticionLibre *libre = list_get(particiones_libres, indice_libre);
	libre->offset_init = new_particion->offset_end + 1;

	if(new_particion->size < tamanio_minimo)
		libre->size -= tamanio_minimo;
	else
		libre->size -= new_particion->size;

	if( libre->size <= 0 ){
		list_remove(particiones_libres, indice_libre);
		free(libre);
	}
}


if(strcmp(algoritmo_memoria, "BS") == 0){
	Buddy* buddy = list_get(buddies, indice_libre);
	buddy->particion = new_particion;
	buddy->esta_libre = 0;
}

list_add(particiones, new_particion);

}

void cachear_mensaje_appeared_or_catch(t_mensaje* msg, int indice_libre, char* cola){ //uint32 largo, nombre, 2 * uint32 posicion


	int offset_init;

	if(strcmp(algoritmo_memoria, "PARTICIONES") == 0){
		ParticionLibre *libre = list_get(particiones_libres, indice_libre);
		offset_init = libre->offset_init;
	}

	if(strcmp(algoritmo_memoria, "BS") == 0){
		Buddy* buddy = list_get(buddies, indice_libre);
		offset_init = buddy->offset_init;
	}


//Crea una nueva particion
	Particion *new_particion = malloc(sizeof(Particion));
	//new_particion->cola = malloc(sizeof(char));

	if(strcmp(cola, "CATCH") == 0)
		new_particion->cola = "CATCH";
	if(strcmp(cola, "APPEARED") == 0)
		new_particion->cola = "APPEARED";
	new_particion->id_mensaje = msg->id_mensaje;
	new_particion->offset_init = offset_init;
	new_particion->size = sizeof(uint32_t) + msg->pokemon_length - 1 + 2 * sizeof(uint32_t);
	new_particion->suscriptores_ack = list_create();
	new_particion->suscriptores_enviados = list_create();
	if(new_particion->size < tamanio_minimo)
			new_particion->offset_end = new_particion->offset_init + tamanio_minimo - 1;
		else
			new_particion->offset_end = new_particion->offset_init + new_particion->size - 1;
	new_particion->id_particion = contador_id_particiones;
	contador_id_particiones++;
	new_particion->tiempo_lru = timestamp();
	new_particion->id_mensaje_correlativo = msg->id_mensaje_correlativo;
//Termina de crear la particion

	msg->pokemon_length -= 1;

	memcpy(memoria + offset_init, &msg->pokemon_length, sizeof(uint32_t)); // LENGTH
	offset_init += sizeof(uint32_t);
	memcpy(memoria + offset_init, msg->pokemon, msg->pokemon_length); // NOMBRE
	offset_init += msg->pokemon_length;
	memcpy(memoria + offset_init, &msg->posx, sizeof(uint32_t)); // POS X
	offset_init += sizeof(uint32_t);
	memcpy(memoria + offset_init, &msg->posy, sizeof(uint32_t)); // POS Y
	offset_init += sizeof(uint32_t);

	msg->pokemon_length += 1;

	log_info(logger,"<ALMACENAMIENTO> Se almaceno el mensaje, la particion inicia en la posicion %d", new_particion->offset_init);


	if(strcmp(algoritmo_memoria, "PARTICIONES") == 0){
		ParticionLibre *libre = list_get(particiones_libres, indice_libre);
		libre->offset_init = new_particion->offset_end + 1;

		if(new_particion->size < tamanio_minimo)
			libre->size -= tamanio_minimo;
		else
			libre->size -= new_particion->size;

		if( libre->size <= 0 ){
			list_remove(particiones_libres, indice_libre);
			free(libre);
		}
	}


	if(strcmp(algoritmo_memoria, "BS") == 0){
		Buddy* buddy = list_get(buddies, indice_libre);
		buddy->particion = new_particion;
		buddy->esta_libre = 0;
	}

	list_add(particiones, new_particion);

}

void cachear_mensaje_caught(t_mensaje *msg, int indice_libre){ //uint32 respuesta


	int offset_init;

	if(strcmp(algoritmo_memoria, "PARTICIONES") == 0){
		ParticionLibre *libre = list_get(particiones_libres, indice_libre);
		offset_init = libre->offset_init;
	}

	if(strcmp(algoritmo_memoria, "BS") == 0){
		Buddy* buddy = list_get(buddies, indice_libre);
		offset_init = buddy->offset_init;
	}


//Crea una nueva particion
	Particion *new_particion = malloc(sizeof(Particion));
	//new_particion->cola = malloc(sizeof(char));

	new_particion->cola = "CAUGHT";
	new_particion->id_mensaje = msg->id_mensaje;
	new_particion->offset_init = offset_init;
	new_particion->size = sizeof(uint32_t);
	new_particion->suscriptores_ack = list_create();
	new_particion->suscriptores_enviados = list_create();
	if(new_particion->size < tamanio_minimo)
			new_particion->offset_end = new_particion->offset_init + tamanio_minimo - 1;
		else
			new_particion->offset_end = new_particion->offset_init + new_particion->size - 1;
	new_particion->id_particion = contador_id_particiones;
	contador_id_particiones++;
	new_particion->tiempo_lru = timestamp();
	new_particion->id_mensaje_correlativo = msg->id_mensaje_correlativo;
//Termina de crear la particion


	memcpy(memoria + offset_init, &msg->resultado, sizeof(uint32_t)); // RESPUESTA

	log_info(logger,"<ALMACENAMIENTO> Se almaceno el mensaje, la particion inicia en la posicion %d", new_particion->offset_init);

	if(strcmp(algoritmo_memoria, "PARTICIONES") == 0){
		ParticionLibre *libre = list_get(particiones_libres, indice_libre);
		libre->offset_init = new_particion->offset_end + 1;

		if(new_particion->size < tamanio_minimo)
			libre->size -= tamanio_minimo;
		else
			libre->size -= new_particion->size;

		if( libre->size <= 0 ){
			list_remove(particiones_libres, indice_libre);
			free(libre);
		}
	}


	if(strcmp(algoritmo_memoria, "BS") == 0){
		Buddy* buddy = list_get(buddies, indice_libre);
		buddy->particion = new_particion;
		buddy->esta_libre = 0;
	}


	list_add(particiones, new_particion);

}
