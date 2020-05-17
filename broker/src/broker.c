#include "/home/utnso/tp-2020-1c-NN/biblioteca/biblioteca.c"
#include <commons/log.h>

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int socket_cliente;
	int SUSCRITOS_GET[10];
	int cant_suscritos_get;
	t_log *logger;
}pp;

/*
	int *SUSCRITOS_NEW[];
	int cant_suscritos_new = 0;

	int *SUSCRITOS_LOC[];
	int cant_suscritos_loc = 0;

	int *SUSCRITOS_APP[];
	int cant_suscritos_app = 0;

	int *SUSCRITOS_CAUGTH[];
	int cant_suscritos_caugth = 0;

	int *SUSCRITOS_CATCH[];
	int cant_suscritos_catch = 0;
*/

	//t_list *SUSCRITOS_GET = create_list();
	//t_list *COLA_GET = create_list();
	/*
	t_list *SUSCRITOS_GET = create_list();
	t_list *SUSCRITOS_GET = create_list();
	t_list *SUSCRITOS_GET = create_list();
	t_list *SUSCRITOS_GET = create_list();
	t_list SUSCRITOS_GET = create_list();
    */

void enviar_a_todos(int suscritos[], int cant, int msg,int size, int cod_op){

	for(int i = 0; i < cant; i++){
		devolver_mensaje(msg, size, suscritos[i], cod_op);
	}

}

void process_request(pp loquito) {
	int size;
	void* msg;
	int cod_op;

	printf("%s",loquito.socket_cliente);

	if(recv(*socket, &cod_op, sizeof(int), MSG_WAITALL) == -1)
			cod_op = -1;


		switch (cod_op) {

		case SUS_GET:
			//Suscribir a la cola get
			loquito.SUSCRITOS_GET[loquito.cant_suscritos_get] = loquito.socket_cliente;
			loquito.cant_suscritos_get++;
			fflush(stdout);
			printf("Se Suscribio a la lista GET");
			log_info(loquito.logger,"Se Suscribio a la lista GET");
			break;

		case GET_POKEMON:
			//Agrega el mensaje a la cola get
			msg = recibir_mensaje(loquito.socket_cliente, &size);
			enviar_a_todos(loquito.SUSCRITOS_GET, loquito.cant_suscritos_get, msg, size, cod_op );
			fflush(stdout);
			//printf("Se envio mensaje a todos los suscriptos!");
			log_info(loquito.logger,"Se envio mensaje a todos los suscriptos!");
			//list_add(COLA_GET, msg);
			free(msg);
			break;

		case SUSCRIBIR:
			msg = recibir_mensaje(loquito.socket_cliente, &size);
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

	pp ppp;
	ppp.logger = logger;
	ppp.cant_suscritos_get = 0;


    while(1){
    	ppp.socket_cliente = esperar_cliente(socket_servidor);

    	socketero[i]= ppp.socket_cliente;
    	log_info(logger,"socketero: %d\n", socketero[i]);
    	i++;

    	pthread_create(&thread,NULL,(void*) process_request,&ppp);
        pthread_detach(thread); // SIN LOS HILOS CORTA EL WHILE
    	//process_request(esperar_cliente(socket_servidor));
    }

	//char *mensaje = recibir_mensaje(conexion);

	return EXIT_SUCCESS;
}
