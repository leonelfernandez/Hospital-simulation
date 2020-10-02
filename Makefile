run: build
	./TP2

zyxcba.o: zyxcba.c
	gcc zyxcba.c -c

funciones_tp2.o: funciones_tp2.c
	gcc funciones_tp2.c -c

strutil.o: strutil.c
	gcc strutil.c -c

pila.o: pila.c
	gcc pila.c -c

cola.o: cola.c
	gcc cola.c -c

lista.o: lista.c
	gcc lista.c -c

heap.o: heap.c
	gcc heap.c -c

hash.o: hash.c
	gcc hash.c -c

abb.o: abb.c
	gcc abb.c -c

estructuras_aux.o: estructuras_aux.c
	gcc estructuras_aux.c -c

csv.o: csv.c
	gcc csv.c -c

build: zyxcba.o funciones_tp2.o strutil.o pila.o cola.o lista.o hash.o abb.o estructuras_aux.o csv.o heap.o
	cc zyxcba.o funciones_tp2.o strutil.o pila.o cola.o lista.o hash.o abb.o estructuras_aux.o csv.o heap.o -o TP2