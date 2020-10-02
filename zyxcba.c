#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include "funciones_tp2.h"
#include "strutil.h"
#include "mensajes.h"
#include "cola.h"

#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"

void procesar_comando(hospital_t* hospital , char* comando, char** parametros) {
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {
		pedir_turno(hospital, parametros);
	} else if (strcmp(comando, COMANDO_ATENDER) == 0) {
		atender_sgte_paciente(hospital, parametros);
	} else if (strcmp(comando, COMANDO_INFORME) == 0) {
		informe_doctores(hospital, parametros[0], parametros[1]);
	} else {
		printf(ENOENT_FORMATO, comando);
		printf(ENOENT_CMD, comando);
		printf(ENOENT_PARAMS, COMANDO_INFORME);
	}
}

void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

void procesar_entrada(hospital_t* hospital) {
	char* linea = NULL;
	size_t c = 0;
	while (getline(&linea, &c, stdin) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, ':');
		if (campos[1] == NULL) {
			printf(ENOENT_FORMATO, linea);
			free_strv(campos);
			continue;	
		}
		char** parametros = split(campos[1], ',');
		procesar_comando(hospital ,campos[0], parametros);
		free_strv(parametros);
		free_strv(campos);
	}
	free(linea);
}

void* wraper_doctor(char** datos, void* extra){
	if(!datos) return extra;
    void* doc = crear_doctor(datos);
    return doc;
}

void* wraper_pacientes(char** datos, void* extra){
	if(!datos) return extra;
    void* paci = crear_paciente(datos);
    return paci;
}

int main(int argc, char** argv) {
	if(!argv[0] && !argv[1]){
		printf(ENOENT_CANT_PARAMS);
		return 0;
	}
	printf("Entro aca\n");


	lista_t* lista_doctores = csv_crear_estructura(argv[0], wraper_doctor, NULL);
	if(!lista_doctores) return 0;
	lista_t* lista_pacientes = csv_crear_estructura(argv[1], wraper_pacientes, NULL);
	if(!lista_pacientes){
		lista_destruir(lista_doctores, NULL);
		return 0;
	}
	hash_t* hash_pacientes = crear_hash_pacientes(lista_pacientes);
	if(!hash_pacientes){
		hospital_destruir_listas(lista_doctores, lista_pacientes);
		return 0;
	}
	hash_t* hash_doctores = crear_hash_doctores(lista_doctores);
	hash_t* pacientes_a_atender = crear_hash_cant_x_especialidad(lista_doctores);
	hash_t* pacientes_por_prioridad = crear_hash_a_atender(lista_doctores);
	abb_t* abb_doctores = crear_abb_doctores(lista_doctores);
	hospital_destruir_listas(lista_doctores, lista_pacientes);
	hospital_t* hospital = crear_hospital(hash_pacientes, hash_doctores, pacientes_a_atender, pacientes_por_prioridad, abb_doctores);
	procesar_entrada(hospital);
	hospital_destruir(hospital);
	return 0;
}