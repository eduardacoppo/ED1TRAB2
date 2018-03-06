#ifndef LIST_H_
#define LIST_H_

/*
Essa lista � uma lista Gen�rica, duplamente encadeada e com sentinela
*/

//Constantes criadas para deixar o c�digo mais leg�vel
#define true 1
#define false 0
typedef short int bool;

//Sentinela da lista
typedef struct list List;

//N� da lista
typedef struct item Item;

//Retorna um ponteiro sentinela inicializada
/*
Inputs: Nenhum.
Outputs: A sentinela da lista.
Pr�-condi��es: Nenhuma.
Pos-Condi��es: Sentinela criada.
*/
List* list_new();

//Retorna um ponteiro para n� da lista com a data passada
/*
Inputs: Dados do n�.
Outputs: N� da lista.
Pr�-condi��es: Nenhuma.
Pos-Condi��es: N� criado.
*/
Item* list_newItem(void *data);

//Insere um n� no inicio da lista
/*
Inputs: Lista e n�.
Outputs: Nenhum.
Pr�-condi��es: Lista e n� v�lidos.
Pos-Condi��es: N� inserido no inicio da lista.
*/
void list_pushOnFirst(List* list, Item* item);

//Insere um n� no final da lista
/*
Inputs: Lista e n�.
Outputs: Nenhum.
Pr�-condi��es: Lista e n� v�lidos.
Pos-Condi��es: N� inserido no final da lista.
*/
void list_pushOnLast(List * list, Item * item);

//Retorna o n�mero de n�s na lista
/*
Inputs: Lista.
Outputs: N�meros de n�s na lista.
Pr�-condi��es: Lista exista.
Pos-Condi��es: Nenhuma.
*/
int list_countItems(List * list);

//Fun��o implementada parcialmente, pois n�o foi utilizada no trabalho
//int list_searchList(List * list, bool(*callback)(void *));

//Retira o N� no index expecificado (liberando-o da mem�ria) e retorna seus dados
/*
Inputs: Lista e a posi��o do n�.
Outputs: Os dados do n�.
Pr�-condi��es: Lista existe e posi��o v�lida.
Pos-Condi��es: N� retirado da lista.
*/
void* list_pull(List * list, int index);

//Retorna os dados de um N� especifico sem remove-lo da lista
/*
Inputs: Lista e a posi��o do n�.
Outputs: Os dados do n�.
Pr�-condi��es: Lista existe e posi��o v�lida.
Pos-Condi��es: Nenhuma.
*/
void* list_pullWithoutRemove(List* list, int index);

//Ordena a lista seguindo as orienta��es do callback
/*
Inputs: Lista e uma fun��o de compara��o.
Outputs: Nenhum.
Pr�-condi��es: Lista e fun��o validos.
Pos-Condi��es: Lista ordenada segundo o retorno da fun��o.
*/
void list_order(List *list, int(*callback)(void*,void*));

//Imprime a lista
/*
Inputs: Lista e a fun��o para imprimir os dados.
Outputs: Nenhum.
Pr�-condi��es: Lista e fun��o validos.
Pos-Condi��es: Dados dos n�s da lista impressos na tela.
*/
void list_print(List* list, void(callback)(void*));

//Libera a lista da mem�ria
/*
Inputs: Lista e fun��o para liberar os dados da lista.
Outputs: Nenhuma.
Pr�-condi��es: Lista e fun��o validos.
Pos-Condi��es: Lista liberado da mem�ria.
*/
void list_free(List *list, void(*callback)(void*));

#endif
