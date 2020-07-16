#ifndef FUNCIONES_AUXILIARES_H_
#define FUNCIONES_AUXILIARES_H_

#include "team.h"

Config_Team* construirConfigTeam(t_config* config);
void setearVariablesGlobales();
t_list *obtenerObjetivoGlobal();
void solicitar_pokemones(t_list *);
void menorDistancia (Poketeam *pokemon);
int distancia(Posicion *posicion1, Posicion posicion2);
void moverse(Entrenador *entrenador);
bool puedeFinalizar(Entrenador *entrenador);
void remover_entrenador(int entrenadorNumero, t_list *lista);
Entrenador* id_coincidente(int id, t_list *lista);

void agregar_segun_objetivo(t_list *capturados, Entrenador *entrenador);
void agregar_segun_faltantes(char* pokemon, Entrenador *entrenador);
bool pokemon_en_lista(char *pokemon, t_list *lista);
void eliminar_pokemon(char *pokemon, t_list *lista);
bool necesitaPokemon(char *pokemon, t_list *objetivoGlobal);
t_list* filtrado(t_list* capturados, t_list* objetivos);
t_list* listaUnica(t_list *listaDeListas);
t_list* listaDeListas(t_config* config, char* cadena);
t_list* listaPosiciones(t_config* config, char* cadena);
void recorrerLista(t_list *lista, t_log *logger);
void enviar_catch(Entrenador *entrenador, int conexion_catch);
void eliminar_pendientes(char *pokemon);
void ponerEnReady(Entrenador *entrenador, Poketeam *pokemon);

bool bloqueado_por_agarrar(Entrenador *entrenador);
bool bloqueado_por_capturar(Entrenador *entrenador);
bool bloqueado_por_deadlock(Entrenador *entrenador);
int indiceMasCercano(Posicion posicion, t_list *lista);
bool id_en_lista(int id_mensaje);

t_list* recibirLocalized(t_mensaje_get* mensaje);

void free_entrenador(Entrenador *entrenador);

#endif
