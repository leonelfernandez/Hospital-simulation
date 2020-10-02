#include "lista.h"
#include <stdlib.h>

typedef struct nodo
{
    void* dato;
    struct nodo* proximo;
} nodo_t;

struct lista
{
    struct nodo* primero;
    struct nodo* ultimo;
    size_t largo;
};

nodo_t* nodo_crear_l(void* valor){
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if(!nodo) return NULL;
    nodo->dato = valor;
    nodo->proximo = NULL;
    return nodo;
}

lista_t *lista_crear(void){
    lista_t* lista = malloc(sizeof(lista_t));
    if(!lista) return NULL;
    lista->largo = 0;
    lista->primero = NULL;
    lista->ultimo = NULL;
    return lista;
}

bool lista_esta_vacia(const lista_t *lista){
    return lista->largo == 0 && lista->primero == NULL && lista->ultimo == NULL;
}

bool lista_insertar_primero(lista_t *lista, void *dato){
    nodo_t* nodo_nuevo = nodo_crear_l(dato);
    if(nodo_nuevo == NULL) return false;
    if(lista_esta_vacia(lista)){
        lista->ultimo = nodo_nuevo;
    }else{
        nodo_nuevo->proximo = lista->primero;
    }
    lista->primero = nodo_nuevo;
    lista->largo ++;
    return true;
}
bool lista_insertar_ultimo(lista_t *lista, void *dato){
    nodo_t* nodo_nuevo = nodo_crear_l(dato);
    if(nodo_nuevo == NULL) return false;
    if(lista_esta_vacia(lista)){
        lista->primero = nodo_nuevo;
    }else{
        lista->ultimo->proximo = nodo_nuevo;
    }
    lista->ultimo = nodo_nuevo;
    lista->largo ++;
    return true;
}

void *lista_borrar_primero(lista_t *lista){
    if(lista_esta_vacia(lista)) return NULL;
    void* dato = lista->primero->dato;
    nodo_t* nodo_nuevo = lista->primero;
    lista->primero = nodo_nuevo->proximo;
    if(lista->largo == 1){
        lista->ultimo = NULL;
    }
    lista->largo --;
    free(nodo_nuevo);
    return dato;
}

void *lista_ver_primero(const lista_t *lista){
    if(lista_esta_vacia(lista)) return NULL;
    return lista->primero->dato;
}

void *lista_ver_ultimo(const lista_t* lista){
    if(lista_esta_vacia(lista)) return NULL;
    return lista->ultimo->dato;
}

size_t lista_largo(const lista_t *lista){
    return lista->largo;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)){
    while(!lista_esta_vacia(lista)){
        void* dato = lista_borrar_primero(lista);
        if(destruir_dato != NULL){
            destruir_dato(dato);
        }
    }
    free(lista);
}

struct lista_iter
{   lista_t* lista;
    nodo_t* actual;
    nodo_t* anterior;
};


lista_iter_t *lista_iter_crear(lista_t *lista){
    lista_iter_t* iterador = malloc(sizeof(lista_iter_t));
    if(!iterador) return NULL;
    iterador->lista = lista;
    iterador->actual = lista->primero;
    iterador->anterior = NULL;
    return iterador;
}
bool lista_iter_al_final(const lista_iter_t *iter){
    return (iter->actual == NULL);
}

bool lista_iter_avanzar(lista_iter_t *iter){
    if(lista_iter_al_final(iter)) return false;
    iter->anterior = iter->actual;
    iter->actual = iter->actual->proximo;
    return true;
}
void *lista_iter_ver_actual(const lista_iter_t *iter){
    if(lista_iter_al_final(iter)) return NULL;
    return iter->actual->dato;
}
void lista_iter_destruir(lista_iter_t *iter){
    free(iter);
}
bool lista_iter_insertar(lista_iter_t *iter, void *dato){
    if(!iter->anterior){
        if(!lista_insertar_primero(iter->lista, dato)){
            return false;
        }
        iter->actual = iter->lista->primero;
        return true;
    }
    if(lista_iter_al_final(iter)){
        if(!lista_insertar_ultimo(iter->lista, dato)){
            return false;
        }
        iter->actual = iter->lista->ultimo;
        return true;
    }
    nodo_t* nuevo_nodo = nodo_crear_l(dato);
    if(!nuevo_nodo) return false;
    iter->lista->largo ++;
    iter->anterior->proximo = nuevo_nodo;
    nuevo_nodo->proximo = iter->actual;
    iter->actual = nuevo_nodo;
    return true;
}
void *lista_iter_borrar(lista_iter_t *iter){
    if(!iter->actual) return NULL;
    void* dato = iter->actual->dato;
    nodo_t* nodo_borrado = iter->actual;
    if(!iter->anterior) iter->lista->primero = iter->actual->proximo;
    if(!iter->actual->proximo) iter->lista->ultimo = iter->anterior;
    if(iter->anterior) iter->anterior->proximo = iter->actual->proximo;
    iter->actual = iter->actual->proximo;
    iter->lista->largo --;
    free(nodo_borrado);
    return dato;
}

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){
	nodo_t* actual = lista->primero;
	while(actual ){
        visitar(actual->dato,extra);
        actual = actual->proximo;
	}
}
