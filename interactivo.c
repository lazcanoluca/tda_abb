#include "mostrar_abb.h"
#include "src/abb.h"
#include <stdio.h>
#include <ctype.h>

void formatear_int(void* _nodo){
  nodo_abb_t* nodo = _nodo;
  int i = *(int*)nodo->elemento;
  printf("%2d", i);
}

int comparar_int(void* _i1, void* _i2){
  int* i1=_i1;
  int* i2=_i2;
  return *i1-*i2;
}

void destruir_int(void* i){
  free(i);
}

int main(int argc, char* argv[]){
  abb_t* abb = abb_crear(comparar_int);

  printf("\n+<valor> inserta un valor, -<valor> elimina un valor, ?<valor> busca un valor, m muestra el arbol, q sale.\nLos valores deben estar entre 1 y 99, plis.\n");

  while(1){

    printf("> ");

    char buffer[100];
    char c=0;
    int valor=0;

    while(!fgets(buffer, 100, stdin));

    sscanf(buffer, "%c%i", &c, &valor);
    c = (char)tolower(c);

    if(c == 'q')
      break;

    if(c == 'm')
      mostrar_abb(abb, formatear_int);
    else if(c && valor>0 && valor<100){

      if(c == '+'){
        int* i = malloc(sizeof(int));
        *i = valor;
        abb_insertar(abb, i);
      }
      else if (c == '-'){
        int* elemento = abb_quitar(abb, &valor);
        free(elemento);
      }
      else if(c == '?'){
        int* encontrado = abb_buscar(abb, &valor);
        if(encontrado)
          printf("Valor encontrado: %i\n", *encontrado);
        else printf("No encontré el valor D:\n");
      }
      else
        printf("Tenes que usar +<numero> o -<numero>");

      mostrar_abb(abb, formatear_int);
    }
  }

  abb_destruir_todo(abb, destruir_int);
}
