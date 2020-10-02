#include "pila.h"
#include <stdlib.h>
#include <stdio.h>
#define TAMANIO 20
/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

//
pila_t* pila_crear(void){
    pila_t* pila = malloc(sizeof(pila_t));

    if( pila == NULL){
        return NULL;
    }
    pila->capacidad = TAMANIO;

    pila->datos = malloc(sizeof(void*)*pila->capacidad);

    if(pila->datos == NULL){
        free(pila);
        return NULL;
    }
    pila->cantidad = 0;

    return pila;
}
void pila_destruir(pila_t *pila){
    free(pila->datos);
    free(pila);
}

bool pila_esta_vacia(const pila_t *pila){
    return (pila->cantidad == 0);
}

void* pila_ver_tope(const pila_t *pila){
    if(pila_esta_vacia(pila)){
        return NULL;
    }
    return pila->datos[pila->cantidad - 1];
}
bool pila_redimensionar(pila_t* pila, size_t nueva_capacidad){
    void** datos_nuevos = realloc(pila->datos, nueva_capacidad * sizeof(void*));

    if(datos_nuevos == NULL){
        return false;
    }

    pila->datos = datos_nuevos;
    pila->capacidad = nueva_capacidad;
    return true;
}

bool pila_apilar(pila_t *pila, void* valor){
    if( pila->cantidad == pila->capacidad){
        if(!pila_redimensionar(pila, pila->capacidad*2)){
            return false;
        }
    }
    pila->datos[pila->cantidad] = valor;
    pila->cantidad ++;
    return true;
}

void* pila_desapilar(pila_t *pila){
    if(pila_esta_vacia(pila)){
        return NULL;
    }
    if(pila->cantidad <= pila->capacidad/4 && pila->capacidad/2 >= TAMANIO){
        if(!pila_redimensionar(pila, pila->capacidad/2)){
            return NULL;
        }
    }
    pila->cantidad --;
    return pila->datos[pila->cantidad];
}
