#include "src/abb.h"
#include <stdio.h>
#include "pa2mm.h"
#include "string.h"
#include <stdbool.h>

#define MAX_ARRAY 9

int comparador(void* _i1, void* _i2){
  int* i1=_i1;
  int* i2=_i2;
  return *i1-*i2;
}

void creacion_de_arbol_nulo()
{
	abb_t *arbol_prueba = abb_crear(comparador);
	pa2m_afirmar(arbol_prueba != NULL, "El árbol creado existe.");
	pa2m_afirmar(arbol_prueba->nodo_raiz == NULL, "La raíz apunta a NULL.");
	pa2m_afirmar(arbol_prueba->tamanio == 0, "El tamaño del árbol es 0.");
}

void insercion_de_elementos()
{
	abb_t *arbol_prueba = abb_crear(comparador);
	int a = 7, b = 10, c = 13, d = 14, e = 15, f = 17, g = 19, h = 23, i = 29;
	abb_insertar(arbol_prueba, &e);
	abb_insertar(arbol_prueba, &c);
	abb_insertar(arbol_prueba, &h);
	abb_insertar(arbol_prueba, &b);
	abb_insertar(arbol_prueba, &a);
	abb_insertar(arbol_prueba, &d);
	abb_insertar(arbol_prueba, &g);
	abb_insertar(arbol_prueba, &i);
	abb_insertar(arbol_prueba, &f);
	pa2m_afirmar(*(int *)arbol_prueba->nodo_raiz->elemento == 15, "El elemento en la raíz es 15.");
	pa2m_afirmar(*(int *)arbol_prueba->nodo_raiz->derecha->elemento == 23, "El elemento de la derecha es 23.");
	pa2m_afirmar(*(int *)arbol_prueba->nodo_raiz->izquierda->elemento == 13, "El elemento de la izquierda es 13.");
	pa2m_afirmar(*(int *)arbol_prueba->nodo_raiz->derecha->derecha->elemento == 29, "El elemento de la derecha derecha es 29.");
	pa2m_afirmar(*(int *)arbol_prueba->nodo_raiz->derecha->izquierda->elemento == 19, "El elemento de la derecha izquierda es 19.");
	pa2m_afirmar(*(int *)arbol_prueba->nodo_raiz->izquierda->derecha->elemento == 14, "El elemento de la izquierda derecha es 14.");
	pa2m_afirmar(*(int *)arbol_prueba->nodo_raiz->izquierda->izquierda->elemento == 10, "El elemento de la izquierda izquierda es 10.");
	pa2m_afirmar(*(int *)arbol_prueba->nodo_raiz->izquierda->izquierda->izquierda->elemento == 7, "El elemento de la izquierda izquierda izquierda es 7.");
	pa2m_afirmar(*(int *)arbol_prueba->nodo_raiz->derecha->izquierda->izquierda->elemento == 17, "El elemento de la derecha izquierda izquierda es 17.");
}

void quitar_elementos()
{
	abb_t *arbol_prueba = abb_crear(comparador);
	int a = 7, b = 10, c = 13, d = 14, e = 15, f = 17, g = 19, h = 23, i = 29;
	abb_insertar(arbol_prueba, &e);
	abb_insertar(arbol_prueba, &c);
	abb_insertar(arbol_prueba, &h);
	abb_insertar(arbol_prueba, &b);
	abb_insertar(arbol_prueba, &a);
	abb_insertar(arbol_prueba, &d);
	abb_insertar(arbol_prueba, &g);
	abb_insertar(arbol_prueba, &i);
	abb_insertar(arbol_prueba, &f);

	pa2m_afirmar(*(int *)abb_quitar(arbol_prueba, &b) == 10, "El elemento quitado es 10.");
	pa2m_afirmar(*(int *)arbol_prueba->nodo_raiz->izquierda->izquierda->elemento == 7, "El elemento de la izquierda izquierda es 7.");
	pa2m_afirmar(*(int *)abb_quitar(arbol_prueba, &f) == 17, "El elemento quitado es 17.");
	pa2m_afirmar(arbol_prueba->nodo_raiz->derecha->izquierda == NULL, "El elemento de la derecha izquierda no tiene izqueirda.");
	pa2m_afirmar(*(int *)abb_quitar(arbol_prueba, &h) == 17, "El elemento quitado es 23.");
	pa2m_afirmar(*(int *)arbol_prueba->nodo_raiz->derecha->elemento == 19, "El elemento de la derecha es 19.");


}

