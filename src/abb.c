#include "abb.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

abb_t *abb_crear(abb_comparador comparador)
{
	if (!comparador) return NULL;

	abb_t *arbol = calloc(1, sizeof(abb_t));
	if (!arbol) return NULL;

	arbol->comparador = comparador;
	return arbol;
}

/**
 * Recibe la raíz de un árbol (que puede o no ser nula), un puntero al elemento a insertar,
 * la función comparadora y un puntero al contador del tamaño del árbol. La función comparadora
 * y el puntero al contador no pueden ser nulos.
 * 
 * Devuelve la raíz de un árbol con el elemento insertado en la ubicación correspondiente, y el
 * contador del tamaño habrá sido actualizado.
 */
nodo_abb_t *nodo_abb_insertar(nodo_abb_t *raiz, void *elemento, abb_comparador comparador, size_t *tamanio)
{
	if (!raiz) {
		nodo_abb_t *nueva_hoja = calloc(1, sizeof(nodo_abb_t));
		if (!nueva_hoja) return NULL;

		nueva_hoja->elemento = elemento;
		(*tamanio)++;

		return nueva_hoja;
	}

	int comparacion = comparador(elemento, raiz->elemento);

	if (comparacion <= 0) {
		raiz->izquierda = nodo_abb_insertar(raiz->izquierda, elemento, comparador, tamanio);
	} else {
		raiz->derecha = nodo_abb_insertar(raiz->derecha, elemento, comparador, tamanio);
	}

	return raiz;
}

abb_t *abb_insertar(abb_t *arbol, void *elemento)
{
	if (!arbol) return NULL;

	arbol->nodo_raiz = nodo_abb_insertar(arbol->nodo_raiz, elemento, arbol->comparador, &arbol->tamanio);

	return arbol;
}

/**
 * Recibe la raíz de un árbol que no debería ser nula.
 * 
 * Devuelve un puntero al nodo "más a la derecha", o la propia raíz,
 * si ya lo es.
 */
nodo_abb_t *nodo_max(nodo_abb_t *raiz)
{
	return !raiz->derecha ? raiz : nodo_max(raiz->derecha);
}

/**
 * Recibe la raíz de un árbol que no debería ser nula, un puntero al elemento a eliminar,
 * la función comparadora, un puntero al contador del tamaño del árbol y un puntero para
 * almacenar el elemento quitado. La función comparadora y el puntero al contador no pueden ser
 * nulos.
 * 
 * Devuelve la raíz de un árbol con el elemento quitado, y el contador del tamaño habrá sido
 * actualizado. En caso de no encontrar el elemento o haber provisto una raíz nula, devolverá
 * NULL.
 */
nodo_abb_t *nodo_abb_quitar(nodo_abb_t *raiz, void *elemento, abb_comparador comparador, size_t *tamanio, void **quitado)
{
	if (!raiz) return NULL;

	int comparacion = comparador(elemento, raiz->elemento);

	if (comparacion == 0) {
		if (!raiz->izquierda || !raiz->derecha){
			nodo_abb_t *aux = raiz;
			if (quitado != NULL ) *quitado = raiz->elemento;

			raiz = raiz->izquierda != NULL ? raiz->izquierda :
				   raiz->derecha != NULL ? raiz->derecha :
				   NULL;

			free(aux);
		} else {
			nodo_abb_t *predecesor = nodo_max(raiz->izquierda);
			if (quitado != NULL ) *quitado = raiz->elemento;
			raiz->elemento = predecesor->elemento;
			raiz->izquierda = nodo_abb_quitar(raiz->izquierda, predecesor->elemento, comparador, NULL, NULL);
		}
		if (tamanio != NULL) (*tamanio)--;
	}

	if (comparacion < 0) raiz->izquierda = nodo_abb_quitar(raiz->izquierda, elemento, comparador, tamanio, quitado);
	else if (comparacion > 0) raiz->derecha = nodo_abb_quitar(raiz->derecha, elemento, comparador, tamanio, quitado);

	return raiz;
}

void *abb_quitar(abb_t *arbol, void *elemento)
{
	if (!arbol || abb_vacio(arbol)) return NULL;

	void *quitado = NULL;

	arbol->nodo_raiz = nodo_abb_quitar(arbol->nodo_raiz, elemento, arbol->comparador, &arbol->tamanio, &quitado);

	return quitado;
}

