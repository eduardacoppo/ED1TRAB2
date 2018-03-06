#include <stdlib.h>
#include <stdio.h>
#include "compress.h"
#include "tree.h"
#include "list.h"
#include "bitmap.h"
#include <string.h>

//Struct com a �rvore e outras informa��es usadas por v�rias fun��es do compactador
//frequencyArray: vetor usado para contar a frequ�ncia dos bytes no arquivo original
//dataLength:tamanho dos dados comprimidos em bit, esse tamanho � usado pelo descompactador para
//saber quantos bits representam o arquivo e desconsiderar o lixo que o s.o. insere no final do arquivo
//tree: �rvore de codifica��o e decodifica��o
//branchList: lista de galhos com as frequ�ncias e o byte representado para montar a arvore
struct compressedFileData{
	unsigned int frequencyArray[256], dataLength;
	Branch *tree;
	List *branchList;
	char *fileType;
};

//Struct para a vari�vel data dos n�s da �rvore
struct branchData{
	unsigned char letter;
	short int frequency;
	bool haveByte;
};

//fun��o de callback para list_order 
int compare(void* i1, void* i2) {
	BranchData *bd1 = tree_getData(i1);
	BranchData *bd2 = tree_getData(i2);

	if (bd1->frequency > bd2->frequency) {
		return -1;
	}
	return 0;
}

//Fun��o de callback para compara dados de uma branchData
bool compareData(void *b1, void *b2) {
	if ((b1 == NULL) || (b2 == NULL)) {
		return false;
	}
	BranchData *bd1, *bd2;
	bd1 = b1;
	bd2 = b2;
	if ((bd1->haveByte == true) && (bd2->haveByte == true)) {
		if (bd1->letter == bd2->letter)
			return true;
	}
	return false;
}

//Fun��o de callback para tree_free
void freeBranchData(void *b) {
	BranchData *_b;
	_b = b;
	free(_b);
}

//Fun��o para retirar a extens�o do arquivo e a retorna
char* extractFileType(char *fileName) {
	char *fileType;

	//Identificando onde come�a tipo do arquivo
	int i,lastDotPosition,j;
	for (i = 0; fileName[i] != '\0'; i++) {
		if (fileName[i] == '.') {
			lastDotPosition = i;
		}
	}
	//armazenando tamanho para o tipo do arquivo
	fileType = malloc(sizeof(char)*(strlen(fileName) - lastDotPosition + 2));
	//Copiando a extens�o do arquivo
	for (i = lastDotPosition, j = 0; fileName[i] != '\0'; i++, j++) {
		fileType[j] = fileName[i];
	}
	//ADD marca de fim da string
	fileType[j] = '\0';

	//removendo a extens�o do arquivo
	fileName[lastDotPosition] = '\0';

	return fileType;
}

//Fun��o respons�vel por contar as frequ�ncias dos bytes dentro do arquivo a ser compactado
void frequencyCount(CompressedFileData* CFData, FILE *f) {
	int i;
	unsigned char bit;

	//Zerando o vetor de frequ�ncia
	for (i = 0; i < 256; i++) {
		CFData->frequencyArray[i] = 0;
	}

	//Contando as frequ�ncias como demonstrado em sala da aula
	while (true) {
		fread(&bit, sizeof(char), 1, f);
		if (feof(f)) {
			break;
		}
		CFData->frequencyArray[bit]++;
	}
}


//Fun��o para prepara os dados para a fun��o que criar a arvore de decodifica��o,
//essa fun��o transforma o vetor de frequ�ncia em uma lista de n�s �rvore e o ordena 
//em ordem crescente
void PreCreateDecoderTree(CompressedFileData *CFData) {

	int i;

	//vari�veis auxiliares
	BranchData *bd;
	CFData->branchList = list_new();

	//Tranformando as frequ�ncias em ra�zes
	for (i = 0; i < 256; i++) {
		if (CFData->frequencyArray[i] > 0) {
			bd = malloc(sizeof(BranchData));
			bd->frequency = CFData->frequencyArray[i];
			bd->letter = i;
			bd->haveByte = true;
			list_pushOnLast(CFData->branchList,list_newItem(tree_newBranch(bd)));
		}
	}

	//ordenando a lista em ordem crescente
	list_order(CFData->branchList,compare);
}

