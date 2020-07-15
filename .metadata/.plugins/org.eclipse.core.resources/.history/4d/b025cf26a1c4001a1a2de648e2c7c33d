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
#include<sys/time.h>
#include<semaphore.h>

//CODIGO DE TIPOS DE MENSAJE
typedef enum
{
	SUSCRIBIR=1,

	SUS_NEW,
	SUS_GET,
	SUS_LOC,
	SUS_APP,
	SUS_CAUGHT,
	SUS_CATCH,

	NEW_POKEMON,
	GET_POKEMON,
	CAUGHT_POKEMON,
	CATCH_POKEMON,
	LOCALIZED_POKEMON,
	APPEARED_POKEMON,

	ACK
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

typedef struct
{
    uint32_t pokemon_length;
    char* pokemon;
    uint8_t posx;
    uint8_t posy;
	uint32_t cantidad;
	uint32_t id_mensaje;
	uint32_t id_mensaje_correlativo;
	uint32_t resultado;
} t_mensaje;

typedef struct
{
	uint32_t pokemon_length;
	char* pokemon;
	uint32_t cantidad;
	uint32_t id_mensaje;
	uint32_t id_mensaje_correlativo;
	uint32_t posiciones_length;
	char* posiciones;
}t_mensaje_get;


void* recibir_buffer(int*, int);
int iniciar_servidor(char*, char*);
int esperar_cliente(int);
void* recibir_mensaje(int );
int recibir_operacion(int);
t_buffer* serializar_mensaje_struct(t_mensaje* );
t_mensaje* deserializar_mensaje_struct(t_buffer* );
void enviar_mensaje_struct(t_buffer* , int , op_code );
t_mensaje* recibir_mensaje_struct(int );
void* serializar_paquete(t_paquete* , int );
void devolver_mensaje(void* , int , int , op_code );

int crear_conexion(char*, char*);
int crear_conexion_broker(char*, char*, t_log*, int, op_code);
int reintentar_conexion(char* , char*  , int );
void enviar_mensaje(char* , int , op_code );
char* recibir_mensaje_cliente(int );
void eliminar_paquete(t_paquete* );
void liberar_conexion(int );
void* serializar_paquete_cliente(t_paquete* , int*);

char* concatena(int , char** );

void* enviarACK(op_code* );
void* recibir_mensaje_ACK(int);

void freeDoblePuntero(char** doblePuntero);

t_buffer* serializar_mensaje_struct_get(t_mensaje_get* mensaje);
t_mensaje_get* deserializar_mensaje_struct_get(t_buffer* buffer);
t_mensaje_get* recibir_mensaje_struct_get(int );

void terminar_programa(int , t_log* , t_config* );
