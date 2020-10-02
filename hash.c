#define _POSIX_C_SOURCE 200809L
#include "hash.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define TAMANIO_INICIAL 101
#define FACTOR_REDIMENSION 2/3
#define FACTOR_MULTIPLICACION 100

typedef enum estados {
    VACIO,
    BORRADO,
    OCUPADO
}estado_t;

typedef struct nodo{
    char* clave;
    void* dato;
    estado_t estado;
} nodo_t;

struct hash {
    nodo_t* tabla;
    size_t tam;
    size_t cant;
    hash_destruir_dato_t destruir_dato;
};

/* Crea una tabla nueva pasandole el tamaño y inicializa los datos */
nodo_t* tabla_crear(size_t len){
    nodo_t* hash_tabla = malloc(sizeof(nodo_t) * len);
    if(!hash_tabla) return NULL;
    for (size_t i = 0; i < len; i++){
        hash_tabla[i].clave = '\0';
        hash_tabla[i].dato = NULL;
        hash_tabla[i].estado = 0;
    }
    return hash_tabla;
}

hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {
    hash_t* hash = malloc(sizeof(hash_t));
    if (hash == NULL) return NULL;
    hash->tam = TAMANIO_INICIAL;
    hash->tabla = tabla_crear(hash->tam);
    if(!hash->tabla){
        free(hash);
        return NULL;
    }
    hash->cant = 0;
    hash->destruir_dato = destruir_dato;
    
    return hash;
}

/* Funcion de hash */
// https://codereview.stackexchange.com/questions/85556/simple-string-hashing-algorithm-implementation
size_t codigo_hash(const char* source, size_t len){    
    if (source == NULL) {
        return 0;
    }

    size_t hash = 0;
    while (*source != '\0') {
        char c = *source++;
        int a = c - '0';
        hash = (hash * 10) + (size_t)a;     
    } 
    return (hash % len);
}

size_t hash_cantidad(const hash_t *hash) {
    return hash->cant;
}

/* Destruye la tabla */
void tabla_destruir(nodo_t* tabla, hash_destruir_dato_t destruir_dato, size_t len){
    for (size_t i = 0; i < len; i ++) {
        if(tabla[i].estado == OCUPADO){
            if(destruir_dato){
                destruir_dato(tabla[i].dato);
            }
            free(tabla[i].clave);
        }
    }
    free(tabla);
}

void hash_destruir(hash_t *hash) {
    tabla_destruir(hash->tabla, hash->destruir_dato, hash->tam);
    free(hash);
}

void *hash_obtener(const hash_t *hash, const char *clave){
    size_t i = codigo_hash(clave, hash->tam);
    while(hash->tabla[i].estado != VACIO){
        if ((hash->tabla[i].estado == OCUPADO) && (strcmp(hash->tabla[i].clave, clave) == 0)) {
            return hash->tabla[i].dato;
        }
        if(i == hash->tam){
            i = 0;
        }
        i++;
    }
    return NULL;
}

bool hash_pertenece(const hash_t *hash, const char *clave) {
    size_t i = codigo_hash(clave, hash->tam);
    if((hash->tabla[i].estado == OCUPADO) && (strcmp(hash->tabla[i].clave, clave) == 0)) return true;
    return hash_obtener(hash, clave);
}

/* Decide si hay que redimensionar la tabla */
bool hay_redimension(hash_t* hash) {
    return hash->tam * FACTOR_REDIMENSION <= hash->cant ? true : false;
}

/* Guarda en la tabla la clave y el dato */
void tabla_guardar(nodo_t* tabla, size_t pos, size_t nuevo_tam, nodo_t tabla_){
    if(tabla[pos].estado == OCUPADO){
        while(tabla[pos].estado == OCUPADO){
            pos ++;
            if(pos == nuevo_tam) pos = 0;
        }
    }
    tabla[pos].clave = strdup(tabla_.clave);
    free(tabla_.clave);
    tabla[pos].dato = tabla_.dato;
    tabla[pos].estado = OCUPADO;    
}

