#ifndef COMPRESS_H_
#define COMPRESS_H_

#define true 1
#define false 0
typedef short int bool;

typedef struct compressedFileData CompressedFileData;
typedef struct branchData BranchData;

#include "bitmap.h"
/*
Inputs: Dois branches.
Outputs: -1 se a frequ�ncia do primeiro for maior, 0 caso contr�rio.
Pr�-condi��es: O branches existam.
Pos-Condi��es: Nenhuma.
*/
int compare(void* i1, void* i2);

/*
Inputs: Dois branches.
Outputs: True se os dados s�o iguais, false caso contr�rio.
Pr�-condi��es: Os branches existam.
Pos-Condi��es: Nenhuma.
*/
bool compareData(void *b1, void *b2);

/*
Inputs: Um branch.
Outputs: Nenhum.
Pr�-Condi��es: O branch exista.
Pos-Condi��es: Dados liberados da mem�ria.
*/
void freeBranchData(void *b);

/*
Inputs: Nome do arquivo a ser tratado.
Outputs: A extens�o do arquivo.
Pr�-condi��es: Filename v�lido.
Pos-Condi��es: Filename sem a extens�o.
*/
char* extractFileType(char *fileName);

/*
Inputs: A estrutura CompressedFileData e o arquivo de entrada a ser compactado.
Outputs: Nenhum.
Pr�-condi��es: O arquivo exista e CompressedFileData inicializado.
Pos-Condi��es: Vetor de frequ�ncia dentro de CFData preenchido.
*/
void frequencyCount(CompressedFileData* CFData, FILE *f);

/*
Inputs: A estrutura CompressedFileData.
Outputs: Nenhum.
Pr�-condi��es: O vetor de frequ�ncias preenchido.
Pos-Condi��es: A lista de galhos dentro de CFData preenchida com os bytes que cont�m frequ�ncias maiores que 0.
*/
void PreCreateDecoderTree(CompressedFileData *CFData);

/*
Inputs: A estrutura CompressedFileData.
Outputs: Nenhum.
Pr�-Condi��es: BranchList criada.
Pos-Condi��es: �rvore �tima de Huffman preenchida no CFData.
*/
void CreateDecoderTree(CompressedFileData *CFData);

/*
Inputs: A estrutura CompressedFileData e o arquivo de sa�da aberto.
Outputs: Nenhum.
Pr�-Condi��es: BranchList criada.
Pos-Condi��es: Informa��es da BranchList escrita no arquivo.
*/
void serializeList(CompressedFileData *CFData, FILE *fc);

/*
Inputs: A estrutura CompressedFileData.
Outputs: O vetor de bitmap.
Pr�-condi��es: �rvore criada.
Pos-Condi��es: Vetor com a tabela de codifica��o retornado.
*/
bitmap *createByteTable(CompressedFileData *CFData);

/*
Inputs: A estrutura CompressedFileData e o vetor de bitmap.
Outputs: Nenhum.
Pr�-condi��es: O vetor de bitmap exista e tenha dados.
Pos-Condi��es: Vetor de bitmap liberado da mem�ria.
*/
void freeByteTable(bitmap *table, CompressedFileData *CFData);

/*
Inputs: A estrutura CompressedFileData, o arquivo de entrada, o arquivo tempor�rio de sa�da e a tabela de codifica��o.
Outputs: Nenhum.
Pr�-condi��es: Arquivos existam e tabela v�lida.
Pos-Condi��es: Arquivo compactado.
*/
void code(CompressedFileData *CFData, FILE *f, FILE *fc, bitmap* table);

/*
Inputs: A estrutura CompressedFileData e o arquivo de entrada.
Outputs: Nenhum.
Pr�-condi��es: Arquivo compactado v�lido.
Pos-Condi��es: Informa��es do CFData preenchidas.
*/
void ProcessHeader(CompressedFileData *CFData, FILE *f);

/*
Inputs: A estrutura CompressedFileData, o arquivo de entrada compactado e o arquivo de sa�da.
Outputs: Nenhum.
Pr�-condi��es: Arquivos abertos e CFData preenchido.
Pos-Condi��es: Arquivo de sa�da descompactado.
*/
void Decode(CompressedFileData *CFData, FILE *f, FILE *fd);

/*
Inputs: Nome do arquivo a ser descompactado.
Outputs: Nenhum.
Pr�-condi��es: O arquivo exista.
Pos-Condi��es: Arquivo descompactado.
*/
void Descompress(char *fileName);

/*
Inputs : Nome do arquivo a ser compactado.
Outputs : True se o arquivo foi compactado, false se n�o foi.
Pr� - condi��es : O arquivo exista.
Pos - Condi��es : Se executada com sucesso, o arquivo devidamente compactado
*/
bool Compress(char *fileName);

#endif
