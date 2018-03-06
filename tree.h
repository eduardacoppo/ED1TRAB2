#ifndef TREE_H_
#define TREE_H_

#include "bitmap.h"

//Constantes criadas para deixar o código mais legível
#define true 1
#define false 0
typedef short int bool;

//Constantes criadas para deixar o código mais legível
#define _right 1
#define _left 0
typedef int Direction;

//Nós da árvore
typedef struct branch Branch;

//Retorna um Nó da árvore com os dados passados
/*
Inputs: Dados a ser armazenado no nó.
Outputs: Nó.
Pré-condições: Nenhuma.
Pos-Condições: Nó criado.
*/
Branch* tree_newBranch(void *data);

//Insere o Nó b na direção determinada no Nó a
/*
Inputs: Nó raiz, nó filho e a direção.
Outputs: True ou false para o sucesso da operação.
Pré-condições: Nós e direção validos.
Pos-Condições: Nó b adicionado como filho de a na direção especificada.
*/
bool tree_pushBranch(Branch *a, Branch *b, Direction direction);

//Retorna o Nó que contenha do dado especificado
/*
Inputs: Nó raiz, dados, e função de callback.
Outputs: Nó que possui os dados procurados.
Pré-condições: Raíz, dados e callback validos.
Pos-Condições: Nenhuma.
*/
Branch *tree_searchBranch(Branch *root, void *data, bool(*callback)(void*, void*));

//Retorna os dados de um Nó
/*
Inputs: Nó o qual se quer extrair os dados.
Outputs: Dados do nó.
Pré-condições: Nó válido.
Pos-Condições: Nenhuma.
*/
void* tree_getData(Branch *b);

//Insere os dados em um Nó
/*
Inputs: Nó e dados.
Outputs: Nenhum.
Pré-condições: Nó e dados válidos.
Pos-Condições: Nó passado possui os dados passados.
*/
void tree_setData(Branch *b, void *data);

//Retorna o Nó da direção especificada
/*
Inputs: Nó e direção.
Outputs: Nó que está na direção apontada.
Pré-condições: Nó e direção válidos.
Pos-Condições: Nenhuma.
*/
Branch *tree_walkTree(Branch *b, Direction d);

//Retorna o caminho da raiz até no Nó especificado em um bitmap
/*
Inputs: Nó que deseja-se obter o caminho.
Outputs: bitmap com o caminho da raiz ao nó.
Pré-condições: Nó valido.
Pos-Condições: nenhuma.
*/
bitmap tree_getWay(Branch *b);

//Diz se um Nó é folha
/*
Inputs: Nó.
Outputs: True ou false para caso o nó seja folha.
Pré-condições: Nó válido.
Pos-Condições: Nenhuma.
*/
bool isLeaf(Branch *auxBranch);

//Libera a árvore da memória
/*
Inputs: Raiz e função para liberar os dados do nó.
Outputs: Nenhum.
Pré-condições: Nó válido.
Pos-Condições: Árvore e dados liberados da memória.
*/
void tree_free(Branch *root, void(*callback)(void*));
#endif