//Fun��o que cria a �rvore �tima
void CreateDecoderTree(CompressedFileData *CFData) {
	Branch *b1, *b2, *bf;
	BranchData *BDaux, *BDaux1, *BDaux2;

	while (list_countItems(CFData->branchList) > 1) {
		//Retirando os dois primeiros sem reposi��o
		b1 = list_pull(CFData->branchList, 0);
		b2 = list_pull(CFData->branchList, 0);

		//Recuperando os dados para acessar os pesos
		BDaux1 = tree_getData(b1);
		BDaux2 = tree_getData(b2);

		BDaux = malloc(sizeof(BranchData));
		BDaux->letter = 0;
		BDaux->haveByte = false;
		BDaux->frequency = BDaux1->frequency + BDaux2->frequency;
		bf = tree_newBranch(BDaux);
		
		tree_pushBranch(bf, b1, _left);
		tree_pushBranch(bf, b2, _right);

		list_pushOnLast(CFData->branchList, list_newItem(bf));
		
		//ordenando a lista em ordem crescente
		list_order(CFData->branchList, compare);
	}
	CFData->tree = list_pull(CFData->branchList, 0);

	list_free(CFData->branchList, NULL);
}

//Fun��o respons�vel por gravar a lista de frequ�ncias no cabe�alho do arquivo
//essa lista de frenqu�ncia ser� usada posteriomente para descompactar o arquivo
void serializeList(CompressedFileData *CFData, FILE *fc) {
	
	short int nItens = list_countItems(CFData->branchList);
	int i;
	Branch *b;
	BranchData *bd;
	short int frequency;
	unsigned char byte;

	fwrite(&nItens, sizeof(short int), 1, fc);

	//� gravado somente os dados importantes
	for (i = 0; i < nItens; i++) {
		b = list_pullWithoutRemove(CFData->branchList, i);
		bd = tree_getData(b);
		frequency = bd->frequency;
		byte = bd->letter;
		fwrite(&frequency, sizeof(short int), 1, fc);
		fwrite(&byte, sizeof(unsigned char), 1, fc);
	}
}


//Fun��o que criar a tabela de codifica��o
bitmap *createByteTable(CompressedFileData *CFData) {
	bitmap *bmVet;
	int i;
	BranchData *bd;

	bmVet = malloc(sizeof(bitmap) * 256);
	bd = malloc(sizeof(BranchData));
	bd->haveByte = true;

	//O byte � o index do vetor e o caminho � o bitmap armazenado no mesmo
	for (i = 0; i < 256; i++) {
		if (CFData->frequencyArray[i] != 0) {
			bd->letter = i;
			bmVet[i] = tree_getWay(tree_searchBranch(CFData->tree, bd, compareData));
		}
	}
	free(bd);
	return bmVet;
}

//Fun��o que libera a tabela de codifica��o da mem�ria
void freeByteTable(bitmap *table, CompressedFileData *CFData) {
	int i;
	for (i = 0; i < 256; i++) {
		if (CFData->frequencyArray[i] != 0) {
			free(table[i].contents);
		}
	}
	free(table);
}