/**
 * Recibe la raíz de un árbol que no debería ser nula, un puntero al elemento buscado, y
 * la función comparadora. La función comparadora no puede ser nula.
 * 
 * Devuelve el elemento encontrado o NULL en su defecto.
 */
void *nodo_abb_buscar(nodo_abb_t *raiz, void *elemento, abb_comparador comparador)
{
	if (!raiz) return NULL;

	int comparacion = comparador(elemento, raiz->elemento);

	if (comparacion == 0) return raiz->elemento;
	else if (comparacion < 0) return nodo_abb_buscar(raiz->izquierda, elemento, comparador);
	else return nodo_abb_buscar(raiz->derecha, elemento, comparador);
}

void *abb_buscar(abb_t *arbol, void *elemento)
{
	if (!arbol) return NULL;

	return nodo_abb_buscar(arbol->nodo_raiz, elemento, arbol->comparador);
}

bool abb_vacio(abb_t *arbol)
{
	if (!arbol || arbol->tamanio == 0) return true;
	return false;
}

size_t abb_tamanio(abb_t *arbol)
{
	if (!arbol) return 0;
	return arbol->tamanio;
}

void abb_destruir(abb_t *arbol)
{
	abb_destruir_todo(arbol, NULL);
}

/**
 * Recibe la raíz de un árbol que no debería ser nula y un puntero a una función destructora,
 * que puede ser nulo.
 * 
 * Destruye todos los elementos si se ha provisto una función destructora, y libera la
 * memoria de cada nodo del árbol.
 */
void nodo_abb_destruir_todo(nodo_abb_t *raiz, void (*destructor)(void *))
{
	if (!raiz) return;

	if (raiz->izquierda) nodo_abb_destruir_todo(raiz->izquierda, destructor);

	if (raiz->derecha) nodo_abb_destruir_todo(raiz->derecha, destructor);

	if (destructor) destructor(raiz->elemento);
	free(raiz);
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *))
{
	if (!arbol) return;

	nodo_abb_destruir_todo(arbol->nodo_raiz, destructor);
	free(arbol);
}

/**
 * Recibe la raíz de un árbol, un puntero a una función de tipo booleana, un puntero a
 * un elemento que será recibida por la función booleana, y un puntero al contador de
 * invocaciones de la función.
 * 
 * Recorre el árbol de forma inorden hasta que la función pasada dé false para algún
 * elemento, donde devuelve false, o se recorra todo el árbol donde devolverá true.
 */
bool inorden_para_cada(nodo_abb_t *raiz, bool (*funcion)(void *, void *), void *aux, size_t *funcion_invocaciones)
{
	if (!raiz) return true;

	if (raiz->izquierda) {
		if (!inorden_para_cada(raiz->izquierda, funcion, aux, funcion_invocaciones)) return false;
	}

	(*funcion_invocaciones)++;
	if (!funcion(raiz->elemento, aux)) return false;

	if (raiz->derecha){
		if (!inorden_para_cada(raiz->derecha, funcion, aux, funcion_invocaciones)) return false;
	}

	return true;
}

/**
 * Recibe la raíz de un árbol, un puntero a una función de tipo booleana, un puntero a
 * un elemento que será recibida por la función booleana, y un puntero al contador de
 * invocaciones de la función.
 * 
 * Recorre el árbol de forma postorden hasta que la función pasada dé false para algún
 * elemento, donde devuelve false, o se recorra todo el árbol donde devolverá true.
 */
bool postorden_para_cada(nodo_abb_t *raiz, bool (*funcion)(void *, void *), void *aux, size_t *funcion_invocaciones)
{
	if (!raiz) return true;

	if (raiz->izquierda) {
		if (!postorden_para_cada(raiz->izquierda, funcion, aux, funcion_invocaciones)) return false;
	}

	if (raiz->derecha){
		if (!postorden_para_cada(raiz->derecha, funcion, aux, funcion_invocaciones)) return false;
	}

	(*funcion_invocaciones)++;
	if (!funcion(raiz->elemento, aux)) return false;

	return true;
}

/**
 * Recibe la raíz de un árbol, un puntero a una función de tipo booleana, un puntero a
 * un elemento que será recibida por la función booleana, y un puntero al contador de
 * invocaciones de la función.
 * 
 * Recorre el árbol de forma preorden hasta que la función pasada dé false para algún
 * elemento, donde devuelve false, o se recorra todo el árbol donde devolverá true.
 */
