#ifndef LIST_H_
#define LIST_H_

/*
Essa lista é uma lista Genérica, duplamente encadeada e com sentinela
*/

//Constantes criadas para deixar o código mais legível
#define true 1
#define false 0
typedef short int bool;

//Sentinela da lista
typedef struct list List;

//Nó da lista
typedef struct item Item;

//Retorna um ponteiro sentinela inicializada
/*
Inputs: Nenhum.
Outputs: A sentinela da lista.
Pré-condições: Nenhuma.
Pos-Condições: Sentinela criada.
*/
List* list_new();

//Retorna um ponteiro para nó da lista com a data passada
/*
Inputs: Dados do nó.
Outputs: Nó da lista.
Pré-condições: Nenhuma.
Pos-Condições: Nó criado.
*/
Item* list_newItem(void *data);

//Insere um nó no inicio da lista
/*
Inputs: Lista e nó.
Outputs: Nenhum.
Pré-condições: Lista e nó válidos.
Pos-Condições: Nó inserido no inicio da lista.
*/
void list_pushOnFirst(List* list, Item* item);

//Insere um nó no final da lista
/*
Inputs: Lista e nó.
Outputs: Nenhum.
Pré-condições: Lista e nó válidos.
Pos-Condições: Nó inserido no final da lista.
*/
void list_pushOnLast(List * list, Item * item);

//Retorna o número de nós na lista
/*
Inputs: Lista.
Outputs: Números de nós na lista.
Pré-condições: Lista exista.
Pos-Condições: Nenhuma.
*/
int list_countItems(List * list);

//Função implementada parcialmente, pois não foi utilizada no trabalho
//int list_searchList(List * list, bool(*callback)(void *));

//Retira o Nó no index expecificado (liberando-o da memória) e retorna seus dados
/*
Inputs: Lista e a posição do nó.
Outputs: Os dados do nó.
Pré-condições: Lista existe e posição válida.
Pos-Condições: Nó retirado da lista.
*/
void* list_pull(List * list, int index);

//Retorna os dados de um Nó especifico sem remove-lo da lista
/*
Inputs: Lista e a posição do nó.
Outputs: Os dados do nó.
Pré-condições: Lista existe e posição válida.
Pos-Condições: Nenhuma.
*/
void* list_pullWithoutRemove(List* list, int index);

//Ordena a lista seguindo as orientações do callback
/*
Inputs: Lista e uma função de comparação.
Outputs: Nenhum.
Pré-condições: Lista e função validos.
Pos-Condições: Lista ordenada segundo o retorno da função.
*/
void list_order(List *list, int(*callback)(void*,void*));

//Imprime a lista
/*
Inputs: Lista e a função para imprimir os dados.
Outputs: Nenhum.
Pré-condições: Lista e função validos.
Pos-Condições: Dados dos nós da lista impressos na tela.
*/
void list_print(List* list, void(callback)(void*));

//Libera a lista da memória
/*
Inputs: Lista e função para liberar os dados da lista.
Outputs: Nenhuma.
Pré-condições: Lista e função validos.
Pos-Condições: Lista liberado da memória.
*/
void list_free(List *list, void(*callback)(void*));

#endif
