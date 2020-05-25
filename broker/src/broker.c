#include "/home/utnso/tp-2020-1c-NN/biblioteca/biblioteca.c"
#include <commons/log.h>

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int socket_cliente;
	int SUSCRITOS_GET[10];
	int cant_suscritos_get;
	t_log *logger;
}Colas;

void enviar_a_todos(int suscritos[], int cant, int msg,int size, int cod_op){

	for(int i = 0; i < cant; i++){
		devolver_mensaje(msg, size, suscritos[i], cod_op);
	}

}

void process_request(Colas *loquito) {
	int size;
	void* msg;
	int cod_op;

	if(recv(loquito->socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) == -1)
			cod_op = -1;


		switch (cod_op) {

		case SUS_GET:
			//Suscribir a la cola get
			loquito->SUSCRITOS_GET[loquito->cant_suscritos_get] = loquito->socket_cliente;
			log_info(loquito->logger,"Se Suscribio a la lista GET %d", loquito->SUSCRITOS_GET[loquito->cant_suscritos_get]);
			loquito->cant_suscritos_get++;

			break;

		case GET_POKEMON:
			//Agrega el mensaje a la cola get
			msg = recibir_mensaje(loquito->socket_cliente, &size);
			enviar_a_todos(loquito->SUSCRITOS_GET, loquito->cant_suscritos_get, msg, size, cod_op );
			//devolver_mensaje(msg,size,loquito.SUSCRITOS_GET[loquito.cant_suscritos_get],cod_op);
			log_info(loquito->logger,"Se envio mensaje a todos los suscriptos!");
			free(msg);
			break;

		case SUSCRIBIR:
			msg = recibir_mensaje(loquito->socket_cliente, &size);
			printf("El mensaje es %s", msg);
			break;
/*
		case SUS_LOC:
			break;
		case SUS_CAUGTH:
			break;

		case SUS_NEW:

			break;

		case SUS_CATCH:

			break;
		case SUS_APP:

			break;
*/


		case 0:
			pthread_exit(NULL);
		case -1:
			pthread_exit(NULL);
		}
}

int main(void) {

	char* ip;
	char* puerto;

	t_log* logger;
	t_config* config;

	fflush(stdout);

	char* conf = "/home/utnso/tp-2020-1c-NN/broker/src/broker.config";

	logger =log_create("broker.log", "Broker", 1, LOG_LEVEL_INFO);

	config=config_create(conf);

	ip= config_get_string_value(config,"IP_BROKER");
	puerto= config_get_string_value(config,"PUERTO_BROKER");

	log_info(logger,"Servidor con IP %s y puerto %s", ip, puerto);

	int i=0;
	int socketero[100];
	int socket_servidor = iniciar_servidor(ip,puerto);

	Colas *colas = malloc(sizeof(Colas));
	colas->logger = logger;
	colas->cant_suscritos_get = 0;


    while(1){
    	colas->socket_cliente = esperar_cliente(socket_servidor);

    	socketero[i]= colas->socket_cliente;
    	log_info(logger,"socketero: %d", socketero[i]);
    	i++;

    	process_request(colas);
    }

	//char *mensaje = recibir_mensaje(conexion);

	return EXIT_SUCCESS;
}