bool preorden_para_cada(nodo_abb_t *raiz, bool (*funcion)(void *, void *), void *aux, size_t *funcion_invocaciones)
{
	if (!raiz) return true;

	(*funcion_invocaciones)++;
	if (!funcion(raiz->elemento, aux)) return false;

	if (raiz->izquierda) {
		if (!preorden_para_cada(raiz->izquierda, funcion, aux, funcion_invocaciones)) return false;
	}

	if (raiz->derecha){
		if (!preorden_para_cada(raiz->derecha, funcion, aux, funcion_invocaciones)) return false;
	}

	return true;
}

size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido,
			     bool (*funcion)(void *, void *), void *aux)
{
	if (!arbol | !funcion) return 0;

	size_t funcion_invocaciones = 0;

	switch (recorrido) {
		case INORDEN:
			inorden_para_cada(arbol->nodo_raiz, funcion, aux, &funcion_invocaciones);
			break;
		
		case PREORDEN:
			preorden_para_cada(arbol->nodo_raiz, funcion, aux, &funcion_invocaciones);
			break;
		
		case POSTORDEN:
			postorden_para_cada(arbol->nodo_raiz, funcion, aux, &funcion_invocaciones);
			break;

		default:
			return 0;
			break;
	}

	return funcion_invocaciones;
}

/**
 * Recibe la raíz de un árbol, un puntero a un array donde guardar los elementos
 * recorridos, el tamanio máximo del array y un puntero al contador de elementos
 * recorridos.
 * 
 * Recorre el árbol de forma inorden hasta que se llene el array o se hayan
 * recorrido todos los elementos.
 */
void inorden(nodo_abb_t *raiz, void **array, size_t tamanio_array, size_t *elementos_recorridos)
{
	if (!raiz || *elementos_recorridos == tamanio_array) return;

	if (raiz->izquierda) inorden(raiz->izquierda, array, tamanio_array, elementos_recorridos);

	array[*elementos_recorridos] = raiz->elemento;
	(*elementos_recorridos)++;

	if (raiz->derecha) inorden(raiz->derecha, array, tamanio_array, elementos_recorridos);
}

/**
 * Recibe la raíz de un árbol, un puntero a un array donde guardar los elementos
 * recorridos, el tamanio máximo del array y un puntero al contador de elementos
 * recorridos.
 * 
 * Recorre el árbol de forma preorden hasta que se llene el array o se hayan
 * recorrido todos los elementos.
 */
void preorden(nodo_abb_t *raiz, void **array, size_t tamanio_array, size_t *elementos_recorridos)
{
	if (!raiz || *elementos_recorridos == tamanio_array) return;

	array[*elementos_recorridos] = raiz->elemento;
	(*elementos_recorridos)++;

	if (raiz->izquierda) preorden(raiz->izquierda, array, tamanio_array, elementos_recorridos);

	if (raiz->derecha) preorden(raiz->derecha, array, tamanio_array, elementos_recorridos);
}

/**
 * Recibe la raíz de un árbol, un puntero a un array donde guardar los elementos
 * recorridos, el tamanio máximo del array y un puntero al contador de elementos
 * recorridos.
 * 
 * Recorre el árbol de forma postorden hasta que se llene el array o se hayan
 * recorrido todos los elementos.
 */
void postorden(nodo_abb_t *raiz, void **array, size_t tamanio_array, size_t *elementos_recorridos)
{
	if (!raiz || *elementos_recorridos == tamanio_array) return;

	if (raiz->izquierda) {
		postorden(raiz->izquierda, array, tamanio_array, elementos_recorridos);
		if ((*elementos_recorridos) == tamanio_array) return;
	}

	if (raiz->derecha) {
		postorden(raiz->derecha, array, tamanio_array, elementos_recorridos);
		if ((*elementos_recorridos) == tamanio_array) return;
	}

	array[*elementos_recorridos] = raiz->elemento;
	(*elementos_recorridos)++;
}

size_t abb_recorrer(abb_t *arbol, abb_recorrido recorrido, void **array, size_t tamanio_array)
{
	if (!arbol) return 0;

	size_t elementos_recorridos = 0;

	switch (recorrido) {
		case INORDEN:
			inorden(arbol->nodo_raiz, array, tamanio_array, &elementos_recorridos);
			break;
		
		case PREORDEN:
			preorden(arbol->nodo_raiz, array, tamanio_array, &elementos_recorridos);
			break;
		
		case POSTORDEN:
			postorden(arbol->nodo_raiz, array, tamanio_array, &elementos_recorridos);
			break;

		default:
			return 0;
			break;
	}

	return elementos_recorridos;
}
