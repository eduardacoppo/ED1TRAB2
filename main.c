#include <stdio.h>
#include "compress.h"
#include <string.h>

int main(int argc, char *argv[]) {
	
	char function[20], fileName[50];

	if (argc < 3) {
		printf("\nA entrada precisa ser '[Descompacta/Compacta] [Nome do arquivo]'\n");
		scanf("%s %s", function, fileName);
	}
	else {
		strcpy(function, argv[1]);
		strcpy(fileName, argv[2]);
	}

	if (strcmp(function, "Compacta") == 0) {
		Compress(fileName);
	}
	else {
		if (strcmp(function, "Descompacta") == 0) {
			Descompress(fileName);
		}
	}
		return 0;
		
}

/*
Consertar o tamano máximo do bitmap (deve ser a altura da arvore)
Organizar o cod prototipando as fun��o no inicio do arquivo
Separar compactador de descompactador
*/