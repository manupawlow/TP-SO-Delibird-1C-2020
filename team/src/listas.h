#ifndef LISTAS_H_
#define LISTAS_H_

#include "team.h"

Config_Team* construirConfigTeam(t_config* config);
t_list* filtrado(t_list* capturados, t_list* objetivos);
t_list* listaUnica(t_list *listaDeListas);
t_list* listaDeListas(t_config* config, char* cadena);
t_list* listaPosiciones(t_config* config, char* cadena);
void recorrerLista(t_list *lista, t_log *logger);
void menorDistancia (Poketeam pokemon);
float distancia(Entrenador *entrenador, Posicion posicion);
t_list *obtenerObjetivoGlobal();


#endif
