//Se retornar -1 não está no array. Se estiver entrega a posição no array
int check_val(int x, int y, vertice val[], int size){

  for(int i = 1; i <= size; i++){
    if(val[i].x == x && val[i].y == y){
      return i;
    }
  }
  return -1;
}



int retangles_seen(State *estado, int vertice, int total_retangulos){
  int flag = -1;

  for(int i = 1; i <= total_retangulos; i++){
    if(estado->val[vertice].id_ret[i] == 1){ //se o retangulo i pertence ao vertice
      estado->ret_visited[i] = 1;
      flag = 1; //uso a flag para dizer se é para vigiar ou não
    }
  }
  return flag;
}



void update_state(State *estado, int total_vertices, int total_retangulos){

   for(int i = 1; i <= total_vertices; i++){
    	for(int x = 1; x <= total_retangulos; x++){
	      	//se o retângulo já está vigiado, retiramos um aos vértices que o vigiam
	      	//e retiramo-lo da "jurisdição" do vértice i
	      	if((estado->ret_visited[x] == 1) && (estado->val[i].id_ret[x] == 1)){
				estado->val[i].id_ret[x] = 0;
				estado->val[i].num_retangulos--; 
	      	} 
	    }
  	}
}


//Se todos os retangulos estiverem vigiados retorna 1
//Caso contrário retorna 0
int check_solution(State estado, int total_retangulos){
  for(int i = 1; i <= total_retangulos; i++){
    if(estado.ret_visited[i] == 0){
      return -1;
    }
  }

  return 1;
}

void print_state(State solution, int total_vertices){

  for(int i = 1; i <= total_vertices; i++){
    if(solution.val[i].vigia == 1) printf("(%d,%d)\n", solution.val[i].x, solution.val[i].y);
  }
   
}

//Depois de termos todos os casos de estados possiveis gerados vamos procurar a solução perfeita
void find_sol(LinkedList *list, int total_retangulos, int total_vertices){

  struct NODE *current = list->header;
  State best_so_far;
  int best_vert = total_vertices;
  int aux = 0;
  int max_limite = (total_retangulos/3) + 1; 
  
  while(current != NULL){
    if(check_solution(current->estado, total_retangulos) == 1){
      for(int i = 1; i <= total_vertices; i++){
	if(current->estado.val[i].vigia == 1) aux++; 
      }

      if(aux < best_vert){
	best_so_far = current->estado;
	best_vert = aux;
	
	if(aux == max_limite){
	  printf("%d\n", best_vert);
	  print_state(best_so_far, total_vertices);
	  break;
	}
	
	aux = 0; 
      }
    }
    
    current = current->next;
  }

  printf("%d\n", best_vert);
  print_state(best_so_far, total_vertices);
}

/*Procura todos os casos possiveis de vizinhos do estado atual e adiciona a lista caso sejam diferentes do que ja la estao
 *Flag 0 = BFS
 *Flag 1 = DFS
 */
void neighbours(State estado, int total_retangulos, int total_vertices, LinkedList *lista, LinkedList *visitados,  int flag){

  State child;

    for(int i = 1; i <= total_vertices; i++){
      if(estado.val[i].vigia == 0){ //se o vértice ainda nao estiver ativo
	
	child = state_copy(estado, total_retangulos, total_vertices);
	child.val[i].vigia = 1;
	child.depth = (estado.depth+1);
	
	int is_valid = retangles_seen(&child, i, total_retangulos);
	
	if(is_valid == 1) update_state(&child, total_vertices, total_retangulos);
	
	//Verifica se o child já esta nas duas linked list
	if(check(child, total_retangulos, total_vertices, lista) == -1 && check(child, total_retangulos, total_vertices, visitados) == -1 ){
	  if(flag == 0){
	    addlast(child, total_retangulos, total_vertices, lista);
	  }else{
	    addfirst(child, total_retangulos, total_vertices, lista);
	  }
	}  
      }
    }
  
}


