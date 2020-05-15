#include "/home/utnso/tp-2020-1c-NN/biblioteca/biblioteca.c"
#include <commons/log.h>

#include <stdio.h>
#include <stdlib.h>


void process_request(int cliente_fd) {
	int size;
	void* msg;
	int cod_op;

	if(recv(*socket, &cod_op, sizeof(int), MSG_WAITALL) == -1)
			cod_op = -1;


		switch (cod_op) {
		case SUS_GET:
			//Agregar a la cola y enviar todo el tiempo los mensajes que le llegen
			break;
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
		/*case GET_POKEMON:
			//Agrega el mensaje a la cola get
			msg = recibir_mensaje(cliente_fd, &size);
			queue_push(GET_POKEMON*,(void*)msg);
			free(msg);
			break;

		case LOCALIZED_POKEMON:
			//Agrega el mensaje a la cola localized
			msg = recibir_mensaje(cliente_fd, &size);
			queue_push(LOCALIZED*,(void*)msg);
			free(msg);
			break;*/

		/*case MENSAJE:
			msg = recibir_mensaje(cliente_fd, &size);
			printf("%s\n",msg);
			devolver_mensaje(msg, size, cliente_fd);

			free(msg);
			break;*/

		//CASO SUSCRIPCION
		case SUSCRIBIR:
			printf("SE QUIEREN SUSCRIBIR BREO\n");
			msg = recibir_mensaje(cliente_fd, &size);
			printf("%s\n",msg);
			//devolver_mensaje(msg, size, cliente_fd);

			free(msg);
		break;
/*
		case SUS_NEW:
			while(1){
			//msg= queue_pop(NEW_POKEMON);
			if(msg != "Hola")
				//log
				printf("Esta vacio, ves? No hay mensajes aqui.\n");
				//msg = recibir_mensaje(cliente_fd, &size);
					//		printf("%s\n",msg);
			//}else{
			//	printf("%s\n",msg);
				//devolver_mensaje(msg, size, cliente_fd, cod_op);

				//free(msg);


		}
		//}
		break;
*/
/*Si hay un solo suscribir
			 if(msg="new")
				susNew(socket)

				susnew(int socket)
				colaNew
				esperar
				enviarmensaje(socket,colanew)
*/
/* Si hay 6 suscribir
		case SUS_NEW:
			while(1)
			enviar o sea send

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

    while(1){
    	int socket_cliente = esperar_cliente(socket_servidor);

    	socketero[i]= socket_cliente;
    	log_info(logger,"socketero: %d\n", socketero[i]);
    	i++;

    	pthread_create(&thread,NULL,(void*) process_request,&socket_cliente);
        pthread_detach(thread); // SIN LOS HILOS CORTA EL WHILE
    	//process_request(esperar_cliente(socket_servidor));
    }

	//char *mensaje = recibir_mensaje(conexion);

	return EXIT_SUCCESS;
}
