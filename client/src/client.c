#include "client.h"
#define ENTER "\n"
#define SIGN_CONSOLA "> "

int main(int argc, char** argv)
{
	// Comentar este if si se quiere tomar los valores por defecto
	if (argc<2) {
		printf("BAD REQUEST: Enviar dos parametros\n");
		printf("Parametros enviados: %d\n", argc);
		for(int i=0; i<argc; i++) {
			printf("%s\n", argv[i]);
		}
		return EXIT_FAILURE;
	}

	// Se setean los parametros que se pasan, con poner valores por defecto
	char* rutaConfig = argv[1] ? argv[1] : "./cliente.config";
	char* rutaInstrucciones = argv[2] ? argv[2] : "";
	char* rutaLog = argv[3] ?  argv[3] : "./client.log";

	int conexion;
	char* clave;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger(rutaLog);
	config = iniciar_config(rutaConfig);

	leer_consola(logger);

	// Creamos una conexión hacia el servidor
	conexion = armar_conexion(config, logger);

	// Enviamos al servidor el valor de CLAVE como mensaje
	clave = config_get_string_value(config, "CLAVE");
	enviar_mensaje(clave, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);
}

t_log* iniciar_logger(char* rutaLog)
{
	t_log *logger;
	if (( logger = log_create(rutaLog, "logs", true, LOG_LEVEL_INFO)) == NULL ) {
		printf("No se pudo crear logger", ENTER);
		exit(1);
	}
	return logger;
}

t_config* iniciar_config(char* rutaConfig)
{
	t_config* nuevo_config;
	if ((nuevo_config = config_create(rutaConfig)) == NULL) {
		printf("No se pudo crear logger", ENTER);
		exit(1);
	}

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	printf("Los siguientes valores que ingresen se guardaran en el log, ingrese enter para terminar de ingresar valores\n");
	char* linea;

	while(1) {
		linea = readline(SIGN_CONSOLA);
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
		lineaPaquete = readline(SIGN_CONSOLA);
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

int armar_conexion(t_config* config, t_log* logger)
{
	char* ip = config_get_string_value(config, "IP");
	char* puerto = config_get_string_value(config, "PUERTO");

	log_info(logger, "Se va a hacer la conexion con los siguientes valores:", ENTER);
	log_info(logger, "ip %s, puerto %s\n", ip, puerto);

	return crear_conexion(ip, puerto);
}
