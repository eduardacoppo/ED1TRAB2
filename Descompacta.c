#include <stdio.h>
#include "compress.h"
#include <string.h>

int main(int argc, char *argv[]) {
	
	char fileName[50];

	if (argc < 2) {
		printf("\nA entrada precisa ser '[Nome do arquivo]'\n");
	}
	else {
		strcpy(fileName, argv[1]);
        Descompress(fileName);
	}
		return 0;
		
}