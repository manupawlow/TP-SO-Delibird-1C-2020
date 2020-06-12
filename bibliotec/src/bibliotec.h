#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<string.h>
#include<pthread.h>
#include<commons/log.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include<commons/collections/queue.h>
#include<commons/string.h>
#include <commons/temporal.h>
#include<readline/readline.h>
#include<math.h>
#include<time.h>
#include<semaphore.h>

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

typedef struct
{
	int posx;
	int posy;
	//char *nombre;
} t_pokemon;

pthread_t thread;

//t_queue *GET_POKEMON,*LOCALIZED;

void* recibir_buffer(int*, int);
int iniciar_servidor(char*, char*);
int esperar_cliente(int);
void* recibir_mensaje(int , int* );
int recibir_operacion(int);
//void process_request(int cod_op, int cliente_fd);
//void serve_client(int *socket);
void* serializar_paquete(t_paquete* , int );
void devolver_mensaje(void* , int , int , op_code );

int crear_conexion(char*, char*);
int reintentar_conexion(char* ip, char* puerto , int tiempo);
void enviar_mensaje(char* , int , op_code );
char* recibir_mensaje_cliente(int );
void eliminar_paquete(t_paquete* );
void liberar_conexion(int );
void* serializar_paquete_cliente(t_paquete* , int*);

char* concatena(int , char** );


void* enviarACK(op_code* );
void* recibir_mensaje_ACK(int);

void enviar_pokemon(t_pokemon pokemon, int socket);
t_pokemon* deserializar_pokemon(t_buffer* buffer);

void terminar_programa(int , t_log* , t_config* );