void buscar_elementos()
{
	abb_t *arbol_prueba = abb_crear(comparador);
	int a = 7, b = 10, c = 13, d = 14, e = 15, f = 17, g = 19, h = 23, i = 29;
	abb_insertar(arbol_prueba, &e);
	abb_insertar(arbol_prueba, &c);
	abb_insertar(arbol_prueba, &h);
	abb_insertar(arbol_prueba, &b);
	abb_insertar(arbol_prueba, &a);
	abb_insertar(arbol_prueba, &d);
	abb_insertar(arbol_prueba, &g);
	abb_insertar(arbol_prueba, &i);
	abb_insertar(arbol_prueba, &f);

	pa2m_afirmar(*(int *)abb_buscar(arbol_prueba, &g) == 19, "El elemento encontrado es 19.");
	pa2m_afirmar(*(int *)abb_buscar(arbol_prueba, &c) == 13, "El elemento encontrado es 13.");

}

void probar_recorridos()
{
	abb_t *arbol_prueba = abb_crear(comparador);
	int elementos[] = {15, 13, 23, 10, 7, 14, 19, 29, 17};

	abb_insertar(arbol_prueba, &elementos[0]);
	abb_insertar(arbol_prueba, &elementos[1]);
	abb_insertar(arbol_prueba, &elementos[2]);
	abb_insertar(arbol_prueba, &elementos[3]);
	abb_insertar(arbol_prueba, &elementos[4]);
	abb_insertar(arbol_prueba, &elementos[5]);
	abb_insertar(arbol_prueba, &elementos[6]);
	abb_insertar(arbol_prueba, &elementos[7]);
	abb_insertar(arbol_prueba, &elementos[8]);

	void *elementos_inorden[MAX_ARRAY];
	void *elementos_preorden[MAX_ARRAY];
	void *elementos_postorden[MAX_ARRAY];

	pa2m_afirmar(abb_recorrer(arbol_prueba, INORDEN, elementos_inorden, MAX_ARRAY) == 9, "Se recorrieron 9 elementos inorden.");
	pa2m_afirmar(abb_recorrer(arbol_prueba, PREORDEN, elementos_preorden, MAX_ARRAY) == 9, "Se recorrieron 9 elementos preorden.");
	pa2m_afirmar(abb_recorrer(arbol_prueba, POSTORDEN, elementos_postorden, MAX_ARRAY) == 9, "Se recorrieron 9 elementos postorden.");
	
	int esperado_inorden[] = {7, 10, 13, 14, 15, 17, 19, 23, 29};
	int esperado_preorden[] = {7, 10, 14, 13, 17, 19, 29, 23, 15};
	int esperado_postorden[] = {15, 13, 10, 7, 14, 23, 19, 17, 29};


	bool flag_inorden = true;

	printf("Elementos inorden: [ ");
	for (int i = 0; i < MAX_ARRAY; i++) {
		printf("%i", *(int *)elementos_inorden[i]);
		if (i < MAX_ARRAY - 1) printf(", ");
		if (*(int *)elementos_inorden[i] != esperado_inorden[i]) flag_inorden = false;
	}
	printf(" ]\n");

	pa2m_afirmar(flag_inorden, "Los elementos salieron en inorden.");


	bool flag_preorden = true;

	printf("Elementos preorden: [ ");
	for (int i = 0; i < MAX_ARRAY; i++) {
		printf("%i", *(int *)elementos_preorden[i]);
		if (i < MAX_ARRAY - 1) printf(", ");
		if (*(int *)elementos_preorden[i] != esperado_preorden[i]) flag_preorden = false;
	}
	printf(" ]\n");

	pa2m_afirmar(flag_preorden, "Los elementos salieron en preorden.");


	bool flag_postorden = true;

	printf("Elementos postorden: [ ");
	for (int i = 0; i < MAX_ARRAY; i++) {
		printf("%i", *(int *)elementos_postorden[i]);
		if (i < MAX_ARRAY - 1) printf(", ");
		if (*(int *)elementos_postorden[i] != esperado_postorden[i]) flag_postorden = false;
	}
	printf(" ]\n");

	pa2m_afirmar(flag_postorden, "Los elementos salieron en postorden.");
}

int main()
{
	pa2m_nuevo_grupo("Pruebas de ABB");

	pa2m_nuevo_grupo("Pruebas de creación de árbol");
	creacion_de_arbol_nulo();

	pa2m_nuevo_grupo("Pruebas de inserción de elementos");
	insercion_de_elementos();

	pa2m_nuevo_grupo("Pruebas buscar elemento");
	buscar_elementos();

	pa2m_nuevo_grupo("Pruebas recorridos");
	probar_recorridos();

	return pa2m_mostrar_reporte();
}
