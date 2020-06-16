#ifndef TEAM_H_
#define TEAM_H_

#include <bibliotec.h>

typedef struct{
	char *ip_broker;
	char *puerto_broker;
	char *log;
	int retardo_cpu;
	int reconexion;

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
	int puede_capturar;
	int idCatch;
	sem_t mx_entrenador;
	pthread_t hilo;
}Entrenador;

char* ip;
char* puerto;
t_log* logger;
Config_Team *config;
t_list *entrenadores;
t_list *objetivoGlobal;
t_list *new;
t_list *ready;
t_queue *exce;
t_list *block;
t_list *blockCaugth;
t_list *blockDeadlock;
pthread_mutex_t mxExce;
sem_t semaforoExce;

#endif /* TEAM_H_ */
