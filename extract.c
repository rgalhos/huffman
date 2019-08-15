#include <stdio.h>
#include <stdlib.h>
#include "structures/heap.c"
#include "structures/hashtable.c"
#include "structures/huffnode.c"

void extract(FILE *file) {
	byte_t header[2];

	int thrashSize = 0;
	int treeSize = 0;

	fseek(file, 0L, SEEK_END);
	int fileSize = ftell(file);
	rewind(file);

	fscanf(file, "%c%c", &header[0], &header[1]);

	thrashSize = header[0] >> 5;
	treeSize = (byte_t)(header[0] << 7) << 1;
	treeSize += header[1];

	fileSize -= 2 + treeSize;

	printf("TAMANHO DO ARQUIVO (sem cabeçalho e arvore): %d\n", fileSize);
	printf("TAMANHO DO LIXO %d\n", thrashSize);
	printf("TAMANHO ARVORE %d\n", treeSize);

	return 0;
}
