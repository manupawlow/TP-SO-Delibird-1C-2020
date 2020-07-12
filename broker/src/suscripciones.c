
#include "suscripciones.h"


void process_request(Colas *loquito) {
    int cod_op;
	t_mensaje* mensaje;
	t_mensaje_get* mensajeGet;
	char* mensajeACK;
	t_buffer *buffer;

	if(recv(loquito->socket_cliente, &cod_op, sizeof(op_code), MSG_WAITALL) == -1)
			cod_op = -1;

		switch (cod_op) {


//--------------------GET-----------------------------------
		case SUS_GET:
			//Suscribir a la cola get
			//enviar_mensajes_en_memoria(loquito->socket_cliente, "GET"); // 				<--- TODO VERLO CON LOS DEMAS
			//recibo mensaje cuando se suscribe y veo que id de team tiene

			list_add(loquito->SUSCRITOS_GET, (void*) loquito->socket_cliente);
			log_info(logger,"Se Suscribio a la lista GET %d", loquito->socket_cliente);

			break;

		case GET_POKEMON:
			//Agrega el mensaje a la cola get
			mensaje = recibir_mensaje_struct(loquito->socket_cliente);

			mensaje->id_mensaje = asignar_id();

			almacenar_mensaje(mensaje, "GET");
			agregar_enviados(mensaje, loquito->SUSCRITOS_GET);

			Particion* p = list_get(particiones, 0);
			p->tiempo_lru = 20;

			mostrar();

			buffer = serializar_mensaje_struct(mensaje);

			//Notifico el id del mensaje
			enviar_mensaje_struct(buffer, loquito->socket_cliente, GET_POKEMON);
																				//NO SE BORRA EL BUFFER CUANDO LE ENVIAS EL MENSAJE AL PRIMERO?
			for(int i=0; i<list_size(loquito->SUSCRITOS_GET); i++)
				enviar_mensaje_struct(buffer, list_get(loquito->SUSCRITOS_GET, i), GET_POKEMON);

			//free(buffer);

			log_info(logger,"Se envio mensaje a todos los suscriptos!");


			break;


//----------------------------------------------------------------
//--------------------LOCALIZED-----------------------------------
		case SUS_LOC:
			//Suscribir a la cola Localized
			loquito->SUSCRITOS_LOCALIZED[loquito->cant_suscritos_localized] = loquito->socket_cliente;
			log_info(logger,"Se Suscribio a la lista LOCALIZED %d", loquito->SUSCRITOS_LOCALIZED[loquito->cant_suscritos_localized]);
			loquito->cant_suscritos_localized++;
			break;

		case LOCALIZED_POKEMON:
			//Agrega el mensaje a la cola localized

			mensajeGet = recibir_mensaje_struct_get(loquito->socket_cliente);
			mensajeGet->id_mensaje = asignar_id();
			buffer = serializar_mensaje_struct_get(mensajeGet);

			for(int i=0; i< loquito->cant_suscritos_localized; i++){
				enviar_mensaje_struct(buffer,loquito->SUSCRITOS_LOCALIZED[i],LOCALIZED_POKEMON);
			}
			free(buffer->stream);
			free(buffer);
			log_info(logger,"Se envio mensaje a todos los suscriptos!");

			break;


//----------------------------------------------------------------
//--------------------CATCH---------------------------------------
		case SUS_CATCH:
			//Suscribir a la cola catch
			loquito->SUSCRITOS_CATCH[loquito->cant_suscritos_catch] = loquito->socket_cliente;
			log_info(logger,"Se Suscribio a la lista CATCH %d", loquito->SUSCRITOS_CATCH[loquito->cant_suscritos_catch]);
			loquito->cant_suscritos_catch++;
			break;

		case CATCH_POKEMON:
			//Agrega el mensaje a la cola catch

			mensaje = recibir_mensaje_struct(loquito->socket_cliente);

			mensaje->id_mensaje = asignar_id();

			almacenar_mensaje(mensaje, "CATCH");
//			agregar_enviados(mensaje, (void*) loquito->SUSCRITOS_CATCH);

			mostrar();//

			buffer = serializar_mensaje_struct(mensaje);

			//Notifico el id del mensaje
			enviar_mensaje_struct(buffer, loquito->socket_cliente, CATCH_POKEMON);

			for(int i=0; i< loquito->cant_suscritos_catch; i++){
				enviar_mensaje_struct(buffer,loquito->SUSCRITOS_CATCH[i],CATCH_POKEMON);
			}
			free(buffer->stream);
			free(buffer);
			log_info(logger,"Se envio mensaje a todos los suscriptos!");

			break;

//----------------------------------------------------------------
//--------------------CAUGHT--------------------------------------
		case SUS_CAUGHT:
			//Suscribir a la cola caught
			loquito->SUSCRITOS_CAUGHT[loquito->cant_suscritos_caught] = loquito->socket_cliente;
			log_info(logger,"Se Suscribio a la lista CAUGHT %d", loquito->SUSCRITOS_CAUGHT[loquito->cant_suscritos_caught]);
			loquito->cant_suscritos_caught++;
			break;

		case CAUGHT_POKEMON:
			//Agrega el mensaje a la cola caugth

			mensaje = recibir_mensaje_struct(loquito->socket_cliente);

			mensaje->id_mensaje = asignar_id();

			almacenar_mensaje(mensaje, "CAUGHT");
//			agregar_enviados(mensaje, loquito->SUSCRITOS_CAUGHT);

			mostrar();//

			buffer = serializar_mensaje_struct(mensaje);

			//Notifico el id del mensaje
			enviar_mensaje_struct(buffer, loquito->socket_cliente, CAUGHT_POKEMON);


			for(int i=0; i< loquito->cant_suscritos_caught; i++){
				enviar_mensaje_struct(buffer,loquito->SUSCRITOS_CAUGHT[i],CAUGHT_POKEMON);
			}
			free(buffer->stream);
			free(buffer);
			log_info(logger,"Se envio mensaje a todos los suscriptos!");

			break;

//----------------------------------------------------------------
//--------------------NEW-----------------------------------------
		case SUS_NEW:
			//Suscribir a la cola new
			loquito->SUSCRITOS_NEW[loquito->cant_suscritos_new] = loquito->socket_cliente;
			log_info(logger,"Se Suscribio a la lista NEW %d", loquito->SUSCRITOS_NEW[loquito->cant_suscritos_new]);
			loquito->cant_suscritos_new++;
			break;

		case NEW_POKEMON:
			//Agrega el mensaje a la cola new

			mensaje = recibir_mensaje_struct(loquito->socket_cliente);

			mensaje->id_mensaje = asignar_id();

			almacenar_mensaje(mensaje, "NEW");
	//		agregar_enviados(mensaje, (void*) loquito->SUSCRITOS_NEW);

			mostrar();//

			buffer = serializar_mensaje_struct(mensaje);

			//Notifico el id del mensaje
			enviar_mensaje_struct(buffer, loquito->socket_cliente, NEW_POKEMON);

			for(int i=0; i< loquito->cant_suscritos_new; i++){
				enviar_mensaje_struct(buffer,loquito->SUSCRITOS_NEW[i],NEW_POKEMON);
			}
			free(buffer->stream);
			free(buffer);

			log_info(logger,"Se envio mensaje a todos los suscriptos!");

			break;

//----------------------------------------------------------------
//--------------------APPEARED------------------------------------
		case SUS_APP:
			//Suscribir a la cola appeared
			loquito->SUSCRITOS_APPEARED[loquito->cant_suscritos_appeared] = loquito->socket_cliente;
			log_info(logger,"Se Suscribio a la lista APPEARED %d", loquito->SUSCRITOS_APPEARED[loquito->cant_suscritos_appeared]);
			loquito->cant_suscritos_appeared++;
			break;

		case APPEARED_POKEMON:

			mensaje = recibir_mensaje_struct(loquito->socket_cliente);

			mensaje->id_mensaje = asignar_id();

			almacenar_mensaje(mensaje, "APPEARED");
	//		agregar_enviados(mensaje, loquito->SUSCRITOS_APPEARED);

			mostrar();//

			buffer = serializar_mensaje_struct(mensaje);

			//Notifico el id del mensaje
			enviar_mensaje_struct(buffer, loquito->socket_cliente, APPEARED_POKEMON);


			for(int i=0; i< loquito->cant_suscritos_appeared; i++){
				enviar_mensaje_struct(buffer,loquito->SUSCRITOS_APPEARED[i],APPEARED_POKEMON);
			}
			free(buffer->stream);
			free(buffer);
			log_info(logger,"Se envio mensaje a todos los suscriptos!");

			break;

//----------------------------------------------------------------
//--------------------ACK-----------------------------------------
		case ACK:
			mensajeACK = recibir_mensaje(loquito->socket_cliente);
			//guardar_ACK(loquito->socket_cliente, mensaje);
			break;
//-----------------------------------------------------------------
		case SUSCRIBIR:
			mensaje = recibir_mensaje_struct(loquito->socket_cliente);
			log_info(logger,"%d",mensaje->posx);
			log_info(logger,"%d",mensaje->posy);
			log_info(logger,"%s",mensaje->pokemon);
			log_info(logger,"%s",mensaje->resultado);
			free(mensaje);
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

uint32_t actualizar_tiempo_lru(){
pthread_mutex_unlock(&mx_lru);
	int tiempo = tiempo_lru;
	tiempo_lru++;
pthread_mutex_unlock(&mx_lru);
	return tiempo;
}



//ADMINISTRACION DE MEMORIA

int indice_particion_libre_para_almacenar(uint32_t size){ //POR AHORA ES FIRST FIT

	if(strcmp(algoritmo_particion_libre, "FF") == 0){

		for(int i = 0; i < list_size(particiones_libres); i++){
				ParticionLibre *particion = list_get(particiones_libres, i);
				if(particion->size >= size)
					return i;
		}

	}

	if(strcmp(algoritmo_particion_libre, "BF") == 0){

		int indice_best = -1;
		int menor_offset = 0;

		for(int  i=0; i < list_size(particiones_libres); i++){

			int indice = indice_libre_menor_offset_mayor_a_n(menor_offset);
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
		}
		return indice_best;
	}

	return -1;

}

void almacenar_mensaje(t_mensaje* mensaje, char* cola){

pthread_mutex_lock(&mx_memoria);

	int size;
	if(strcmp(cola, "NEW") == 0)
		size = sizeof(uint32_t) + strlen(mensaje->pokemon) + 3 * sizeof(uint32_t);
	//TODO LOCALIZED
	if(strcmp(cola, "GET") == 0)
		size = strlen(mensaje->pokemon) + sizeof(uint32_t);
	if(strcmp(cola, "APPEARED") == 0 || strcmp(cola, "CATCH") == 0 )
		size = sizeof(uint32_t) + strlen(mensaje->pokemon) + 2 * sizeof(uint32_t);
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
	//TODO LOCALIZED
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
				libre->size += p->offset_end - p->offset_init;
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
					libre->size += p->size;

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
		//TODO LOCALIZED
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

void enviar_mensajes_en_memoria(int socket, char* cola){

	t_buffer* buffer;
	t_mensaje* mensaje;

	for(int i=0; i<list_size(particiones); i++){

		Particion* p = list_get(particiones, i);

		if(strcmp(p->cola, cola) == 0)

			if( !recibio_el_mensaje(socket, p) ){

				list_add(p->suscriptores_enviados, (void*) socket);
				mensaje = leer_particion(p);
				buffer = serializar_mensaje_struct(mensaje);

				if(strcmp(cola, "NEW") == 0)
					enviar_mensaje_struct(buffer, socket, NEW_POKEMON);
				if(strcmp(cola, "LOCALIZED") == 0)
					enviar_mensaje_struct(buffer, socket, LOCALIZED_POKEMON);
				if(strcmp(cola, "GET") == 0)
					enviar_mensaje_struct(buffer, socket, GET_POKEMON);
				if(strcmp(cola, "APPEARED") == 0)
					enviar_mensaje_struct(buffer, socket, APPEARED_POKEMON);
				if(strcmp(cola, "CATCH") == 0)
					enviar_mensaje_struct(buffer, socket, CATCH_POKEMON);
				if(strcmp(cola, "CAUGHT") == 0)
					enviar_mensaje_struct(buffer, socket, CAUGHT_POKEMON);

			}

		p->tiempo_lru = actualizar_tiempo_lru();

	}

}

bool recibio_el_mensaje(int socket, Particion* p){


	for(int i=0; i<list_size(p->suscriptores_enviados); i++){
		int enviado = list_get(p->suscriptores_enviados, i);
		if(enviado == socket)
			return 1;
	}

	return 0;
}



t_mensaje* leer_particion(Particion* p){

pthread_mutex_lock(&mx_memoria);

	t_mensaje* mensaje = malloc(sizeof(t_mensaje));
	mensaje->pokemon = malloc(sizeof(char));
	mensaje->id_mensaje = p->id_mensaje;

	if(strcmp(p->cola, "GET") == 0){
		memcpy(&mensaje->pokemon_length, memoria + p->offset_init, sizeof(uint32_t)); //LENGTH
		mensaje->pokemon = realloc(mensaje->pokemon, mensaje->pokemon_length);
		memcpy(mensaje->pokemon, memoria + p->offset_init + sizeof(uint32_t), mensaje->pokemon_length); // POKEMON
	}

	if(strcmp(p->cola, "NEW") == 0){
		memcpy(&mensaje->pokemon_length, memoria + p->offset_init, sizeof(uint32_t)); //LENGTH
		mensaje->pokemon = realloc(mensaje->pokemon, mensaje->pokemon_length);
		memcpy(mensaje->pokemon, memoria + p->offset_init + sizeof(uint32_t), mensaje->pokemon_length); //POKEMON
		memcpy(&mensaje->cantidad, memoria + p->offset_init + 2 * sizeof(uint32_t), sizeof(uint32_t)); //CANTIDAD
		memcpy(&mensaje->posx, memoria + p->offset_init + 3 * sizeof(uint32_t), sizeof(uint32_t)); // POS X
		memcpy(&mensaje->posy, memoria + p->offset_init + 4 * sizeof(uint32_t), sizeof(uint32_t)); // POS Y
	}

	//TODO APPEARED

	if(strcmp(p->cola, "APPEARED") == 0 || strcmp(p->cola, "CATCH") == 0){
		memcpy(&mensaje->pokemon_length, memoria + p->offset_init, sizeof(uint32_t)); //LENGTH
		mensaje->pokemon = realloc(mensaje->pokemon, mensaje->pokemon_length);
		memcpy(mensaje->pokemon, memoria + p->offset_init + sizeof(uint32_t), mensaje->pokemon_length); //POKEMON
		memcpy(&mensaje->posx, memoria + p->offset_init + 2 * sizeof(uint32_t), sizeof(uint32_t)); //POS X
		memcpy(&mensaje->posy, memoria + p->offset_init + 3 * sizeof(uint32_t), sizeof(uint32_t)); // POS Y
	}

	if(strcmp(p->cola,"CAUGHT") == 0){
		memcpy(&mensaje->resultado, memoria + p->offset_init, sizeof(uint32_t));//RESPUESTA
	}

pthread_mutex_unlock(&mx_memoria);

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
		t_mensaje* mostrar = leer_particion(p);

		if(strcmp(p->cola, "GET") == 0)
			log_info(logger,"<MENSAJE> cola (%s) -> nombre pokemon(%s)  longitud(%d) -- lru(%d) -- Particion offset_init(%d)", p->cola, mostrar->pokemon, mostrar->pokemon_length, p->tiempo_lru, p->offset_init);

		if(strcmp(p->cola, "NEW") == 0)
			log_info(logger,"<MENSAJE> cola (%s) -> nombre pokemon(%s)  longitud(%d) cantidad(%d) pos(%d,%d) -- Particion offset_init(%d)", p->cola, mostrar->pokemon, mostrar->pokemon_length, mostrar->cantidad, mostrar->posx, mostrar->posy, p->offset_init);

			//TODO APPEARED

		if(strcmp(p->cola, "APPEARED") == 0 || strcmp(p->cola, "CATCH") == 0)
			log_info(logger,"<MENSAJE> cola (%s) -> nombre pokemon(%s)  longitud(%d) pos(%d,%d) -- Particion offset_init(%d)", p->cola, mostrar->pokemon, mostrar->pokemon_length, mostrar->posx, mostrar->posy, p->offset_init);

		if(strcmp(p->cola,"CAUGHT") == 0)
			log_info(logger,"<MENSAJE> cola (%s) -> respuesta(%d) -- Particion offset_init(%d)", p->cola, mostrar->resultado, p->offset_init);

		free(mostrar->pokemon);
		free(mostrar);


	}

	log_info(logger,"//////////////////////////////////////////////////////////////////////////");

pthread_mutex_unlock(&mx_mostrar);

}


void guardar_ACK(int socket_cliente, t_mensaje* mensaje){
//QUE PASA SI ME LLEGA UN ACK DE UN MENSAJE QUE NO TENGO EN MEMORIA??

	for(int i=0; i<list_size(particiones); i++){

		Particion* p = list_get(particiones, i);

		if(mensaje->id_mensaje == p->id_mensaje){
			list_add(p->suscriptores_ack, (void*) socket_cliente);
		}
	}
}

void agregar_enviados(t_mensaje* mensaje, t_list* lista){

	Particion* p;

	for(int i=0; i<list_size(particiones); i++){
		p = list_get(particiones, i);
		if(p->id_mensaje == mensaje->id_mensaje)
			break;
	}

	for(int i=0; i<list_size(lista); i++)
		list_add(p->suscriptores_enviados, list_get(lista,i));
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
	new_particion->cola = malloc(sizeof(char));

	new_particion->cola = "NEW";
	new_particion->id_mensaje = msg->id_mensaje;
	new_particion->offset_init = offset_init;
	new_particion->size = sizeof(uint32_t) + msg->pokemon_length + 3 * sizeof(uint32_t);
	new_particion->suscriptores_ack = list_create();
	new_particion->suscriptores_enviados = list_create();
	if(new_particion->size < tamanio_minimo)
			new_particion->offset_end = new_particion->offset_init + tamanio_minimo - 1;
		else
			new_particion->offset_end = new_particion->offset_init + new_particion->size - 1;
	new_particion->id_particion = contador_id_particiones;
	contador_id_particiones++;
//Termina de crear la particion


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
	new_particion->size = sizeof(uint32_t) + msg->pokemon_length;
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
	new_particion->tiempo_lru = actualizar_tiempo_lru();
//Termina de crear la particion


	memcpy(memoria + offset_init, &msg->pokemon_length, sizeof(uint32_t));
	offset_init += sizeof(uint32_t);
	memcpy(memoria + offset_init, msg->pokemon, msg->pokemon_length);
	offset_init += msg->pokemon_length;


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

// TODO void cachear_mensaje_localized(t_mensaje *msg, int indice_libre)

void cachear_mensaje_appeared_or_catch(t_mensaje *msg, int indice_libre, char* cola){ //uint32 largo, nombre, 2 * uint32 posicion


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
	new_particion->cola = malloc(sizeof(char));

	if(strcmp(cola, "CATCH") == 0)
		new_particion->cola = "CATCH";
	if(strcmp(cola, "APPEARED") == 0)
		new_particion->cola = "APPEARED";
	new_particion->id_mensaje = msg->id_mensaje;
	new_particion->offset_init = offset_init;
	new_particion->size = sizeof(uint32_t) + msg->pokemon_length + 2 * sizeof(uint32_t);
	new_particion->suscriptores_ack = list_create();
	new_particion->suscriptores_enviados = list_create();
	if(new_particion->size < tamanio_minimo)
			new_particion->offset_end = new_particion->offset_init + tamanio_minimo - 1;
		else
			new_particion->offset_end = new_particion->offset_init + new_particion->size - 1;
	new_particion->id_particion = contador_id_particiones;
	contador_id_particiones++;
//Termina de crear la particion


	memcpy(memoria + offset_init, &msg->pokemon_length, sizeof(uint32_t)); // LENGTH
	offset_init += sizeof(uint32_t);
	memcpy(memoria + offset_init, msg->pokemon, msg->pokemon_length); // NOMBRE
	offset_init += msg->pokemon_length;
	memcpy(memoria + offset_init, &msg->posx, sizeof(uint32_t)); // POS X
	offset_init += sizeof(uint32_t);
	memcpy(memoria + offset_init, &msg->posy, sizeof(uint32_t)); // POS Y
	offset_init += sizeof(uint32_t);


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
	new_particion->cola = malloc(sizeof(char));

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
//Termina de crear la particion


	memcpy(memoria + offset_init, &msg->pokemon_length, sizeof(uint32_t)); // RESPUESTA
	offset_init += sizeof(uint32_t);

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







