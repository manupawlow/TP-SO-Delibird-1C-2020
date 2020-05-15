#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<commons/config.h>
#include<string.h>
#include<pthread.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/collections/queue.h>
#include<readline/readline.h>


//CODIGO DE TIPOS DE MENSAJE
typedef enum
{
	//hacer 6 tipos o uno solo?
	SUSCRIBIR=1,
	SUS_NEW,
	SUS_GET,
	SUS_LOC,
	SUS_APP,
	SUS_CAUGTH,
	SUS_CATCH,
	//acknowledgment es para avisar que le llego un mensaje al broker + boludeces
	ACK,
	NEW_POKEMON,
	GET_POKEMON,
	CAUGHT_POKEMON,
	CATCH_POKEMON,
	LOCALIZED_POKEMON,
	APPEARED_POKEMON
}op_code;
//----------------------------

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

pthread_t thread;

//t_queue *GET_POKEMON,*LOCALIZED;

void* recibir_buffer(int*, int);
int iniciar_servidor(char*, char*);
int esperar_cliente(int);
void* recibir_mensaje(int socket_cliente, int* size);
int recibir_operacion(int);
//void process_request(int cod_op, int cliente_fd);
//void serve_client(int *socket);
void* serializar_paquete(t_paquete* paquete, int bytes);
void devolver_mensaje(void* payload, int size, int socket_cliente, op_code codigo);

int crear_conexion(char*, char*);
void enviar_mensaje(char* mensaje, int socket_cliente, op_code codigo);
char* recibir_mensaje_cliente(int socket_cliente);
void eliminar_paquete(t_paquete* paquete);
void liberar_conexion(int socket_cliente);
void* serializar_paquete_cliente(t_paquete* paquete, int*);

void terminar_programa(int conexion, t_log* logger, t_config* config);
