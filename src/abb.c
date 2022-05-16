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

nodo_abb_t *nodo_max(nodo_abb_t *raiz)
{
	return !raiz->derecha ? raiz : nodo_max(raiz->derecha);
}

nodo_abb_t *nodo_abb_quitar(nodo_abb_t *raiz, void *elemento, abb_comparador comparador, size_t *tamanio, void **quitado)
{
	if (!raiz /*|| !elemento*/) return NULL;

	int comparacion = comparador(elemento, raiz->elemento);

	// Caso base: el nodo a eliminar es el corriente.
	if (comparacion == 0) {
		// Caso 1: El nodo no tiene hijos (es hoja).
		if (!raiz->izquierda && !raiz->derecha) {
			*quitado = raiz->elemento;
			free(raiz);
			raiz = NULL;
			if (tamanio != NULL) (*tamanio)--;
		}
		// Caso 2.1: El nodo tiene un hijo derecho.
		else if (!raiz->izquierda) {
			nodo_abb_t *aux = raiz;
			*quitado = raiz->elemento;
			raiz = raiz->derecha;
			if (tamanio != NULL) (*tamanio)--;
			free(aux);
		}
		// Caso 2.2: El nodo tiene un hijo izquierdo.
		else if (!raiz->derecha) {
			nodo_abb_t *aux = raiz;
			*quitado = raiz->elemento;
			raiz = raiz->izquierda;
			if (tamanio != NULL) (*tamanio)--;
			free(aux);
		}
		// Caso 3: El nodo tiene dos hijos.
		else {
			nodo_abb_t *aux = nodo_max(raiz->izquierda);
			*quitado = raiz->elemento;
			raiz->elemento = aux->elemento;
			void *puntero_residuo = malloc(sizeof(void *));
			raiz->izquierda = nodo_abb_quitar(raiz->izquierda, aux->elemento, comparador, NULL, puntero_residuo);
			free(puntero_residuo);
			if (tamanio != NULL) (*tamanio)--;
			// free(aux);
		}
		// return raiz;
	}

	// Caso recursivo: buscamos el nodo con el elemento a eliminar.
	if (comparacion < 0) {
		raiz->izquierda = nodo_abb_quitar(raiz->izquierda, elemento, comparador, tamanio, quitado);
	} else if (comparacion > 0) {
		raiz->derecha = nodo_abb_quitar(raiz->derecha, elemento, comparador, tamanio, quitado);
	}

	return raiz;
}

void *abb_quitar(abb_t *arbol, void *elemento)
{
	if (!arbol /*|| !elemento*/ || abb_vacio(arbol)) return NULL;

	void *quitado = NULL;

	arbol->nodo_raiz = nodo_abb_quitar(arbol->nodo_raiz, elemento, arbol->comparador, &arbol->tamanio, &quitado);

	return quitado;
}

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
	if (!arbol || !elemento) return NULL;

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

void inorden(nodo_abb_t *raiz, void **array, size_t tamanio_array, size_t *elementos_recorridos)
{
	if (!raiz || *elementos_recorridos == tamanio_array) return;

	if (raiz->izquierda) inorden(raiz->izquierda, array, tamanio_array, elementos_recorridos);

	array[*elementos_recorridos] = raiz->elemento;
	(*elementos_recorridos)++;

	if (raiz->derecha) inorden(raiz->derecha, array, tamanio_array, elementos_recorridos);
}

void preorden(nodo_abb_t *raiz, void **array, size_t tamanio_array, size_t *elementos_recorridos)
{
	if (!raiz || *elementos_recorridos == tamanio_array) return;

	array[*elementos_recorridos] = raiz->elemento;
	(*elementos_recorridos)++;

	if (raiz->izquierda) preorden(raiz->izquierda, array, tamanio_array, elementos_recorridos);

	if (raiz->derecha) preorden(raiz->derecha, array, tamanio_array, elementos_recorridos);
}

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
