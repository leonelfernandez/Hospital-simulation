#define _POSIX_C_SOURCE 200809L
#include "abb.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pila.h"

typedef struct nodo_abb {
    struct nodo_abb* izq;
    struct nodo_abb* der;
    const char* clave;
    void* dato;
} nodo_abb_t;

struct abb {
    nodo_abb_t* raiz;
    size_t cant;
    abb_destruir_dato_t destruir;
    abb_comparar_clave_t comparar;
};

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* abb = malloc(sizeof(abb_t));
    if(!abb) return NULL;
    abb->cant = 0;
    abb->comparar = cmp;
    abb->destruir = destruir_dato;
    abb->raiz = NULL;
    return abb;
}

nodo_abb_t* nodo_crear(const char* clave, void* dato){
    nodo_abb_t* nodo = malloc(sizeof(nodo_abb_t));
    if(!nodo) return NULL;
    char* clave_auxiliar = strdup(clave);
    if(!clave_auxiliar){
        return NULL;
    }
    nodo->clave = clave_auxiliar;
    nodo->dato = dato;
    nodo->der = NULL;
    nodo->izq = NULL;
    return nodo;
}

nodo_abb_t* buscar_clave(const abb_t* arbol, nodo_abb_t* nodo, const char* clave){
    if (arbol->cant == 0) return NULL;
    if (arbol->comparar(clave, nodo->clave) == 0) return nodo;
    else if (arbol->comparar(clave, nodo->clave) < 0) {
        if (nodo->izq != NULL) {
            return buscar_clave(arbol, nodo->izq, clave);
        }
        else {
            return NULL;
        }
    }
    else {
        if (nodo->der != NULL) {
            return buscar_clave(arbol, nodo->der, clave);
        }
        else {
            return NULL;
        }
    }
}

nodo_abb_t* buscar_padre(abb_t* arbol, nodo_abb_t* nodo, const char* clave) {
    if (arbol->comparar(clave, nodo->clave) < 0) {
        if (nodo->izq == NULL || arbol->comparar(clave, nodo->izq->clave) == 0) {
            return nodo;
        }
        return buscar_padre(arbol, nodo->izq, clave);
    }
    else {
        if (nodo->der == NULL || arbol->comparar(clave, nodo->der->clave) == 0) {
            return nodo;
        }
        return buscar_padre(arbol, nodo->der, clave);
    }
}
 
size_t abb_cantidad(abb_t *arbol){
    return arbol->cant;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
    if (arbol == NULL) return false;
    if (arbol->cant == 0) {
        nodo_abb_t* nodo = nodo_crear(clave, dato);
        arbol->raiz = nodo;
        arbol->cant ++;
        return true;
    } else {
        nodo_abb_t* nodo_aux_1 = buscar_clave(arbol, arbol->raiz, clave);
        if (nodo_aux_1 != NULL) { //Caso que el nodo_aux no es NULL: Hay un nodo con esa clave ya en el ABB
            if (arbol->destruir) { 
                arbol->destruir(nodo_aux_1->dato); //Destruyo el dato y pongo el nuevo
            }
            nodo_aux_1->dato = dato;
        } else {
            nodo_abb_t* nodo_padre = buscar_padre(arbol, arbol->raiz, clave);
            nodo_abb_t* nodo_aux_2 = nodo_crear(clave, dato);
            if (nodo_aux_2 == NULL) return false;
            if(arbol->comparar(clave, nodo_padre->clave) < 0) {
               nodo_padre->izq = nodo_aux_2;
            } else {
                nodo_padre->der = nodo_aux_2;
            }
            arbol->cant ++; 
        }
        return true;
    }   
}

void* liberar_nodo(nodo_abb_t* nodo) {
    void* res = nodo->dato;
    free((char*)nodo->clave);
    free(nodo);
    return res;
}

size_t contar_hijos(nodo_abb_t* nodo) {
    if (nodo->der != NULL && nodo->izq != NULL) return 2;
    if ((nodo->der != NULL && nodo->izq == NULL) || (nodo->der == NULL && nodo->izq != NULL)) return 1;
    return 0;
}
 
void* eliminar(abb_t* arbol, nodo_abb_t* nodo, const char* clave, nodo_abb_t* nuevo_nodo, nodo_abb_t* padre) {
    if (padre == NULL) {
        arbol->raiz = nuevo_nodo;
    } else {
        if (padre->der != NULL && arbol->comparar(padre->der->clave, clave) == 0) {
            padre->der = nuevo_nodo;
        } else {
            padre->izq = nuevo_nodo;
        }
    }
    void* dato_aux = liberar_nodo(nodo); //Me lo guardo para devolverlo en el abb_borrar
    arbol->cant--;
    return dato_aux;
}
 
nodo_abb_t* buscar_minimo(nodo_abb_t* nodo){
    nodo_abb_t* actual = nodo;
    while (actual->izq != NULL) {
        actual = actual->izq;
    }
    return actual;
}
 
