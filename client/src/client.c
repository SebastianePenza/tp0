#include "client.h"
#include <stdlib.h>
#include <stdio.h>

char* log_path = "../logs/tp0.log";
char* log_process_name = "Client-Logger";
char* log_msg = "Soy un Log";
char* config_path = "../cliente.config";

int main(void)
{
	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */
	logger = iniciar_logger();

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */
	config = iniciar_config();
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");
	log_info(logger, valor);

	/* ---------------- LEER DE CONSOLA ---------------- */
	//leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/
	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	nuevo_logger = log_create(log_path, log_process_name, true, 2);
	log_info(nuevo_logger, log_msg);

	if(nuevo_logger == NULL) {
		printf("No se pudo crear el log");
		exit(EXIT_FAILURE);
	}

return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	nuevo_config = config_create(config_path);
	
	if(nuevo_config == NULL) {
		printf("No se pudo crear el archivo config.");
		exit(EXIT_FAILURE);
	}

return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;		// Puntero de tipo char para recibir el malloc de readline()

	leido = readline("Escribir la primera linea: \n> ");

	while(*leido != NULL){
		log_info(logger, leido);
		printf("\nLinea recibida, ");
		leido = readline("escribir la siguiente linea: \n> ");
	} 
	if (*leido == NULL)
	{
		printf("Cerrando Programa...");
		free(leido);
		exit(EXIT_SUCCESS);
	}
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;
	paquete = crear_paquete();
	leido = readline("> ");
	
	while((*leido) != NULL) {
		agregar_a_paquete(paquete, leido, strlen(leido) + 1);
		leido = readline("> ");
	}
	free(leido);
	enviar_paquete(paquete, conexion);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
}
