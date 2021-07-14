#include <stdio.h>
#include <stdlib.h>
#include "heapMax.h"

typedef struct{
  int x;
  int y;
  int num_retangulos; //Guarda o total de retangulos pertencentes ao vértice
  int *id_ret; //Guarda os retangulos pertencentes ao vértice
}vertice;

int check(int x, int y, vertice val[], int size);
void copy_array (vertice val[], int array[], int size); //copiar o id do array de vértices para um array de ints para construir a heap
int check_array(int ret[], int total_retangulos); //verifica se os retângulos já estão todos a ser vigiados
int retangles_seen(vertice val[], int ret[], int vertice, int total_retangulos); 
void heap_update(HEAPMAX *h, vertice val[], int ret[], int num_vert, int total_retangulos);
void algoritmo_greedy(HEAPMAX *h, vertice val[], int ret[], int num_vert, int total_retangulos);


int main(){

  int instancias = 0;
  int id = 0;
  int total_retangulos = 0;
  int total_vertices = 0;
  int vertices = 0;
  int x = 0, y = 0;
  int num_vert = 0;
  int pos = 0;
  int num_vigiar = 0;

  scanf("%d", &instancias);

  for(int i = 0; i < instancias; i++){
    scanf("%d", &total_retangulos);

    int ret[total_retangulos+1]; //Array contendo os retangulos para marcar 1 se visto e 0 se não visto
    for(int x = 0; x <= total_retangulos; x++) ret[x] = 1;

    scanf("%d", &num_vigiar);
    for(int x = 0; x < num_vigiar; x++){
      scanf("%d", &vertices);
      ret[vertices] = 0;
    }

    vertice val[(total_retangulos)*2+3]; //Guardar os vértices  2*R+3 porque o numero máx de verices é 2*R+2 e +1 para começar no vértice 1
    
    for(int h = 1; h <= (total_retangulos)*2+2; h++){ //Inicializa o array a -1
      val[h].x = -1;
      val[h].y = -1;
      val[h].num_retangulos = -1;
      val[h].id_ret = (int *)malloc ((total_retangulos + 1) * sizeof(int)); 
      for(int d = 1; d <= (total_retangulos); d++){
	val[h].id_ret[d] = 0;
      } 
    }
    
    for(int j = 0; j < total_retangulos; j++){
      scanf("%d %d", &id, &vertices);

      
      for(int z = 0; z < vertices; z++){
	scanf("%d %d", &x, &y);
	
	if(ret[id] == 0){
	  if((pos = check(x,y,val,num_vert)) == -1){
	    num_vert++;
	    val[num_vert].x = x;
	    val[num_vert].y = y;
	    val[num_vert].num_retangulos = 1;
	    val[num_vert].id_ret[id] = 1;
	  }else{
            val[pos].num_retangulos++;
            val[pos].id_ret[id] = 1;
	  }
	}
      }
    }
    
    int val_heaps[num_vert+1]; //Guarda o numero de retangulos em cada vertice, vertice guardado pelo indice
    copy_array (val, val_heaps, num_vert);
    

    HEAPMAX *h = build_heap_max(val_heaps, num_vert);
    
    algoritmo_greedy(h, val, ret, num_vert, total_retangulos);
    destroy_heap(h);
    for(int h = 1; h <= (total_retangulos+1)*2; h++) free(val[h].id_ret);
    num_vert = 0;
  }
  
  return 0;
}


//Se retornar -1 não está no array. Se estiver entrega a posição no array
int check(int x, int y, vertice val[], int size){

  for(int i = 1; i <= size; i++){
    if(val[i].x == x && val[i].y == y){
      return i;
    }
  }
  return -1;
}

void copy_array (vertice val[], int array[], int num_vert){

  for(int i = 1; i <= num_vert; i++){
    array[i] = val[i].num_retangulos;
  }
  
}

/* Se o array estiver todo preenchido retorna 1
 * Caso contrário retorna -1
   verifica se os retângulos já estão todos a ser vigiados
 */
int check_array(int ret[], int total_retangulos){
  int result = 1;

  for(int i = 1; i <= total_retangulos; i++){
    if(ret[i] == 0){ 
      result = -1;
    }
  }
  
  return result;
}


int retangles_seen(vertice val[], int ret[], int vertice, int total_retangulos){
  int flag = -1;

  for(int i = 1; i <= total_retangulos; i++){
    if(val[vertice].id_ret[i] == 1){ //se o retangulo i pertence ao vertice e for suposto vigiar
      ret[i] = 1;
      flag = 1; //uso a flag para dizer se é para vigiar ou não
    }
  }
  return flag;
}


void heap_update(HEAPMAX *h, vertice val[], int ret[], int num_vert, int total_retangulos){

  for(int i = 1; i <= num_vert; i++){
    for(int x = 1; x <= total_retangulos; x++){  	
      //se o retângulo já está vigiado, retiramos um aos vértices que o vigiam
      //e retiramo-lo da "jurisdição" do vértice i	
      if(val[i].id_ret[x] == 1 && ret[x] == 1){
  		val[i].id_ret[x] = 0;
  		val[i].num_retangulos--; 
      }
    }
    
    increaseKey(i, val[i].num_retangulos, h);
  }

}


void algoritmo_greedy(HEAPMAX *h, vertice val[], int ret[], int num_vert, int total_retangulos){

  int vert = 0; //Vai conter o id do vertice tirado do array
  int total_vert = 0; //Vai conter o numero total de vértices usados
  
  while(check_array(ret, total_retangulos) == -1){
    vert = extractMax(h);

    int is_valid = retangles_seen(val, ret, vert, total_retangulos); //vê os retângulos que o vértice vê e se estão selecionados para vigia
    if (is_valid == 1){   
      heap_update(h, val, ret, num_vert, total_retangulos);
      printf("(%d,%d)  ", val[vert].x, val[vert].y);
      total_vert++;
    }
  }

  printf("Total verts = %d\n", total_vert);
}
