#ifndef TEAM_H_
#define TEAM_H_

#include <bibliotec.h>

char* ID_PROCESO;

typedef struct{
	char *ip_broker;
	char *puerto_broker;
	char *log;
	int retardo_cpu;
	int reconexion;
	int quantum;

	t_list *pokemon_entrenadores;
	t_list *objetivos_entrenadores;
	t_list *posiciones_entrenadores;

}Config_Team;

typedef struct{
	uint8_t x;
	uint8_t y;
}Posicion;

typedef struct{
	Posicion pos;
	char *pokemon;
}Poketeam;

typedef struct{
	int entrenadorNumero;
	Posicion *posicion;
	Posicion *posicion_a_capturar;
	char *pokemon_a_caputar;
	t_list *pokemones_capturados;
	t_list *pokemones_objetivos;
	t_list *pokemones_faltantes;
	t_list *pokemones_a_intercambiar;
	bool puede_agarrar;
	bool block_capturar;
	bool block_agarrar;
	bool block_deadlock;
	int puede_capturar;
	int idCatch;
	sem_t sem_entrenador;
}Entrenador;

char* ip;
char* puerto;
int cantEntrenadores;
t_log* logger;
Config_Team *config;

t_list *objetivoGlobal;

t_list *new;
t_list *ready;
t_list *block;

t_list *id_localized;
t_list *pokemones_pendientes;
t_list *pokemones_en_busqueda;

pthread_mutex_t mxExce;
pthread_mutex_t mx_llegada_pokemon;
sem_t semaforoExce;
sem_t semaforoIntercambio;
sem_t semaforoDeadlock;


void liberar_lista_de_listas(t_list* listaDeListas);
void liberar_listas_config();

#endif /* TEAM_H_ */
