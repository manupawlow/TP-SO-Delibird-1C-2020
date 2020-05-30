#ifndef TEAM_H_
#define TEAM_H_

typedef struct{
	char *ip_broker;
	char *puerto_broker;

	t_list *pokemon_entrenadores;
	t_list *objetivos_entrenadores;
	t_list *posiciones_entrenadores;

}Config_Team;

typedef struct{
	int x;
	int y;
}posicion;

Config_Team* construirConfigTeam(t_config* config);
t_list* filtrado(t_list* capturados, t_list* objetivos);
t_list* listaUnica(t_list *listaDeListas);
t_list* listaDeListas(t_config* config, char* cadena);
t_list* listaDeListasPosiciones(t_config* config, char* cadena);
void recorrerLista(t_list *lista, t_log *logger);

#endif /* TEAM_H_ */
