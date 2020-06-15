
#include "suscripciones.h"



void enviar_a_todos(int suscritos[], int cant, char* msg,int size, int cod_op){
	for(int i = 0; i < cant; i++)
		devolver_mensaje(msg, size, suscritos[i], cod_op);
}


char* asignar_id(){
	int id = contador_de_id;
	//contador_de_id++;
	char *hola = "-1";
	if(id == 0)
		return "0";
	if(id == 1)
		return "1";
	if(id == 2)
		return "2";
	if(id == 3)
		return "3";
	if(id == 4)
		return "4";
	if(id == 5)
		return "5 (ultimo id)";

	return hola;
}

void process_request(Colas *loquito) {
    int cod_op;
    char *msg;
    int size;
    char *id;
	t_mensaje* mensaje = malloc(sizeof(t_mensaje));
	t_buffer *buffer;

	if(recv(loquito->socket_cliente, &cod_op, sizeof(op_code), MSG_WAITALL) == -1)
			cod_op = -1;

		switch (cod_op) {

//----------------------------------------------------------
//--------------------GET-----------------------------------
		case SUS_GET:
			//Suscribir a la cola get
			loquito->SUSCRITOS_GET[loquito->cant_suscritos_get] = loquito->socket_cliente;
			log_info(loquito->logger,"Se Suscribio a la lista GET %d", loquito->SUSCRITOS_GET[loquito->cant_suscritos_get]);
			loquito->cant_suscritos_get++;
			break;

		case GET_POKEMON:
			//Agrega el mensaje a la cola get
			//msg = recibir_mensaje(loquito->socket_cliente, &size);

			// W mutex de los id;
			//asignar_id(msg); //////////
			// S mutex de los id;
			mensaje = recibir_mensaje_struct(loquito->socket_cliente);
			log_info(loquito->logger,"el pokemon es %s",mensaje->pokemon);
			buffer = serializar_mensaje_struct(mensaje);

			for(int i=0; i< loquito->cant_suscritos_get; i++){
				enviar_mensaje_struct(buffer,loquito->SUSCRITOS_GET[i],cod_op);
			}
			//enviar_a_todos(loquito->SUSCRITOS_GET, loquito->cant_suscritos_get, msg, size, cod_op );
			log_info(loquito->logger,"Se envio mensaje a todos los suscriptos!");

			//le digo el id del mensaje al que mando el msg

			/*id = asignar_id();

			devolver_mensaje(id, size, loquito->socket_cliente, cod_op);

			log_info(loquito->logger, "Se notifico el id del mensaje!");*/

			//free(mensaje);
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
			msg = recibir_mensaje(loquito->socket_cliente, &size);

			// W mutex de los id;
			//asignar_id(msg); //////////
			// S mutex de los id;

			enviar_a_todos(loquito->SUSCRITOS_LOCALIZED, loquito->cant_suscritos_localized, msg, size, cod_op );
			log_info(loquito->logger,"Se envio mensaje a todos los suscriptos!");

			//le digo el id del mensaje al que mando el msg

			id = asignar_id();

			devolver_mensaje(id, size, loquito->socket_cliente, cod_op);

			log_info(loquito->logger, "Se notifico el id del mensaje!");

			free(msg);
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
			//msg = recibir_mensaje(loquito->socket_cliente, &size);

			// W mutex de los id;
			//asignar_id(msg); //////////
			// S mutex de los id;
			mensaje = recibir_mensaje_struct(loquito->socket_cliente);
			log_info(loquito->logger,"el pokemon es %s",mensaje->pokemon);
			buffer = serializar_mensaje_struct(mensaje);

			for(int i=0; i< loquito->cant_suscritos_catch; i++){
				enviar_mensaje_struct(buffer,loquito->SUSCRITOS_CATCH[i],cod_op);
			}
						//enviar_a_todos(loquito->SUSCRITOS_GET, loquito->cant_suscritos_get, msg, size, cod_op );
						log_info(loquito->logger,"Se envio mensaje a todos los suscriptos!");
			//enviar_a_todos(loquito->SUSCRITOS_CATCH, loquito->cant_suscritos_catch, msg, size, cod_op );
			//log_info(loquito->logger,"Se envio mensaje a todos los suscriptos!");

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
			buffer = serializar_mensaje_struct(mensaje);

			for(int i=0; i< loquito->cant_suscritos_caught; i++){
				enviar_mensaje_struct(buffer,loquito->SUSCRITOS_CAUGHT[i],CAUGHT_POKEMON);

			}
			// W mutex de los id;
			//asignar_id(msg); //////////
			// S mutex de los id;

			//enviar_a_todos(loquito->SUSCRITOS_CAUGHT, loquito->cant_suscritos_caught, msg, size, cod_op );
			log_info(loquito->logger,"Se envio mensaje a todos los suscriptos!");

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
			//msg = recibir_mensaje(loquito->socket_cliente, &size);

			// W mutex de los id;
			//asignar_id(msg); //////////
			// S mutex de los id;
			mensaje = recibir_mensaje_struct(loquito->socket_cliente);
			log_info(loquito->logger,"el pokemon es %s",mensaje->pokemon);
			buffer = serializar_mensaje_struct(mensaje);

			for(int i=0; i< loquito->cant_suscritos_new; i++){
				enviar_mensaje_struct(buffer,loquito->SUSCRITOS_NEW[i],cod_op);
			}

			//enviar_a_todos(loquito->SUSCRITOS_NEW, loquito->cant_suscritos_new, msg, size, cod_op );
			log_info(loquito->logger,"Se envio mensaje a todos los suscriptos!");

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
			//mensaje = recibir_mensaje_struct(loquito->socket_cliente);
			//log_info(loquito->logger,"A gamecard le llego el mensaje");
			msg = recibir_mensaje(loquito->socket_cliente, &size);
			log_info(loquito->logger,"%s",msg);
			free(msg);
			//guardar_ACK(msg);
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

/*
void asignar_id(Mensaje *msg){

	msg.id = contador_de_id;
	contador_de_id++;

}
void guardar_ACK(Mensaje msg){
	confirmados_ACK[cant_confirmados] = msg->id;
	cant_confirmados++;
}


//dentro de gamecard (y team) va a tener que haber una funcion asignar_id_correlativo(msg_catch, msg_caugth)
//que agarre ya haga:    msg_caugth->id_correlativo = msg_catch.id    o algo asi

*/

