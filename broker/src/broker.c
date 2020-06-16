#include "broker.h"
#include "suscripciones.h"

int main(void) {

	char* ip;
	char* puerto;
	int i=0;

	t_log* logger;
	t_config* config;

	fflush(stdout);

	char* conf = "/home/utnso/tp-2020-1c-NN/broker/src/broker.config";

	logger = log_create("/home/utnso/broker.txt", "Broker", 1, LOG_LEVEL_INFO);

	config=config_create(conf);

	ip= config_get_string_value(config,"IP_BROKER");
	puerto= config_get_string_value(config,"PUERTO_BROKER");

	log_info(logger,"Servidor con IP %s y puerto %s", ip, puerto);

	int socketero[100];
	int socket_servidor = iniciar_servidor(ip,puerto);

	Colas *colas = malloc(sizeof(Colas));
	colas->logger = logger;
	colas->cant_suscritos_get = 0;
	colas->cant_suscritos_appeared = 0;
	colas->cant_suscritos_localized = 0;
	colas->cant_suscritos_caught =0;
	colas->cant_suscritos_catch =0;
	colas->cant_suscritos_new =0;

    while(1){
    	colas->socket_cliente = esperar_cliente(socket_servidor);

    	socketero[i]= colas->socket_cliente;
    	log_info(logger,"socketero: %d", socketero[i]);
    	i++;

    	process_request(colas);
    }

	//char *mensaje = recibir_mensaje(conexion);

	return EXIT_SUCCESS;
}
