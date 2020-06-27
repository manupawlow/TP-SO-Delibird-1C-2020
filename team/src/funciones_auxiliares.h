#ifndef FUNCIONES_AUXILIARES_H_
#define FUNCIONES_AUXILIARES_H_

#include "team.h"

Config_Team* construirConfigTeam(t_config* config);
void setearVariablesGlobales();
t_list *obtenerObjetivoGlobal();
void solicitar_pokemones(t_list *);
void menorDistancia (Poketeam pokemon);
int distancia(Posicion *posicion1, Posicion posicion2);
void moverse(Entrenador *entrenador);

void remover_entrenador(int entrenadorNumero, t_list *lista);
t_list* filtrado(t_list* capturados, t_list* objetivos);
t_list* listaUnica(t_list *listaDeListas);
t_list* listaDeListas(t_config* config, char* cadena);
t_list* listaPosiciones(t_config* config, char* cadena);
void recorrerLista(t_list *lista, t_log *logger);


bool bloqueado_por_agarrar(Entrenador *entrenador);

//bool bloqueado_por_capturar(Entrenador *entrenador);

//bool bloqueado_por_deadlock(Entrenador *entrenador);

#endif
