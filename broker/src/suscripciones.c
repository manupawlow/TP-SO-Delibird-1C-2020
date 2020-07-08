
#include "suscripciones.h"

uint8_t asignar_id(){
	contador_de_id++;
	return contador_de_id - 1;
}

void process_request(Colas *loquito) {
    int cod_op;
	t_mensaje* mensaje = malloc(sizeof(t_mensaje));
	t_buffer *buffer;
	int* size;

	if(recv(loquito->socket_cliente, &cod_op, sizeof(op_code), MSG_WAITALL) == -1)
			cod_op = -1;

	log_info(loquito->logger,"cod op: %d", cod_op);

		switch (cod_op) {


//--------------------GET-----------------------------------
		case SUS_GET:
			//Suscribir a la cola get
			loquito->SUSCRITOS_GET[loquito->cant_suscritos_get] = loquito->socket_cliente;
			log_info(loquito->logger,"Se Suscribio a la lista GET %d", loquito->SUSCRITOS_GET[loquito->cant_suscritos_get]);
			loquito->cant_suscritos_get++;
			break;

		case GET_POKEMON:
			//Agrega el mensaje a la cola get

			mensaje = recibir_mensaje_struct(loquito->socket_cliente);
			mensaje->id_mensaje = asignar_id();
			buffer = serializar_mensaje_struct(mensaje);

			for(int i=0; i< loquito->cant_suscritos_get; i++)
				enviar_mensaje_struct(buffer,loquito->SUSCRITOS_GET[i],GET_POKEMON);


			log_info(loquito->logger,"Se envio mensaje a todos los suscriptos!");
			log_info(loquito->logger,"id mensaje: %d, proximo id %d", mensaje->id_mensaje, contador_de_id);
	/*		ParticionLibre *pp = list_get(particiones_libres, 0);
			log_info(loquito->logger,"cantidad particiones libres ANTES: %d %d ", list_size(particiones_libres), pp->size);
			//juntar_libres_adyacentes();
			unificar_libres();
			log_info(loquito->logger,"cantidad particiones libres ANTES: %d %d", list_size(particiones_libres), pp->size);
	*/





/*
			ParticionLibre *l = list_get(particiones_libres, 0 );
			log_info(loquito->logger,"Particion libre antes de cahear: size(%d) offset(%d)", l->size, l->offset_init);

			//bucar particion libre
			//indice = indice_particion_libre_para_almacenar(mensaje->pokemon_length + sizeof(uint32_t));
			cachear_mensaje_get(mensaje, 0);

			log_info(loquito->logger,"Particion libre despues de cachear: size(%d) offset(%d)", l->size, l->offset_init);

			Particion *p = list_get(particiones, 0);

			msg_get* mostrar = malloc(sizeof(msg_get));
			mostrar->nombre_pokemon = malloc(sizeof(char));

			memcpy(&mostrar->longitud_nombre, memoria + p->offset_init, sizeof(uint32_t));
			memcpy(mostrar->nombre_pokemon, memoria + p->offset_init + sizeof(uint32_t), mostrar->longitud_nombre);

			log_info(loquito->logger,"longitud nombre pokemon: %d", mostrar->longitud_nombre);
			log_info(loquito->logger,"nombre pokemon: %s", mostrar->nombre_pokemon);


			log_info(loquito->logger,"Particion: id(%d)", p->id_particion);
			log_info(loquito->logger,"Particion: offset(%d)", p->offset_init);
			log_info(loquito->logger,"Particion: size(%d)", p->size);
			free(mostrar);
*/
			break;


//----------------------------------------------------------------
//--------------------LOCALIZED-----------------------------------
		case SUS_LOC:
			//Suscribir a la cola Localized
			loquito->SUSCRITOS_LOCALIZED[loquito->cant_suscritos_localized] = loquito->socket_cliente;
			log_info(loquito->logger,"Se Suscribio a la lista LOCALIZED %d", loquito->SUSCRITOS_LOCALIZED[loquito->cant_suscritos_localized]);
			loquito->cant_suscritos_localized++;
			break;

		case LOCALIZED_POKEMON:
			//Agrega el mensaje a la cola localized

			mensaje = recibir_mensaje_struct(loquito->socket_cliente);
			mensaje->id_mensaje = asignar_id();
			buffer = serializar_mensaje_struct(mensaje);

			for(int i=0; i< loquito->cant_suscritos_localized; i++){
				enviar_mensaje_struct(buffer,loquito->SUSCRITOS_LOCALIZED[i],LOCALIZED_POKEMON);

			}
			log_info(loquito->logger,"Se envio mensaje a todos los suscriptos!");
			// W mutex de los id;
			//asignar_id(msg); //////////
			// S mutex de los id;



			//le digo el id del mensaje al que mando el msg

			//id = asignar_id();

			//devolver_mensaje(id, size, loquito->socket_cliente, cod_op);

			//log_info(loquito->logger, "Se notifico el id del mensaje!");

			//free(msg);
			break;


//----------------------------------------------------------------
//--------------------CATCH---------------------------------------
		case SUS_CATCH:
			//Suscribir a la cola catch
			loquito->SUSCRITOS_CATCH[loquito->cant_suscritos_catch] = loquito->socket_cliente;
			log_info(loquito->logger,"Se Suscribio a la lista CATCH %d", loquito->SUSCRITOS_CATCH[loquito->cant_suscritos_catch]);
			loquito->cant_suscritos_catch++;
			break;

		case CATCH_POKEMON:
			//Agrega el mensaje a la cola catch

			mensaje = recibir_mensaje_struct(loquito->socket_cliente);
			mensaje->id_mensaje = asignar_id();
			buffer = serializar_mensaje_struct(mensaje);

			enviar_mensaje_struct(buffer,loquito->socket_cliente,CATCH_POKEMON);




			log_info(loquito->logger,"Se envio mensaje a todos los suscriptos!");
			// W mutex de los id;
			//asignar_id(msg); //////////
			// S mutex de los id;



			//le digo el id del mensaje al que mando el msg

			//id = asignar_id();

			//devolver_mensaje(id, size, loquito->socket_cliente, cod_op);

			//log_info(loquito->logger, "Se notifico el id del mensaje!");

			//free(msg);
			break;
//----------------------------------------------------------------
//--------------------CAUGHT--------------------------------------
		case SUS_CAUGHT:
			//Suscribir a la cola caught
			loquito->SUSCRITOS_CAUGHT[loquito->cant_suscritos_caught] = loquito->socket_cliente;
			log_info(loquito->logger,"Se Suscribio a la lista CAUGHT %d", loquito->SUSCRITOS_CAUGHT[loquito->cant_suscritos_caught]);
			loquito->cant_suscritos_caught++;
			break;

		case CAUGHT_POKEMON:
			//Agrega el mensaje a la cola caugth

			mensaje = recibir_mensaje_struct(loquito->socket_cliente);
			mensaje->id_mensaje = asignar_id();
			buffer = serializar_mensaje_struct(mensaje);

			for(int i=0; i< loquito->cant_suscritos_appeared; i++){
				enviar_mensaje_struct(buffer,loquito->SUSCRITOS_CAUGHT[i],CAUGHT_POKEMON);

			}
			log_info(loquito->logger,"Se envio mensaje a todos los suscriptos!");
			// W mutex de los id;
			//asignar_id(msg); //////////
			// S mutex de los id;



			//le digo el id del mensaje al que mando el msg

			//id = asignar_id();

			//devolver_mensaje(id, size, loquito->socket_cliente, cod_op);

			//log_info(loquito->logger, "Se notifico el id del mensaje!");

			//free(msg);
			break;

//----------------------------------------------------------------
//--------------------NEW-----------------------------------------
		case SUS_NEW:
			//Suscribir a la cola new
			loquito->SUSCRITOS_NEW[loquito->cant_suscritos_new] = loquito->socket_cliente;
			log_info(loquito->logger,"Se Suscribio a la lista NEW %d", loquito->SUSCRITOS_NEW[loquito->cant_suscritos_new]);
			loquito->cant_suscritos_new++;
			break;

		case NEW_POKEMON:
			//Agrega el mensaje a la cola new
			mensaje = recibir_mensaje_struct(loquito->socket_cliente);
			mensaje->id_mensaje = asignar_id();
			buffer = serializar_mensaje_struct(mensaje);

			for(int i=0; i< loquito->cant_suscritos_new; i++){
				enviar_mensaje_struct(buffer,loquito->SUSCRITOS_NEW[i],NEW_POKEMON);

			}
			log_info(loquito->logger,"Se envio mensaje a todos los suscriptos!");
			// W mutex de los id;
			//asignar_id(msg); //////////
			// S mutex de los id;



			//le digo el id del mensaje al que mando el msg

			//id = asignar_id();

			//devolver_mensaje(id, size, loquito->socket_cliente, cod_op);

			//log_info(loquito->logger, "Se notifico el id del mensaje!");

			//free(msg);
			break;

//----------------------------------------------------------------
//--------------------APPEARED------------------------------------
		case SUS_APP:
			//Suscribir a la cola appeared
			loquito->SUSCRITOS_APPEARED[loquito->cant_suscritos_appeared] = loquito->socket_cliente;
			log_info(loquito->logger,"Se Suscribio a la lista APPEARED %d", loquito->SUSCRITOS_APPEARED[loquito->cant_suscritos_appeared]);
			loquito->cant_suscritos_appeared++;
			break;

		case APPEARED_POKEMON:

			// W mutex de los id;
			//asignar_id(msg); //////////
			// S mutex de los id;
			mensaje = recibir_mensaje_struct(loquito->socket_cliente);
			mensaje->id_mensaje = asignar_id();
			buffer = serializar_mensaje_struct(mensaje);

			for(int i=0; i< loquito->cant_suscritos_appeared; i++){
				enviar_mensaje_struct(buffer,loquito->SUSCRITOS_APPEARED[i],APPEARED_POKEMON);
			}

			//enviar_a_todos(loquito->SUSCRITOS_APPEARED, loquito->cant_suscritos_appeared, msg, size, cod_op );
			log_info(loquito->logger,"Se envio mensaje a todos los suscriptos!");

			//le digo el id del mensaje al que mando el msg
/*
			id = asignar_id();

			devolver_mensaje(id, size, loquito->socket_cliente, cod_op);

			log_info(loquito->logger, "Se notifico el id del mensaje!");
*/
			//free(msg);

			break;

//----------------------------------------------------------------
//--------------------ACK-----------------------------------------
		case ACK:
           	mensaje = recibir_mensaje(loquito->socket_cliente,size);
			log_info(loquito->logger,"Me llego el mensaje");
			break;
//-----------------------------------------------------------------
		case SUSCRIBIR:
			mensaje = recibir_mensaje_struct(loquito->socket_cliente);
			log_info(loquito->logger,"%d",mensaje->posx);
			log_info(loquito->logger,"%d",mensaje->posy);
			log_info(loquito->logger,"%s",mensaje->pokemon);
			log_info(loquito->logger,"%s",mensaje->resultado);
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



void cachear_mensaje_get(t_mensaje *msg, int indice_libre){
	//W de la memoria

	Particion *new_particion = malloc(sizeof(Particion));

	ParticionLibre *libre = list_get(particiones_libres, indice_libre);

	msg_get* mensaje = malloc(sizeof(msg_get));
	mensaje->nombre_pokemon = malloc(strlen(msg->pokemon) + 1);
	mensaje->longitud_nombre = strlen(msg->pokemon) + 1;

	memcpy(mensaje->nombre_pokemon, msg->pokemon, mensaje->longitud_nombre);
	//mensaje->nombre_pokemon = string_duplicate(msg->pokemon);

	new_particion->offset_init = libre->offset_init;
	new_particion->size = sizeof(uint32_t) + mensaje->longitud_nombre;
	new_particion->id_particion = contador_id_particiones;
	contador_id_particiones++;

	//memcpy(memoria + libre->offset_init, mensaje, new_particion->size);
	memcpy(memoria + libre->offset_init, &mensaje->longitud_nombre, sizeof(uint32_t));
	libre->offset_init += sizeof(uint32_t);
	memcpy(memoria + libre->offset_init, mensaje->nombre_pokemon, mensaje->longitud_nombre);
	libre->offset_init += mensaje->longitud_nombre;

	libre->size -= new_particion->size;

	if( libre->size <= 0 ){
		list_remove(particiones_libres, indice_libre);
		free(libre);
	}

	list_add(particiones, new_particion);
	free(mensaje);
	//S de la memoria
}

int indice_particion_libre_para_almacenar(uint32_t size){ //POR AHORA ES FIFO

	for(int i = 0; i < list_size(particiones_libres); i++){
		ParticionLibre *particion = list_get(particiones_libres, i);
		if(particion->size >= size)
			return i;
	}

	return -1;
}

void borrar_particion_FIFO(){

	//FIFO
	int indice_a_borrar = 0;
	/*for(int i=0; i<list_size(particiones); i++){
		Particion *p = list_get(particiones, i);
		Particion *borrar = list_get(particiones, indice_a_borrar);
		if(p->id_particion > borrar->id_particion)
			indice_a_borrar = i;
	}*/

	borrar_particion(indice_a_borrar);
}



void borrar_particion(int i){ //Borra la particion y crea una libre en su lugar

	ParticionLibre *new_pl = malloc(sizeof(ParticionLibre));
	Particion *p = list_remove(particiones, i);

	new_pl->offset_init = p->offset_init;
	new_pl->size = p->size;

	list_add(particiones_libres, new_pl);

	free(p);
}

void juntar_libres_adyacentes(){
	if(list_size(particiones_libres) > 1){

		for(int i=0; i < list_size(particiones_libres)-1; i++){
			ParticionLibre *libre = list_get(particiones_libres, i);
			ParticionLibre *next_libre = list_get(particiones_libres, i+1);
			int suma = libre->offset_init + libre->size + 1;
			if( suma >= next_libre->offset_init ){
				libre->size += next_libre->size;
				list_remove(particiones_libres, i+1);
				free(next_libre);
				i--;
			}
		}
	}
}

void unificar_libres(){

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

	int indice = 0;
	for(int i=0; i<list_size(particiones); i++){
		Particion *p = list_get(particiones, indice);
		Particion *p2 = list_get(particiones, i);

		if(p2->offset_init >= n &&  p2->offset_init <= p->offset_init)
			indice = i;
	}

	return indice;
}

int mover_particiones(){

	int menor_offset = 0;

	for(int i=0; i < list_size(particiones); i++){

		int indice = indice_menor_offset_mayor_a_n(menor_offset);

		Particion *p = list_get(particiones, indice);

		p->offset_init = menor_offset;

		//FALTA HACER EL MEMCPY

		menor_offset += p->size + 1;
	}

	return menor_offset;
}

void compactar_memoria(){
	unificar_libres();
	int offset = mover_particiones();
	ParticionLibre *libre = list_get(particiones_libres, 0);
	libre->offset_init = offset;
}

/*

}
void guardar_ACK(Mensaje msg){
	confirmados_ACK[cant_confirmados] = msg->id;
	cant_confirmados++;
}


//dentro de gamecard (y team) va a tener que haber una funcion asignar_id_correlativo(msg_catch, msg_caugth)
//que agarre ya haga:    msg_caugth->id_correlativo = msg_catch.id    o algo asi

*/




