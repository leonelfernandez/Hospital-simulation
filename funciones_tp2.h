#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "lista.h"
#include "hash.h"
#include "heap.h"
#include "csv.h"
#include <ctype.h>
#include "string.h"
#include "estructuras_aux.h"

void pedir_turno(hospital_t* hospital, char** turno);

void atender_sgte_paciente(hospital_t* hospital, char** linea);

void informe_doctores(hospital_t* hospital, char* inicio, char* fin);