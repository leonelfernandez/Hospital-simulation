#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "heap.h"

#define NUEVO_TAMANIO 16
#define TAMANIO_INICIAL 4

struct heap{
    void** datos;
    cmp_func_t cmp;
    size_t tam;
    size_t cant;
};

heap_t *heap_crear(cmp_func_t cmp){
    heap_t* heap = malloc(sizeof(heap_t));
    if(!heap) return NULL;
    heap->datos = malloc(sizeof(void*)*TAMANIO_INICIAL);
    if(!heap->datos){ 
        free(heap);
        return NULL;
    }
    heap->cmp = cmp;
    heap->cant = 0;
    heap->tam = TAMANIO_INICIAL;
    return heap;
}

bool heap_esta_vacio(const heap_t *heap){
    return heap->cant == 0;
}

void *heap_ver_max(const heap_t *heap){
    if(!heap->cant) return NULL;
    return heap->datos[0];
}

size_t heap_cantidad(const heap_t *heap){
    return heap->cant;
}

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)){
    if(destruir_elemento){
        for (size_t i = 0; i < heap->cant; i++){
            destruir_elemento(heap->datos[i]);
        }
    }
    free(heap->datos);
    free(heap);
}

void swap(void* arreglo[], size_t posicion_1, size_t posicion_2){
	void* aux = arreglo[posicion_1];
	arreglo[posicion_1] = arreglo[posicion_2];
	arreglo[posicion_2] = aux;
}

void upheap(void* arreglo[], size_t pos, cmp_func_t cmp){
	if (pos == 0) return;
	size_t padre = (pos-1) / 2;
	if (cmp(arreglo[pos], arreglo[padre]) > 0){
		swap(arreglo,pos,padre);
		upheap(arreglo, padre, cmp);
    }
}

void downheap(void* A[], size_t tam, size_t pos, cmp_func_t cmp){
    if (pos >= tam) return;
    size_t min = pos; // PADRE
    size_t izq = 2 * pos + 1;
    size_t der = 2 * pos + 2;
    if (izq < tam && cmp(A[izq], A[min]) > 0)
        min = izq;
    if (der < tam && cmp(A[der], A[min]) > 0)
        min = der;
    if (min != pos){
        swap(A, pos, min);
        downheap(A, tam, min, cmp);
    }
}

void heapify(void* arreglo[], size_t tam, cmp_func_t cmp){
    for (size_t pos = tam/2; pos > 0; pos--){
        downheap(arreglo, tam, pos-1, cmp);
    }
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp) {
    heap_t* heap = malloc(sizeof(heap_t));
    if (heap == NULL) return NULL;
    heap->datos = malloc(sizeof(void*) * n);      
    if(!heap->datos){
        free(heap);
        return NULL;
    }
    heap->cant = 0;
    for (size_t i = 0; i < n; i ++) {
        heap->datos[i] = arreglo[i];
        heap->cant ++;
    }
    heap->cmp = cmp;
    heap->tam = n;
    heapify(heap->datos, heap->tam, heap->cmp);
    return heap;
}

void heap_sort(void *elementos[], size_t tam, cmp_func_t cmp){
    heapify(elementos, tam, cmp);
    for (size_t i = tam-1; i > 0; i--){
        swap(elementos,0,i);
        tam--;
        downheap(elementos, tam, 0, cmp);
    }   
}

bool heap_redimensionar(heap_t* heap, size_t new_size){
    void** datos_nuevos = realloc(heap->datos, sizeof(void*)*new_size);
    if(!datos_nuevos) return false;
    heap->datos = datos_nuevos;
    heap->tam = new_size;
    return true;
}

bool heap_encolar(heap_t *heap, void *elem){
    if(elem == NULL) return false;
    if(heap->cant >= heap->tam){
        if(!heap_redimensionar(heap, heap->tam*NUEVO_TAMANIO)) return false;
    }
    if(heap_esta_vacio(heap)) heap->datos[0] = elem;
    else{
        heap->datos[heap->cant] = elem;
        upheap(heap->datos, heap->cant, heap->cmp);
    }
    heap->cant ++;
    return true;
}

void *heap_desencolar(heap_t *heap){
    if(heap_esta_vacio(heap)) return NULL;
    void* devolver = heap->datos[0];
    heap->datos[0] = heap->datos[heap->cant-1];
    downheap(heap->datos,heap->cant-1,0,heap->cmp);
    if(heap->cant <= heap->tam/2 && heap->tam/2 >= TAMANIO_INICIAL){
        if(!heap_redimensionar(heap, heap->tam / 2)) return false;
    }
    heap->cant --;
    return devolver;
}