#include<stdio.h>
#include<stdlib.h>

typedef struct{
  int x;
  int y;
  int num_retangulos; //Guarda o total de retangulos pertencentes ao vértice
  int *id_ret; //Guarda os retangulos pertencentes ao vértice
  int vigia;
}vertice;

typedef struct{
  vertice *val;
  int depth;
  int *ret_visited;
}State;

struct NODE{
  State estado;
  struct NODE *next;
};

typedef struct {
    struct NODE *header;
    int total_size;
}LinkedList;

//Inicializar a lista vazia (Construtor)
LinkedList make(){
    LinkedList list;
    list.header = NULL;
    list.total_size = 0;
    return list;
}

//Fazer print da lista
void print(LinkedList list, int pos_inicial, int pos_final, int total_vertices, int total_retangulos){
 
  struct NODE *current = list.header;
  int i = 0;
  for(; i < pos_inicial; i++){
      current = current->next;
  }

  while((current != NULL) && (i <= pos_final)){
    printf("Depth: %d\n", current->estado.depth);
    
    for(int i = 1; i <= total_vertices; i++){
      printf("(%d,%d) Num: %d  Vigia: %d \n", current->estado.val[i].x, current->estado.val[i].y, current->estado.val[i].num_retangulos, current->estado.val[i].vigia);
      //for(int j = 1; j <= total_retangulos; j++) printf("   %d -> %d\n", j, current->estado.val[i].id_ret[j]);
    }
    
    
    i++;
    printf("------------------------------------\n");
    current = current->next;
  }
}


State state_copy(State estado, int num_retangulos, int num_vert){
  State returning;

  returning.val = (vertice *)malloc(sizeof(vertice) * (num_vert+1));
  returning.ret_visited = (int *)malloc(sizeof(int) * (num_retangulos+1));
  returning.depth = estado.depth;
    
  for(int i = 1; i <= num_retangulos; i++){
    returning.ret_visited[i] = estado.ret_visited[i];
  }
  
  for(int i = 1; i <= num_vert; i++){
    returning.val[i].x = estado.val[i].x;
    returning.val[i].y = estado.val[i].y;
    returning.val[i].vigia = estado.val[i].vigia;
    returning.val[i].num_retangulos = estado.val[i].num_retangulos;
    returning.val[i].id_ret = (int *)malloc ((num_retangulos + 1) * sizeof(int));
    
    for (int j = 1; j <= num_retangulos; ++j){
      returning.val[i].id_ret[j] = estado.val[i].id_ret[j];
    }
  }

  return returning;
}


//Adicionar valores
void addfirst(State atual, int total_retangulos, int total_vertices, LinkedList *list){

    struct NODE *new = (struct NODE*)malloc(sizeof(struct NODE));

    new->estado = state_copy(atual, total_retangulos, total_vertices);

    new->next = list->header;
    list->header = new;

    list->total_size++;
}

void addlast(State atual, int total_retangulos, int total_vertices, LinkedList *list){

    if(list->total_size == 0){
      addfirst(atual, total_retangulos, total_vertices, list);
      return;
    }

    struct NODE *current = list->header;

    while(current->next != NULL) current = current->next;

    struct NODE *new = (struct NODE*)malloc(sizeof(struct NODE));
    new->estado = state_copy(atual, total_retangulos, total_vertices);
   
    current->next = new;    
    new->next = NULL;

    list->total_size++;
}

State removefirst(LinkedList *list, int total_vertices){

  State estado;

  struct NODE *temp = list->header;
  list->header = temp->next;
  estado = temp->estado;

  list->total_size--;
  return estado;
}


/*Funcao auxiliar para check
  Se for igual retorna 1
  Caso não seja igual retorna -1
*/
int compare(State estado, int total_retangulos, int total_vertices,  struct NODE *current){
  for(int i = 1; i <= total_retangulos; i++){
    if((current->estado.ret_visited[i] != estado.ret_visited[i])){
      return -1;
    }
  }

  
  for(int i = 1; i <= total_vertices; i++){

    if((current->estado.val[i].x != estado.val[i].x) || (current->estado.val[i].y != estado.val[i].y) || (current->estado.val[i].num_retangulos != estado.val[i].num_retangulos) || (current->estado.val[i].vigia != estado.val[i].vigia)) return -1;
    
    for(int j = 1; j <= total_retangulos; j++){
      if(estado.val[i].id_ret[j] != current->estado.val[i].id_ret[j]){
	return -1;
      }
    }
  }
  
  return 1;
}

/* Verificar se o estado já pertence a lista
 * Se não pertencer retorna -1
 * Se pertencer retorna 1
 */
int check(State estado, int total_retangulos, int total_vertices, LinkedList *list){
  int result = -1;
  struct NODE *actual = list->header;

  while(actual != NULL){

    if(compare(estado, total_retangulos, total_vertices, actual) == 1){
      result = 1;
      return result;
    }
    
    actual = actual->next;
  }

  return result;
}


void destroy_LinkedList(LinkedList lista, int total_retangulos, int total_vertices){

  struct NODE *current = lista.header;
  struct NODE *temp;
  
  while(current != NULL){
    temp = current->next;

    for(int i = 1; i <= total_vertices; i++){
      free(current->estado.val[i].id_ret);
    }

    free(current->estado.ret_visited);
    free(current->estado.val);
    free(current);
    current = temp;
  }
  
}