void swapear(nodo_abb_t* nodo_1, nodo_abb_t* nodo_2){
    const char* clave_1 = nodo_1->clave;
    void* valor_1 = nodo_1->dato;
    nodo_1->clave = nodo_2->clave;
    nodo_1->dato = nodo_2->dato;
    nodo_2->clave = clave_1;
    nodo_2->dato = valor_1;
}
 
void* eliminar_ambos_hijos(abb_t* arbol, const char* clave, nodo_abb_t* nodo) {
    nodo_abb_t* nuevo_nodo = buscar_minimo(nodo->der);
    nodo_abb_t* nuevo_padre = buscar_padre(arbol, arbol->raiz, nuevo_nodo->clave);
    swapear(nodo, nuevo_nodo);
    if(nuevo_nodo->der != NULL) {
        return eliminar(arbol, nuevo_nodo, clave, nuevo_nodo->der, nuevo_padre);
    } else if (nuevo_nodo->izq != NULL){
        return eliminar(arbol, nuevo_nodo, clave, nuevo_nodo->izq, nuevo_padre);
    }
    return eliminar(arbol, nuevo_nodo, clave, NULL, nuevo_padre);
}

void *abb_borrar(abb_t *arbol, const char *clave) { 
    if (arbol == NULL) return NULL;
    if (arbol->cant == 0) return NULL;
    if (!abb_pertenece(arbol, clave)) return NULL;
    nodo_abb_t* nodo_borrado = buscar_clave(arbol, arbol->raiz, clave);
    size_t hijos = contar_hijos(nodo_borrado);
    nodo_abb_t* padre = NULL;
    if (hijos == 0) {
        if (arbol->comparar(arbol->raiz->clave, nodo_borrado->clave) != 0) { // Si no es raiz
            padre = buscar_padre(arbol, arbol->raiz, clave);
        }
        return eliminar(arbol, nodo_borrado, clave, NULL, padre);
    } else if (hijos == 1) {
        if (arbol->comparar(arbol->raiz->clave, nodo_borrado->clave) != 0) { // Si no es raiz
            padre = buscar_padre(arbol, arbol->raiz, clave);
        }
        if (nodo_borrado->izq != NULL) {
            return eliminar(arbol, nodo_borrado, clave, nodo_borrado->izq, padre);
        } else {
            return eliminar(arbol, nodo_borrado, clave, nodo_borrado->der, padre);
        }
    } else {
        return eliminar_ambos_hijos(arbol, clave, nodo_borrado);
    } 
}

void raiz_destruir(nodo_abb_t* raiz, abb_destruir_dato_t destruir_dato){
	if(!raiz) return;
    raiz_destruir(raiz->izq, destruir_dato);
	raiz_destruir(raiz->der, destruir_dato);
    void* dato = liberar_nodo(raiz);
    if(destruir_dato){
        destruir_dato(dato);
    }
}

void abb_destruir(abb_t *arbol){
    if(!arbol) return;
    raiz_destruir(arbol->raiz, arbol->destruir);
    free(arbol);
}

void *abb_obtener(const abb_t *arbol, const char *clave){
    nodo_abb_t* nodo_clave = buscar_clave(arbol, arbol->raiz, clave);
    return !nodo_clave ? NULL : nodo_clave->dato; 
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
    return buscar_clave(arbol, arbol->raiz, clave) != NULL;
}

void iterador_inorder(nodo_abb_t* nodo, bool visitar(const char *, void *, void *),void* extra, bool* continuar){
	if(!nodo || !*continuar)  return;
	iterador_inorder(nodo->izq,visitar,extra,continuar);
	if(*continuar){
		*continuar = visitar(nodo->clave,nodo->dato,extra);
		if(!*continuar) return;
	}
	iterador_inorder(nodo->der,visitar,extra,continuar);
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
	if(!arbol) return;
	if(!visitar) return;
	bool continuar = true;
	iterador_inorder(arbol->raiz,visitar,extra,&continuar);
}

struct abb_iter{
    pila_t* pila;
};

void apilar_hijos_izq(abb_iter_t* iter, nodo_abb_t* nodo) {
    if (nodo == NULL) return;
    pila_apilar(iter->pila, nodo);
    apilar_hijos_izq(iter, nodo->izq);
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol) {
    if(!arbol) return NULL;
    abb_iter_t* iter = malloc(sizeof(abb_iter_t));
    if(!iter) return NULL;
    pila_t* pila = pila_crear();
    if(!pila) return NULL;
    iter->pila = pila;
    nodo_abb_t* nodo = arbol->raiz;
    apilar_hijos_izq(iter, nodo);
    return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter) {
    if (abb_iter_in_al_final(iter)) return false;
    nodo_abb_t* nodo_desapilado = pila_desapilar(iter->pila);
    if (nodo_desapilado->der) {
        apilar_hijos_izq(iter, nodo_desapilado->der);
    }
    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter) {
    if (abb_iter_in_al_final(iter)) return NULL;
    return ((nodo_abb_t*)pila_ver_tope(iter->pila))->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter) {
    return (pila_esta_vacia(iter->pila));
}

void abb_iter_in_destruir(abb_iter_t* iter) {
    pila_destruir(iter->pila);
    free(iter);
}