//Fun��o respons�vel por compactar o arquivo
void code(CompressedFileData *CFData, FILE *f, FILE *fc, bitmap* table) {

	unsigned char byte, byte2;
	//bm2 � usada para armazenar a representa��o do byte na �rvore (caminho) e bm1 �
	//usada como burffer, uma vez q o s.o. s� escreve de byte em byte
	bitmap bm1, bm2;
	int bm1Length,bm2Position,bm2Length;
	int i;

	bm1  = bitmapInit(8);
	CFData->dataLength = 0;

	while (!feof(f)) {
		//Recupera o tamanho do bitmap 1 para verificar se o mesmo n�o est� cheio
		bm1Length = bitmapGetLength(bm1);
		
		//Caso o bitmap 1 n�o esteja cheio, a fun��o recupera o proximo byte,
		//caso contrario ela esvazia-o
		while (bm1Length < 8) {

			//L� o byte do arquivo a ser compactado
			fread(&byte, sizeof(char), 1, f);
			//Verifica se o byte n�o � o final do arquivo
			if (feof(f)) {
				break;
			}
			//Recupera a representa��o do byte lido (caminho da raiz a folha)
			bm2 = table[byte];
			bm2Position = 0;
			//soma o tamanho do bm2 na dataLength (que representa o tamanho total dos dados
			//compactados)
			CFData->dataLength += bitmapGetLength(bm2);
			
			//Tranfere os dados do bitmap 2 para o 1
			for (i = 0; i < bitmapGetLength(bm2); i++) {
					bitmapAppendLeastSignificantBit(&bm1, bitmapGetBit(bm2, i));
					bm2Position++;
					//Quando o bitmap 1 atinge o tamanho de um byte, o loop � interrompido
					if (bitmapGetLength(bm1) > 7) {
						break;
					}
				}
				//Atualiza o bm1Length que � usado no inicio do loop para saber se bm1 esta cheio
				bm1Length = bitmapGetLength(bm1);
		}
		//recupera o byte que est� em bm1, lembrando que ele est� cheio (contem 8 bits)
		byte2 = bm1.contents[0];
		//escreve o byte de bm1
		fwrite(&byte2, sizeof(char), 1, fc);
		//reinicia o bitmap bm1
		free(bm1.contents);
		bm1 = bitmapInit(8);
		//caso ainda exista alguma informa��o em bm2, a mesma � tranferida pra bm1,
		bm2Length = bitmapGetLength(bm2);
		while (bm2Position < bm2Length) {
			for (i = bm2Position; i < bitmapGetLength(bm2); i++) {
				bitmapAppendLeastSignificantBit(&bm1, bitmapGetBit(bm2, i));
				bm2Position++;
				if (bitmapGetLength(bm1) > 7)
					break;
			}
			//Esvazia bm1 caso esteja cheio
			if (bitmapGetLength(bm1) > 7) {
				byte2 = bm1.contents[0];
				fwrite(&byte2, sizeof(char), 1, fc);
				free(bm1.contents);
				bm1 = bitmapInit(8);
			}
		}
	}
	free(bm1.contents);

	//unsigned char byte, byte2;
	////bm2 � usada para armazenar a representa��o do byte na arvore (caminho) e bm1 �
	////usada como burffer, uma vez q o s.o. s� escreve de byte em byte
	//bitmap bm1, bm2;
	//BranchData *bd;
	//int bm1Length, bm2Position, bm2Length;
	//int i;

	////Necessario para passar nos requisitos do compare data
	//bd = malloc(sizeof(BranchData));
	//bd->haveByte = true;

	//bm1 = bitmapInit(8);
	//CFData->dataLength = 0;

	//while (!feof(f)) {
	//	//Recupera o tamanho do bitmap 1 para verificar se o mesmo n esta cheio
	//	bm1Length = bitmapGetLength(bm1);

	//	//Caso o bitmap 1 n�o esteja cheio, a fun��o recupera o proximo byte,
	//	//caso contrario ele esvazia
	//	//o bitmap 1
	//	while (bm1Length < 8) {

	//		//Le o byte do arquivo a ser compactado
	//		fread(&byte, sizeof(char), 1, f);
	//		//Verifica se o byte n�o � o final do arquivo
	//		if (feof(f)) {
	//			break;
	//		}
	//		bd->letter = byte;
	//		//Recupera a representa��o do byte lido na arvore (caminho da raiz a folha)
	//		bm2 = tree_getWay(tree_searchBranch(CFData->tree, bd, compareData));
	//		bm2Position = 0;
	//		//soma o tamanho do bm2 na dataLength (que representa o tamanho total dos dados
	//		//compactados)
	//		CFData->dataLength += bitmapGetLength(bm2);

	//		//Tranfere os dados do bitmap 2 para o 1
	//		for (i = 0; i < bitmapGetLength(bm2); i++) {
	//			bitmapAppendLeastSignificantBit(&bm1, bitmapGetBit(bm2, i));
	//			bm2Position++;
	//			//Quando o bitmap 1 atinge o tamanho de um byte, o loop � interrompido
	//			if (bitmapGetLength(bm1) > 7) {
	//				break;
	//			}
	//		}
	//		//Caso o bm2 n�o tenha mais buffer ele � liberado
	//		if (bm2Position >= bitmapGetLength(bm2)) {
	//			free(bm2.contents);
	//		}
	//		//Atualiza o bm1Length que � usado no inicio do loop para saber se bm1 esta cheio
	//		bm1Length = bitmapGetLength(bm1);
	//	}
	//	//recupera o byte que esta em bm1, lembrando que ele esta cheio (contem 8 bits)
	//	byte2 = bm1.contents[0];
	//	//escreve o byte de bm1
	//	fwrite(&byte2, sizeof(char), 1, fc);
	//	//reinicia o bitmap bm1
	//	free(bm1.contents);
	//	bm1 = bitmapInit(8);
	//	//caso ainda exista alguma informa��o em bm2, a mesma � tranferida pra bm1,
	//	bm2Length = bitmapGetLength(bm2);
	//	while (bm2Position < bm2Length) {
	//		for (i = bm2Position; i < bitmapGetLength(bm2); i++) {
	//			bitmapAppendLeastSignificantBit(&bm1, bitmapGetBit(bm2, i));
	//			bm2Position++;
	//			if (bitmapGetLength(bm1) > 7)
	//				break;
	//		}
	//		//Exvazia bm1 caso estaja cheio
	//		if (bitmapGetLength(bm1) > 7) {
	//			byte2 = bm1.contents[0];
	//			fwrite(&byte2, sizeof(char), 1, fc);
	//			free(bm1.contents);
	//			bm1 = bitmapInit(8);
	//		}
	//		if (bm2Position >= bm2Length)
	//			free(bm2.contents);
	//	}
	//}
	//free(bm1.contents);
	//free(bd);
}

