#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"
#include "Pesquisa.h"

//Faz a pesquisa por BFS
void bfs_search(State estado, int total_retangulos, int total_vertices){
  
    LinkedList lista = make();
    LinkedList visitados = make();
       
    addfirst(estado, total_retangulos,total_vertices, &lista);
     	  
    while(lista.total_size != 0){
      estado = removefirst(&lista, total_vertices);
      addlast(estado, total_retangulos, total_vertices, &visitados);

      neighbours(estado, total_retangulos, total_vertices, &lista, &visitados, 0);
    }

    find_sol(&visitados, total_retangulos, total_vertices);
    destroy_LinkedList(visitados, total_retangulos, total_vertices);
    destroy_LinkedList(lista, total_retangulos, total_vertices);
}


//Faz a pesquisa por DFS
void dfs_search(State estado, int total_retangulos, int total_vertices){
  
    LinkedList lista = make();
    LinkedList visitados = make();
       
    addfirst(estado, total_retangulos,total_vertices, &lista);
     	  
    while((lista.total_size != 0)){
      estado = removefirst(&lista, total_vertices);
      addfirst(estado, total_retangulos, total_vertices, &visitados);

      neighbours(estado, total_retangulos, total_vertices, &lista, &visitados, 1);
    }

    find_sol(&visitados, total_retangulos, total_vertices);
    destroy_LinkedList(visitados, total_retangulos, total_vertices);
    destroy_LinkedList(lista, total_retangulos, total_vertices);
}



///////////////////////////////// A_SEARCH ///////////////////////////////////////////////////
int rets_vigiados_novo_estado(State estado_novo, State estado_anterior, int size){
  int temp = 0;
  int temp1 = 0;
  for(int i = 1; i<=size; i++)
    temp += estado_anterior.ret_visited[i];

  for(int i = 1; i<=size; i++)
    temp1 += estado_novo.ret_visited[i];

  int returning = temp1 - temp;

  return returning;
}


void A_Search(State solution, int total_vertices, int total_retangulos, int rets_por_vigiar){

  State estado = state_copy(solution, total_retangulos, total_vertices);
  int h = 0;
  
  LinkedList visitados = make();
  LinkedList lista = make();

  addfirst(estado, total_retangulos,total_vertices, &lista);

  while(1){
    int best_so_far = 0;
    State helper;
    State temporary;

    //procurar estados a partir do estado atual
    estado = removefirst(&lista, total_vertices);
    addlast(estado, total_retangulos, total_vertices, &visitados);

    neighbours(estado, total_retangulos, total_vertices, &lista, &visitados, 0);

    while(lista.total_size != 0){

      helper = removefirst(&lista, total_vertices);

      h = rets_vigiados_novo_estado(helper, solution, total_retangulos); //quantidade de rets que passaram a ser vigiados no estado em analise

      //vê se o em análise é melhor que o melhor até agora
      if(h > best_so_far){
        best_so_far = h;
        temporary = state_copy(helper, total_retangulos, total_vertices);
      }
    }

     //atualizar o estado e numero de rets por vigiar
    rets_por_vigiar -= best_so_far;
    solution = state_copy(temporary, total_retangulos, total_vertices);

    //se ja nao houverem rets por vigiar, para se o ciclo
    if(rets_por_vigiar == 0)
      break;

    addlast(solution, total_retangulos, total_vertices, &lista);
  }

  print_state(solution, total_vertices);
  destroy_LinkedList(visitados, total_retangulos, total_vertices);
  destroy_LinkedList(lista, total_retangulos, total_vertices);
}

//////////////////////////////////////////////////////////////////////////////////

void BranchAndBound(State inicial, int total_vertices, int total_retangulos, int rets_por_vigiar){

  int bound = (rets_por_vigiar/3) + 1;
  
  LinkedList visitados = make();
  LinkedList lista = make();
  State estado;

  addfirst(inicial, total_retangulos,total_vertices, &lista);

  while((lista.total_size != 0)){
    estado = removefirst(&lista, total_vertices);
    addfirst(estado, total_retangulos, total_vertices, &visitados);

    if(estado.depth <= bound){
      if(check_solution(estado, total_retangulos) == 1){
	print_state(estado, total_vertices);
	break;
      }
      neighbours(estado, total_retangulos, total_vertices, &lista, &visitados, 1);
    }    
  }

  destroy_LinkedList(visitados, total_retangulos, total_vertices);
  destroy_LinkedList(lista, total_retangulos, total_vertices);
}


int main(){

  int instancias = 0;
  int id = 0;
  int total_retangulos = 0;
  int vertices = 0;
  int retangulos = 0;
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
      scanf("%d", &retangulos);
      ret[retangulos] = 0;
    }
    
    vertice val[(total_retangulos+1)*2+1]; //Guardar os vértices
    
    for(int h = 1; h <= (total_retangulos+1)*2; h++){ //Inicializa o array a -1 e 0
      val[h].x = -1;
      val[h].y = -1;
      val[h].num_retangulos = -1;
      val[h].vigia = 0;
      
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
	  if((pos = check_val(x,y,val,num_vert)) == -1){
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

    
    State atual;
    atual.val = (vertice *)malloc(sizeof(vertice) * (num_vert+1));
    atual.ret_visited = (int *)malloc(sizeof(int) * (total_retangulos+1));
    atual.depth = 0;

    for(int i = 1; i <= num_vert; i++){
      atual.val[i] = val[i];
    }

    for(int i = 1; i <= total_retangulos; i++){
      atual.ret_visited[i] = ret[i];
    }

    printf("BFS:\n");
    //bfs_search(atual, total_retangulos, num_vert);
    printf("\n-----------------------------\n");

    printf("DFS:\n");
    //dfs_search(atual, total_retangulos, num_vert);
    printf("\n-----------------------------\n");

    printf("A* Search:\n");
    //A_Search(atual, num_vert, total_retangulos, num_vigiar);
    printf("\n-----------------------------\n");

    printf("Branch and Bound:\n");
    //BranchAndBound(atual, num_vert, total_retangulos, num_vigiar);
    printf("\n-----------------------------\n");


    //Libertar memoria 
    for(int h = 1; h <= (total_retangulos+1)*2; h++){ 
      free(val[h].id_ret); 
    }
    
    free(atual.ret_visited);
    free(atual.val);
  }

  return 0;
}

