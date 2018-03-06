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
Outputs: -1 se a frequência do primeiro for maior, 0 caso contrário.
Pré-condições: O branches existam.
Pos-Condições: Nenhuma.
*/
int compare(void* i1, void* i2);

/*
Inputs: Dois branches.
Outputs: True se os dados são iguais, false caso contrário.
Pré-condições: Os branches existam.
Pos-Condições: Nenhuma.
*/
bool compareData(void *b1, void *b2);

/*
Inputs: Um branch.
Outputs: Nenhum.
Pré-Condições: O branch exista.
Pos-Condições: Dados liberados da memória.
*/
void freeBranchData(void *b);

/*
Inputs: Nome do arquivo a ser tratado.
Outputs: A extensão do arquivo.
Pré-condições: Filename válido.
Pos-Condições: Filename sem a extensão.
*/
char* extractFileType(char *fileName);

/*
Inputs: A estrutura CompressedFileData e o arquivo de entrada a ser compactado.
Outputs: Nenhum.
Pré-condições: O arquivo exista e CompressedFileData inicializado.
Pos-Condições: Vetor de frequência dentro de CFData preenchido.
*/
void frequencyCount(CompressedFileData* CFData, FILE *f);

/*
Inputs: A estrutura CompressedFileData.
Outputs: Nenhum.
Pré-condições: O vetor de frequências preenchido.
Pos-Condições: A lista de galhos dentro de CFData preenchida com os bytes que contém frequências maiores que 0.
*/
void PreCreateDecoderTree(CompressedFileData *CFData);

/*
Inputs: A estrutura CompressedFileData.
Outputs: Nenhum.
Pré-Condições: BranchList criada.
Pos-Condições: Árvore ótima de Huffman preenchida no CFData.
*/
void CreateDecoderTree(CompressedFileData *CFData);

/*
Inputs: A estrutura CompressedFileData e o arquivo de saída aberto.
Outputs: Nenhum.
Pré-Condições: BranchList criada.
Pos-Condições: Informações da BranchList escrita no arquivo.
*/
void serializeList(CompressedFileData *CFData, FILE *fc);

/*
Inputs: A estrutura CompressedFileData.
Outputs: O vetor de bitmap.
Pré-condições: Árvore criada.
Pos-Condições: Vetor com a tabela de codificação retornado.
*/
bitmap *createByteTable(CompressedFileData *CFData);

/*
Inputs: A estrutura CompressedFileData e o vetor de bitmap.
Outputs: Nenhum.
Pré-condições: O vetor de bitmap exista e tenha dados.
Pos-Condições: Vetor de bitmap liberado da memória.
*/
void freeByteTable(bitmap *table, CompressedFileData *CFData);

/*
Inputs: A estrutura CompressedFileData, o arquivo de entrada, o arquivo temporário de saída e a tabela de codificação.
Outputs: Nenhum.
Pré-condições: Arquivos existam e tabela válida.
Pos-Condições: Arquivo compactado.
*/
void code(CompressedFileData *CFData, FILE *f, FILE *fc, bitmap* table);

/*
Inputs: A estrutura CompressedFileData e o arquivo de entrada.
Outputs: Nenhum.
Pré-condições: Arquivo compactado válido.
Pos-Condições: Informações do CFData preenchidas.
*/
void ProcessHeader(CompressedFileData *CFData, FILE *f);

/*
Inputs: A estrutura CompressedFileData, o arquivo de entrada compactado e o arquivo de saída.
Outputs: Nenhum.
Pré-condições: Arquivos abertos e CFData preenchido.
Pos-Condições: Arquivo de saída descompactado.
*/
void Decode(CompressedFileData *CFData, FILE *f, FILE *fd);

/*
Inputs: Nome do arquivo a ser descompactado.
Outputs: Nenhum.
Pré-condições: O arquivo exista.
Pos-Condições: Arquivo descompactado.
*/
void Descompress(char *fileName);

/*
Inputs : Nome do arquivo a ser compactado.
Outputs : True se o arquivo foi compactado, false se não foi.
Pré - condições : O arquivo exista.
Pos - Condições : Se executada com sucesso, o arquivo devidamente compactado
*/
bool Compress(char *fileName);

#endif