//Fun��o que coordena o processo de compactar
bool Compress(char *fileName) {

	CompressedFileData CFData;
	FILE *f, *fc, *fcTemp;
	char *fileCompressedName;
	char byte;
	bitmap *table;

	f = fopen(fileName, "rb");
	
	if (f == NULL) {
		return false;
	}

	CFData.fileType = extractFileType(fileName);

	fileCompressedName = malloc(sizeof(char)*(strlen(fileName) + 6));
	
	strcpy(fileCompressedName, fileName);
	strcat(fileCompressedName, ".comp");

	fc = fopen(fileCompressedName, "wb");

	//Armazena a extens�o do arquivo original no cabe�alho
	short int fileTypeLength = strlen(CFData.fileType) + 1;
	fwrite(&fileTypeLength, sizeof(short int), 1, fc);
	fwrite(CFData.fileType, sizeof(char), fileTypeLength, fc);

	frequencyCount(&CFData,f);

	PreCreateDecoderTree(&CFData);

	serializeList(&CFData, fc);

	CreateDecoderTree(&CFData);

	//Colocando o cursor do arquivo de entrada no in�cio novamente
	fseek(f, 0, SEEK_SET);
	
	//Cria um arquivo temporario para ser gravado o corpo do arquivo
	fcTemp = tmpfile();
	
	//Criando a tabela de bytes
	table = createByteTable(&CFData);
		
	//Iniciando a compacta��o
	code(&CFData, f, fcTemp, table);
	
	//Gravando o tamanho dos dados no arquivo de sa�da
	fwrite(&CFData.dataLength, sizeof(unsigned int), 1, fc);

	//Colocando o cusor de fcTemp no in�cio
	fseek(fcTemp, 0, SEEK_SET);

	//tranfere o conte�do do arquivo temporario para fc
	while (!feof(fcTemp)) {
		fread(&byte, sizeof(char), 1, fcTemp);
		fwrite(&byte, sizeof(char), 1, fc);
	}

	freeByteTable(table, &CFData);
	fclose(f);
	fclose(fc);
	fclose(fcTemp);
	free(fileCompressedName);
	tree_free(CFData.tree, freeBranchData);
	free(CFData.fileType);

	return true;
}

