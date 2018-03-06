#ifndef TREE_H_
#define TREE_H_

#include "bitmap.h"

//Constantes criadas para deixar o c�digo mais leg�vel
#define true 1
#define false 0
typedef short int bool;

//Constantes criadas para deixar o c�digo mais leg�vel
#define _right 1
#define _left 0
typedef int Direction;

//N�s da �rvore
typedef struct branch Branch;

//Retorna um N� da �rvore com os dados passados
/*
Inputs: Dados a ser armazenado no n�.
Outputs: N�.
Pr�-condi��es: Nenhuma.
Pos-Condi��es: N� criado.
*/
Branch* tree_newBranch(void *data);

//Insere o N� b na dire��o determinada no N� a
/*
Inputs: N� raiz, n� filho e a dire��o.
Outputs: True ou false para o sucesso da opera��o.
Pr�-condi��es: N�s e dire��o validos.
Pos-Condi��es: N� b adicionado como filho de a na dire��o especificada.
*/
bool tree_pushBranch(Branch *a, Branch *b, Direction direction);

//Retorna o N� que contenha do dado especificado
/*
Inputs: N� raiz, dados, e fun��o de callback.
Outputs: N� que possui os dados procurados.
Pr�-condi��es: Ra�z, dados e callback validos.
Pos-Condi��es: Nenhuma.
*/
Branch *tree_searchBranch(Branch *root, void *data, bool(*callback)(void*, void*));

//Retorna os dados de um N�
/*
Inputs: N� o qual se quer extrair os dados.
Outputs: Dados do n�.
Pr�-condi��es: N� v�lido.
Pos-Condi��es: Nenhuma.
*/
void* tree_getData(Branch *b);

//Insere os dados em um N�
/*
Inputs: N� e dados.
Outputs: Nenhum.
Pr�-condi��es: N� e dados v�lidos.
Pos-Condi��es: N� passado possui os dados passados.
*/
void tree_setData(Branch *b, void *data);

//Retorna o N� da dire��o especificada
/*
Inputs: N� e dire��o.
Outputs: N� que est� na dire��o apontada.
Pr�-condi��es: N� e dire��o v�lidos.
Pos-Condi��es: Nenhuma.
*/
Branch *tree_walkTree(Branch *b, Direction d);

//Retorna o caminho da raiz at� no N� especificado em um bitmap
/*
Inputs: N� que deseja-se obter o caminho.
Outputs: bitmap com o caminho da raiz ao n�.
Pr�-condi��es: N� valido.
Pos-Condi��es: nenhuma.
*/
bitmap tree_getWay(Branch *b);

//Diz se um N� � folha
/*
Inputs: N�.
Outputs: True ou false para caso o n� seja folha.
Pr�-condi��es: N� v�lido.
Pos-Condi��es: Nenhuma.
*/
bool isLeaf(Branch *auxBranch);

//Libera a �rvore da mem�ria
/*
Inputs: Raiz e fun��o para liberar os dados do n�.
Outputs: Nenhum.
Pr�-condi��es: N� v�lido.
Pos-Condi��es: �rvore e dados liberados da mem�ria.
*/
void tree_free(Branch *root, void(*callback)(void*));
#endif