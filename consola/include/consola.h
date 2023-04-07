#ifndef CLIENT_H_
#define CLIENT_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>

#include "utils.h"


t_log* iniciar_logger(char*);
t_config* iniciar_config(char*);
void leer_consola(t_log*);
void paquete(int);
void terminar_programa(int, t_log*, t_config*);
int armar_conexion(t_config*, t_log*);
#endif /* CLIENT_H_ */