//Fun��o que l� o cabe�alho do arquivo comprimido � recria a branchList
void ProcessHeader(CompressedFileData *CFData, FILE *f) {
	int i;
	short int fileTypeLength = 0;
	short int numItens = 0;
	short int frequency;
	unsigned char byte;
	
	//L� o tipo do arquivo
	fread(&fileTypeLength, sizeof(short int), 1, f);
	CFData->fileType = malloc(sizeof(char)*(fileTypeLength));
	fread(CFData->fileType, sizeof(char), fileTypeLength, f);

	//L� as informa��es dos dataBranchs
	fread(&numItens, sizeof(short int), 1, f);

	CFData->branchList = list_new();
	BranchData *auxData;

	for (i = 0; i < numItens; i++) {
		auxData = malloc(sizeof(BranchData));
		fread(&frequency, sizeof(short int), 1, f);
		fread(&byte, sizeof(unsigned char), 1, f);
		auxData->frequency = frequency;
		auxData->letter = byte;
		auxData->haveByte = true;
		list_pushOnLast(CFData->branchList, list_newItem(tree_newBranch(auxData)));
	}

	//L� o tamanho do corpo do arquivo
	fread(&(CFData->dataLength), sizeof(unsigned int), 1, f);
}

//Fun��o que descompacta o arquivo
void Decode(CompressedFileData *CFData, FILE *f, FILE *fd) {
	
	Branch *auxBranch;
	BranchData *auxBranchData = NULL;
	bitmap bm;
	unsigned char byte,bit;
	int i, bitCount = 0;

	//Manipulando diretamente o bitmap, pois o mesmo n�o possui fun��o para isso
	bm.length = bm.max_size = 8;
	
	auxBranch = CFData->tree;

	//Loop ser� executado enquanto houver dados a ser lidos
	while (bitCount < CFData->dataLength) {
		//L� o proximo byte e verifica se � o final do arquivo
		if (feof(f))
			break;
		fread(&byte, sizeof(char), 1, f);
		
		//insere o byte no bitmap para poder manipula-lo bit a bit
		bm.contents = &byte;

		//loop que pecorre todo o bitmap
		for (i = 0; (i < 8) && (bitCount < CFData->dataLength); i++) {
			//recupera o proximo bit a ser lido e anda na �rvore
			bit = bitmapGetBit(bm, i);
			if (bit == 0) {
				auxBranch = tree_walkTree(auxBranch, _left);
			}
			else {
				auxBranch = tree_walkTree(auxBranch, _right);
			}
			bitCount++;
			//ap�s andar na �rvore, a fun��o verifica se o N� � uma folha, caso seja, ela imprime
			//o byte no arquivo descomprimido,
			//retorna o auxBranch para a raiz da arvore e continua o loop
			if (isLeaf(auxBranch)) {
				auxBranchData = tree_getData(auxBranch);
				fwrite(&(auxBranchData->letter), sizeof(char), 1, fd);
				auxBranch = CFData->tree;
			}
		}
	}
}

//Fun��o que coordena o processo de descompactar
void Descompress(char *fileName) {
	
	CompressedFileData CFData;
	FILE *f;
	FILE *fd;
	char *fileDescompressedName, *fileNameWithoutType, *fileType;

	fileNameWithoutType = malloc(sizeof(char)*(strlen(fileName)+1));
	strcpy(fileNameWithoutType, fileName);

	//Verifica se o arquivo � valido
	fileType = extractFileType(fileNameWithoutType);
	if (strcmp(fileType, ".comp")) {
		exit(EXIT_FAILURE);
	}
	free(fileType);

	f = fopen(fileName, "rb");

	if (f == NULL) {
		return;
	}

	ProcessHeader(&CFData ,f);

	fileDescompressedName = malloc(sizeof(char)*(strlen(fileNameWithoutType) + strlen(CFData.fileType) + 1));
	strcpy(fileDescompressedName, fileNameWithoutType);
	strcat(fileDescompressedName, CFData.fileType);

	fd = fopen(fileDescompressedName, "wb");

	if (fd == NULL) {
		return;
	}

	CreateDecoderTree(&CFData);

	Decode(&CFData, f, fd);

	fclose(f);
	fclose(fd);
	tree_free(CFData.tree, freeBranchData);
 	free(fileDescompressedName);
	free(fileNameWithoutType);
	free(CFData.fileType);
}