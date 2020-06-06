#include "bibliotec.h"

//t_queue *GET_POKEMON = create_queue();
//t_queue *LOCALIZED = create_queue();

int iniciar_servidor(char* ip, char* puerto)
{
	int socket_servidor;

    struct addrinfo hints, *servinfo, *p;

    fflush(stdout);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(ip, puerto, &hints, &servinfo);

    for (p=servinfo; p != NULL; p = p->ai_next)
    {
        if ((socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            continue;

        if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1) {
            close(socket_servidor);
            continue;
        }
        break;
    }

	listen(socket_servidor, SOMAXCONN);
    freeaddrinfo(servinfo);

    return socket_servidor;
}

int esperar_cliente(int socket_servidor)
{
	struct sockaddr_in dir_cliente;

	socklen_t tam_direccion = sizeof(struct sockaddr_in);
	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

	//pthread_create(&thread,NULL,(void*)serve_client,&socket_cliente);
	//pthread_detach(thread);
    return socket_cliente;
}



void* recibir_mensaje(int socket_cliente, int* size)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void* serializar_paquete(t_paquete* paquete, int bytes)
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

void devolver_mensaje(void* payload, int size, int socket_cliente, op_code codigo)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = codigo;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = size;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, payload, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

//---------- Cliente

int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
		return -1;

	freeaddrinfo(server_info);

	return socket_cliente;
}

int reintentar_conexion(char* ip, char* puerto , int tiempo)
{
	clock_t start, diff;
	int elapsedsec;
	int sec = tiempo;
	int iterations = 0;
	int conexion = -1;

	while (iterations < 500 && (conexion== -1)) {
		start = clock();

		while (1) {
			diff = clock() - start;
	        elapsedsec = diff / CLOCKS_PER_SEC;

	        if (elapsedsec >= sec) {
	        	conexion =crear_conexion(ip,puerto);
	        	iterations++;
	            break;
	            }
	        }
	    }
	return conexion;

}

void* serializar_paquete_cliente(t_paquete* paquete, int *bytes)
{

	    int size_serializado= sizeof(op_code) + sizeof(int) + paquete->buffer->size;
		void *buffer= malloc(size_serializado);


		int bytes_escritos= 0;
		memcpy(buffer + bytes_escritos, &(paquete->codigo_operacion), sizeof(paquete->codigo_operacion));
		bytes_escritos += sizeof(paquete->codigo_operacion);

		memcpy(buffer + bytes_escritos, &(paquete->buffer->size),sizeof(paquete->buffer->size));
		bytes_escritos += sizeof(paquete->buffer->size);

		memcpy(buffer + bytes_escritos, paquete->buffer->stream,paquete->buffer->size);
		bytes_escritos += sizeof(paquete->buffer->size);


		(*bytes)= size_serializado;
		return buffer;

}

void enviar_mensaje(char* mensaje, int socket_cliente, op_code codigo)
{

	//----------------EMPAQUETAMIENTO----------------
			t_paquete *paquete = malloc(sizeof(t_paquete));
			paquete->codigo_operacion= codigo;
			paquete->buffer = malloc(sizeof(t_buffer));
			paquete->buffer->stream= mensaje;
			paquete->buffer->size= strlen(mensaje) + 1 ;

	//----------------ACA SE USA serializar_paquete----------------
			int size_serializado;
			void* serializado= serializar_paquete_cliente(paquete, &size_serializado);
			send(socket_cliente,serializado,size_serializado,0);
	//----------------libero la memoria del paquete mandado----------------
			free(serializado);

}


char* recibir_mensaje_cliente(int socket_cliente)
{
	op_code operacion;
		recv(socket_cliente,&operacion,sizeof(operacion),0);
		int buffer_size;
		recv(socket_cliente,&buffer_size,sizeof(buffer_size),0);
		char *buffer = malloc(buffer_size);
		recv(socket_cliente,buffer,buffer_size,0);
		if (buffer[buffer_size - 1] != '\0'){

			//ACA ES CON LOG
			printf("WARN: El buffer no es un string\n");
		}
		return buffer;
}
//------------------------------LOCURAS---------------------------------------------------------
void* recibir_mensaje_ACK(int socket_cliente)
{
	op_code operacion;
	pthread_t ack;

	recv(socket_cliente,&operacion,sizeof(operacion),0);
	int buffer_size;
	recv(socket_cliente,&buffer_size,sizeof(buffer_size),0);
	char *buffer = malloc(buffer_size);
	recv(socket_cliente,buffer,buffer_size,0);
	if (buffer[buffer_size - 1] != '\0'){
			//ACA ES CON LOG
		printf("WARN: El buffer no es un string\n");
	}

	printf("Recibi el mensaje: %s\n", buffer);

	printf("Codigo: %d\n", operacion);

	pthread_create(&ack,NULL,(void*)enviarACK,&operacion);
	pthread_detach(ack);

	//pthread_join(ack,NULL);
	//pthread_exit(&ack);
	return NULL;

}

void* enviarACK(op_code* operacion){

	int socket_ACK;

	printf("Codigo: %d\n", *operacion);

	socket_ACK = crear_conexion("127.0.0.1","5003");

	switch(*operacion){
	case GET_POKEMON:
		enviar_mensaje("Llego el Mensaje de suscripcion a la cola NEW.\n",socket_ACK,ACK);
		break;
	case SUS_NEW:
		printf("HOLA LO ENVIO.");
		enviar_mensaje("Llego el Mensaje de suscripcion a la cola NEW.\n",socket_ACK,ACK);
		break;
	case SUSCRIBIR:
		enviar_mensaje("Llego el Mensaje de suscripcion a la cola MASTER OF PUPPETS.\n",socket_ACK,ACK);
		break;
	default:
		printf("No anda como nada de lo que hacemos.\n");
		break;
	}
	liberar_conexion(socket_ACK);
	return NULL;
}


//-------------------------------------------------------------------------------------------------

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}


void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	if(logger != NULL){
	 log_destroy(logger);
	}

	if(config != NULL){
		config_destroy(config);
	}

	liberar_conexion(conexion);
}

char* concatena(int cant, char* argv[]){
	int i;
	char *mensajeFinal = string_new();
	for (i=3;i<cant;i++){
		string_append_with_format(&mensajeFinal," %s",argv[i]);
	}
	string_append_with_format(&mensajeFinal," %s\n",argv[i]);

	return mensajeFinal;

}
