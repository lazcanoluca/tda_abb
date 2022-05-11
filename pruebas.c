#include "src/abb.h"
#include <stdio.h>
#include "pa2mm.h"
#include "string.h"

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
	// int a = 13, b = 17, c = 19;
	int d = 17;
	// abb_insertar(arbol_prueba, &b);
	// abb_insertar(arbol_prueba, &a);
	// abb_insertar(arbol_prueba, &c);
	abb_insertar(arbol_prueba, &d);
	pa2m_afirmar(*(int *)arbol_prueba->nodo_raiz->elemento == 17, "El elemento en la raíz es 17");

}

int main()
{
	pa2m_nuevo_grupo("Pruebas de ABB");

	pa2m_nuevo_grupo("Pruebas de creación de árbol");
	creacion_de_arbol_nulo();

	pa2m_nuevo_grupo("Pruebas de inserción de elementos");
	insercion_de_elementos();

	return pa2m_mostrar_reporte();
}
