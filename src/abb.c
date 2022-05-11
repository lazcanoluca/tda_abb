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

void nodo_abb_insertar(nodo_abb_t **raiz, void *elemento, abb_comparador comparador, size_t *tamanio)
{
	if (!*raiz) {
		nodo_abb_t *nueva_hoja = calloc(1, sizeof(nodo_abb_t));
		if (!nueva_hoja) return;

		nueva_hoja->elemento = elemento;
		*raiz = nueva_hoja;
		(*tamanio)++;

		return;
	}

	if (comparador(elemento, (*raiz)->elemento) <= 0) {
		nodo_abb_insertar(&(*raiz)->izquierda, elemento, comparador, tamanio);
	} else {
		nodo_abb_insertar(&(*raiz)->derecha, elemento, comparador, tamanio);
	}
	return;
}

abb_t *abb_insertar(abb_t *arbol, void *elemento)
{
	if (!arbol) return NULL;

	nodo_abb_insertar(&arbol->nodo_raiz, elemento, arbol->comparador, &arbol->tamanio);

	return arbol;
}

nodo_abb_t *nodo_abb_buscar(nodo_abb_t *raiz, void *elemento, abb_comparador comparador)
{
	if (!raiz || !elemento) return NULL;

	if (comparador(elemento, raiz->elemento) == 0)
		return raiz;
	else if (comparador(elemento, raiz->elemento) < 0) 
		return nodo_abb_buscar(raiz->izquierda, elemento, comparador);
	else if (comparador(elemento, raiz->elemento) > 0)
		return nodo_abb_buscar(raiz->derecha, elemento, comparador);

	return NULL;

}

nodo_abb_t *nodo_abb_buscar_padre(nodo_abb_t *raiz, void *elemento, abb_comparador comparador)
{
	if (!raiz || !elemento) return NULL;

	if (comparador(elemento, raiz->derecha->elemento) == 0 ||
		comparador(elemento, raiz->izquierda->elemento) == 0)
		return raiz;
	else if (comparador(elemento, raiz->elemento) < 0)
		return nodo_abb_buscar_padre(raiz->izquierda, elemento, comparador);
	else if (comparador(elemento, raiz->elemento) > 0)
		return nodo_abb_buscar_padre(raiz->derecha, elemento, comparador);
	
	return NULL;
}

int cantidad_hijos(nodo_abb_t *nodo)
{
	if (!nodo) return -1;
	else if (nodo->derecha != NULL && nodo->izquierda != NULL) return 2;
	else if (nodo->derecha == NULL && nodo->izquierda == NULL) return 0;
	else return 1;
}

void *abb_quitar(abb_t *arbol, void *elemento)
{
	// nodo_abb_t *nodo_a_quitar = nodo_abb_buscar(arbol->nodo_raiz, elemento, arbol->comparador);
	// // nodo_abb_t *nodo_padre = nodo_abb_buscar_padre(arbol->nodo_raiz, elemento, arbol->comparador);
	// // if (!nodo_padre) return NULL;
	// // nodo_abb_t *nodo_a_quitar;
	// // nodo_abb_t **puntero;

	// // if (comparador(elemento, nodo_padre->elemento) < 0) {
	// // 	nodo_a_quitar = nodo_padre->izquierda;
	// // 	puntero = &nodo_padre->izquierda;
	// // } else {
	// // 	nodo_a_quitar = nodo_padre->derecha;
	// // 	puntero = &nodo_padre->derecha;
	// // }

	// // if (cantidad_hijos(nodo_a_quitar) == 0) {
	// // 	*puntero = NULL;
	// // 	free(nodo_a_quitar);
	// // } else if (cantidad_hijos(nodo_a_quitar) == 1) {
	// // 	*puntero = nodo_a_quitar->izquierda != NULL ?
	// // 		nodo_a_quitar->izquierda :
	// // 		nodo_a_quitar->derecha;
	// // 	free(nodo_a_quitar);
	// // } else if (cantidad_hijos(nodo_a_quitar) == 2) {

	// // }

	return NULL;
}

void *abb_buscar(abb_t *arbol, void *elemento)
{
	return NULL;
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
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *))
{
}

size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido,
			     bool (*funcion)(void *, void *), void *aux)
{
	return 0;
}

size_t abb_recorrer(abb_t *arbol, abb_recorrido recorrido, void **array,
		    size_t tamanio_array)
{
	return 0;
}
