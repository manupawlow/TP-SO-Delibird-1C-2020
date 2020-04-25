#ifndef gameBoy
#define gameBOy

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>

#include "comunic.h"


t_log* iniciar_logger(void);
t_config* leer_config(void);
void terminar_programa(int, t_log*, t_config*);

#endif