/* Redimensiona la tabla (Copia todos los elementos de la tabla vieja y cambia el tamaño)*/
bool hash_redimensionar(hash_t* hash, size_t nuevo_tamanio){
    if(nuevo_tamanio < TAMANIO_INICIAL) nuevo_tamanio = TAMANIO_INICIAL;
    nodo_t* nueva_tabla = tabla_crear(nuevo_tamanio);
    if (!nueva_tabla) return false;
    hash->cant = 0;
    for (size_t i = 0; i < hash->tam; i++){
        if(hash->tabla[i].estado == OCUPADO){
            size_t posicion = codigo_hash(hash->tabla[i].clave, nuevo_tamanio);
            tabla_guardar(nueva_tabla, posicion, nuevo_tamanio, hash->tabla[i]);
            hash->cant++;
        }
    }
    free(hash->tabla);
    hash->tabla = nueva_tabla;
    hash->tam = nuevo_tamanio;
    return true; 
}


bool hash_guardar(hash_t *hash, const char *clave, void *dato) {
    if(hay_redimension(hash)){
        if(!hash_redimensionar(hash, hash->tam * FACTOR_MULTIPLICACION)){
            return false;
        }
    }

    for (size_t i = codigo_hash(clave, hash->tam); i <= hash->tam; i++) {
        if(i == hash->tam){
            i = 0;}
        if ((hash->tabla[i].estado == OCUPADO) && (strcmp(hash->tabla[i].clave, clave) == 0)) {
            if(hash->destruir_dato){
                hash->destruir_dato(hash->tabla[i].dato);
            }
            hash->tabla[i].dato = dato;
            break;
        }
        else if (hash->tabla[i].estado == VACIO) {
            hash->tabla[i].clave = strdup(clave);
            hash->tabla[i].dato = dato;
            hash->tabla[i].estado = OCUPADO;
            hash->cant ++;
            break;
        }
        i++;
    }
    return true;
}

void *hash_borrar(hash_t *hash, const char *clave){
    if(hash->cant == 0) return NULL;
    if(!hash_pertenece(hash, clave)) return NULL;
    size_t i = codigo_hash(clave, hash->tam);
    while(hash->tabla[i].estado != VACIO){
        if ((hash->tabla[i].estado == OCUPADO) && (strcmp(hash->tabla[i].clave, clave) == 0)) {
            void* devolver = hash->tabla[i].dato;
            free(hash->tabla[i].clave);
            hash->tabla[i].dato = NULL;
            hash->tabla[i].clave = NULL;
            hash->tabla[i].estado = BORRADO;
            hash->cant --;
            return devolver;
        }
        if(i == hash->tam) i = 0;
        i ++;
    }
    return NULL;
}

struct hash_iter{
    const hash_t* hash;
    size_t actual;
};

hash_iter_t *hash_iter_crear(const hash_t *hash){
    hash_iter_t* iter = malloc(sizeof(hash_iter_t));
    if(!iter) return NULL;
    iter->hash = hash;
    for (size_t i = 0; i < hash->tam; i++){
        if(hash->tabla[i].estado == OCUPADO){
            iter->actual = i;
            break;
        }
    }
    return iter;    
}

bool hash_iter_avanzar(hash_iter_t *iter){
    if(iter->hash->cant == 0) return false;
    if(hash_iter_al_final(iter)) return false;
    iter->actual ++;
    for (size_t i = iter->actual; i < iter->hash->tam; i++){
        if(iter->hash->tabla[i].estado ==OCUPADO){
            iter->actual = i;
            return true;
        }
    }
    return false;
}

const char* hash_iter_ver_actual(const hash_iter_t *iter){
	if (hash_iter_al_final(iter)) return NULL;
	return iter->hash->tabla[iter->actual].clave;
}

bool hash_iter_al_final(const hash_iter_t *iter){
    if (iter->hash->cant == 0) return true;
    if (iter->actual == iter->hash->tam) return true;
    if (iter->hash->tabla[iter->actual].estado ==  VACIO) return true;
    return false;
}

void hash_iter_destruir(hash_iter_t* iter){
    free(iter);
}