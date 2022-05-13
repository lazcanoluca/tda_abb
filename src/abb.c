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

// nodo_abb_t *nodo_abb_buscar(nodo_abb_t *raiz, void *elemento, abb_comparador comparador)
// {
// 	if (!raiz || !elemento) return NULL;

// 	if (comparador(elemento, raiz->elemento) == 0)
// 		return raiz;
// 	else if (comparador(elemento, raiz->elemento) < 0) 
// 		return nodo_abb_buscar(raiz->izquierda, elemento, comparador);
// 	else if (comparador(elemento, raiz->elemento) > 0)
// 		return nodo_abb_buscar(raiz->derecha, elemento, comparador);

// 	return NULL;
// }

nodo_abb_t *nodo_max(nodo_abb_t *raiz)
{
	return !raiz->derecha ? raiz : nodo_max(raiz->derecha);
}



nodo_abb_t *nodo_abb_quitar(nodo_abb_t *raiz, void *elemento, abb_comparador comparador, size_t *tamanio, void **quitado)
{
	if (!raiz || !elemento) return NULL;

	int comparacion = comparador(elemento, raiz->elemento);

	// Caso base: el nodo a eliminar es el corriente.
	if (comparacion == 0) {
		// Caso 1: El nodo no tiene hijos (es hoja).
		if (!raiz->izquierda && !raiz->derecha) {
			// free(raiz);
			*quitado = raiz->elemento;
			raiz = NULL;
			(*tamanio)--;
		}
		// Caso 2.1: El nodo tiene un hijo derecho.
		else if (!raiz->izquierda) {
			// nodo_abb_t *aux = raiz;
			*quitado = raiz->elemento;
			raiz = raiz->derecha;
			(*tamanio)--;
			// free(aux);
		}
		// Caso 2.2: El nodo tiene un hijo izquierdo.
		else if (!raiz->derecha) {
			// nodo_abb_t *aux = raiz;
			*quitado = raiz->elemento;
			raiz = raiz->izquierda;
			(*tamanio)--;
			// free(aux);
		}
		// Caso 3: El nodo tiene dos hijos.
		else {
			nodo_abb_t *aux = nodo_max(raiz->izquierda);
			*quitado = raiz->elemento;
			raiz->elemento = aux->elemento;
			void *puntero_residuo = malloc(sizeof(void *));
			raiz->izquierda = nodo_abb_quitar(raiz->izquierda, aux->elemento, comparador, tamanio, puntero_residuo);
			free(puntero_residuo);
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
	if (!arbol || !elemento) return NULL;

	void *quitado;

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
