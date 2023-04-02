#include "client.h"
#define ENTER "\n"

int main(void)
{
	int conexion;
	char* ip;
	char* puerto;
	char* clave;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger();

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	log_info(logger, getcwd(NULL, 0));
	config = iniciar_config();

	clave = config_get_string_value(config, "CLAVE");
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");

	log_info(logger, "Se han obtenido del archivo de configuracion los siguientes valores:", ENTER);
	log_info(logger, "ip %s, puerto %s y clave \%s\n", ip, puerto, clave);

	/* ---------------- LEER DE CONSOLA ---------------- */
	leer_consola(logger);

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(clave, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);
}

t_log* iniciar_logger(void)
{
	t_log *logger;
	if (( logger = log_create("client.log", "logs", true, LOG_LEVEL_INFO)) == NULL ) {
		printf("No se pudo crear logger", ENTER);
		exit(1);
	}
	return logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	if ((nuevo_config = config_create("./cliente.config")) == NULL) {
		printf("No se pudo crear logger", ENTER);
		exit(1);
	}

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	printf("Los siguientes valores que ingresen se guardaran en el log client.log, ingrese enter para terminar de ingresar valores\n");
	char* linea;

	while(1) {
		linea = readline("> ");
		if (strcmp(linea, "") == 0) {
			break;
		}
		log_info(logger, linea, ENTER);
		free(linea);
	}
}

void paquete(int conexion)
{
	t_paquete* paquete;
	char* lineaPaquete;

	if(!(paquete = crear_paquete())) {
		printf("Error al crear paquete");
	}

	// Leemos y esta vez agregamos las lineas al paquete
	printf("Los siguientes valores que ingreses se enviaran al servidor, ingrese enter para terminar de ingresar valores\n");

	while(1) {
		lineaPaquete = readline("> ");
		if (strcmp(lineaPaquete, "") == 0) {
			break;
		}
		agregar_a_paquete(paquete, lineaPaquete, strlen(lineaPaquete)+1);
		free(lineaPaquete);
	}

	enviar_paquete(paquete, conexion);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	if (logger != NULL) {
		log_destroy(logger);
	}

	if (config != NULL) {
		config_destroy(config);
	}

	liberar_conexion(conexion);
}
