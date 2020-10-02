#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "lista.h"
#include "hash.h"
#include "heap.h"
#include "csv.h"
#include "string.h"
#include "mensajes.h"
#include "cola.h"
#include "abb.h"

typedef struct hospital hospital_t;
typedef struct doctores doctor_t;
typedef struct pacientes paciente_t;
typedef struct paciente_a_atender paciente_a_atender_t;
typedef struct prioridades prioridades_t;


doctor_t* crear_doctor(char** datos);

paciente_t* crear_paciente(char** datos);

hospital_t* crear_hospital(hash_t* pacientes, hash_t* doctores, hash_t* cant_pacientes_esp, hash_t* turnos_pedidos, abb_t*);

hash_t* crear_hash_pacientes(lista_t* pacientes);

hash_t* crear_hash_doctores(lista_t* doctores);

hash_t* crear_hash_cant_x_especialidad(lista_t* doctores);

abb_t* hospital_abb_doctore(hospital_t* hospital);

hash_t* hospital_cantidad(hospital_t* hospital);

hash_t* hospital_turnos(hospital_t* hospital);

hash_t* hospital_doctores(hospital_t* hospital);

hash_t* hospital_pacientes(hospital_t* hospital);

hash_t* crear_hash_a_atender(lista_t* doctores);

abb_t* crear_abb_doctores(lista_t* doctores);

char* obtener_inscripcion_paciente(paciente_t* paciente);

cola_t* obtener_cola_prioridad(prioridades_t* prioridad);

abb_t* obtener_abb_prioridad(prioridades_t* prioridad);

char* obtener_especialidad_doctor(doctor_t* doctor);

char* obtener_nombre_doctor(doctor_t* doctor);

size_t obtener_cant_atendidos_doctor(doctor_t* doctor);

char* obtener_nombre_paciente(paciente_t* paciente);

char* desencolar_nombre(cola_t* cola);

void incrementar_atendidos(hospital_t* hospital, doctor_t* doctor);

void incrementar_atendidos_abb(hospital_t* hospital, doctor_t* doctor);

void hospital_destruir(hospital_t* hospital);

void hospital_destruir_listas(lista_t* lista_doctores, lista_t* lista_pacientes);