#include "mostrar_abb.h"
#include "src/abb.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int max(int a, int b){ return a>b?a:b; }

int abb_a_array_r(nodo_abb_t* abb, void** buffer, int n, int i){
  if(i>=n || !abb)
    return 0;

  buffer[i] = abb;
  int a = abb_a_array_r(abb->izquierda, buffer, n, 2*i+1);
  int b = abb_a_array_r(abb->derecha, buffer, n, 2*i+2);
  return max(max(a,b),i+1);
}

int abb_a_array(abb_t* abb, void** buffer, int n){
  if(!abb)
    return 0;
  return abb_a_array_r(abb->nodo_raiz, buffer, n, 0);
}

void mostrar_arbol(void** buffer, int n, void (*formateador)(void*)){
  printf("\n");
  int niveles = 0;
  for(int restantes=n;restantes>0;restantes>>=1)
    niveles++;

  int n_nodo=0;
  for(int nivel=0;nivel<niveles;nivel++){
    int posicion = 1<<(niveles-nivel-1);

    for(int nodo=0;nodo<(1<<nivel);nodo++){
      for(int i=1;i<posicion;i++){
        if(i<=posicion/2-1 || buffer[2*n_nodo+1]==0)
          printf("  ");
        else{
          if(i == posicion/2)
            printf(" ┌");
          else printf("――");
        }
      }

      if(buffer[n_nodo]!=0){
        formateador(buffer[n_nodo]);
      }
      else{
        printf("  ");
      }

      for(int i=0;i<posicion;i++){
        if(i>=posicion/2 || buffer[2*n_nodo+2]==0)
          printf("  ");
        else{
          if(i == posicion/2-1)
            printf("―┐");
          else printf("――");
        }
      }
      n_nodo++;
    }
    printf("\n");
  }
}


void  mostrar_abb(abb_t* abb, void (*formateador)(void*)){
    void* valores[MAX_ARBOL];

    memset(valores, 0, sizeof(valores));
    int cantidad = abb_a_array(abb, valores, MAX_ARBOL);
    mostrar_arbol(valores, cantidad, formateador);
}
