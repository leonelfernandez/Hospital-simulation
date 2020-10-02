#ifndef LISTA_H
#define LISTA_H
#include <stdbool.h>
#include <stdio.h>


typedef struct lista lista_t;

// Pre:
// Post: Devuelve un puntero a la lista, en caso contrario devuelve NULL
lista_t *lista_crear(void);

// Pre: La lista este creada
// Post: Devuelve true si la lista esta vacia, en caso contrario devuelve false
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo elemento al principio de la lista. Devuelve falso en caso de error
// Pre: La lista este creada
// Post: Se agregó un nuevo elemento a la lista,
// valor se encuentra al principio de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Agrega un nuevo elemento al final de la lista. Devuelve falso en caso de error
// Pre: La lista este creada
// Post: Se agregó un nuevo elemento a la lista,
// valor se encuentra al final de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Saca el primer elemento de la lista. Si la lista tiene elementos, se quita el
// primero de la lista, y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía.
void *lista_borrar_primero(lista_t *lista);

// Obtiene el valor del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía.
void *lista_ver_primero(const lista_t *lista);

// Obtiene el valor del ultimo elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del ultimo, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el ultimo elemento de la lista, cuando no está vacía.
void *lista_ver_ultimo(const lista_t* lista);

// Devuelve el tamaño de la lista
// Pre: la lista fue creada
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *));

/* *****************************************************************
 *                 PRIMITIVA DEL ITERADOR INTERNO
 * *****************************************************************/
// Itera la lista de principio a fin.Si visitar no es NULL, se la aplica
// a cada elemento.
// Pre: la lista fue creada.
// Post: Itera la lista de principio a fin o hasta que visitar lo detiene.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

/* *****************************************************************
 *                 PRIMITIVA DEL ITERADOR EXTERNO
 * *****************************************************************/

typedef struct lista_iter lista_iter_t;

// Pre: lista creada
// Post: Devuelve un puntero a la lista, en caso contrario devuelve NULL
lista_iter_t *lista_iter_crear(lista_t *lista);

// Recorre la lista en cada posicion
// Pre: iterador creado
// Post: Devuelve false si el iterador llego al final de la lista,
// en caso contrario devuelve true
bool lista_iter_avanzar(lista_iter_t *iter);

// Pre: iterador creado
// Post: Devuelve el valor de la posicion actual del iterador
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Pre: iterador creado
// Post: Devuelve true si el iterador esta al final de la lista,
// en caso contrario devuelve false
bool lista_iter_al_final(const lista_iter_t *iter);

// Pre: iterador creado
// Post: Destruye el iterador
void lista_iter_destruir(lista_iter_t *iter);

// Inserta un elemento en la posicion actual del iterador.
// Pre: iterador creado
// Post: dato está en la posición actual del iterador
// Devuelve true si pudo agregar el elemento a la lista,
// en caso contrario devuelve false 
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Borra el elemento de la posicion actual del iterador.
// Pre: iterador creado
// Post: Borra y devuelve un elemento de la lista, actual pasa a ser
// el siguiente del elemento borrado. Devuelve NULL si paso el final o esta vacía.
void *lista_iter_borrar(lista_iter_t *iter);

/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

// Realiza pruebas sobre la implementación del alumno.

void pruebas_lista_alumno(void);

#endif // LISTA_H
