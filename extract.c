#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures/huffnode.c"

#ifndef DEBUG
#define DEBUG if (0)
#endif

huffnode_t* buildHuffTree(char *str, int *pos) {
	byte_t *byte = (byte_t *)malloc(sizeof(byte_t));
	*byte = str[*pos];

	huffnode_t *node = createHuffnode((void *)byte, -1, NULL, NULL);

	(*pos)++;

	if (*byte == '*') {
		node->left = buildHuffTree(str, pos);
		node->right = buildHuffTree(str, pos);
	} else if (*byte == '\\') {
		*byte = str[*pos];
		node->item = (void *)byte;

		(*pos)++;
	}

	return node;
}

int extract(char *fileName) {
	FILE *file = fopen(fileName, "rb");

	if (file == NULL) {
		fprintf(stderr, "file not found. terminating application\n");
		return 1;
	}

	char outputName[strlen(fileName)];
	strcpy(outputName, fileName);
	outputName[strlen(fileName) - 5] = '\0';

	FILE *output = fopen(outputName, "wb");

	int trashSize = 0;
	int treeSize = 0;
	int fileSize = 0;

	fseek(file, 0, SEEK_END);
	fileSize = ftell(file);
	rewind(file);

	byte_t header[2];
	fscanf(file, "%c%c", &header[0], &header[1]);
	
	trashSize = header[0] >> 5;
	treeSize = (byte_t)(header[0] << 3) * 32;
	treeSize += header[1];

	fileSize -= 2 + treeSize;
	
	DEBUG {
		printf("TAMANHO LIXO: %d\n", trashSize);
		printf("TAMANHO ARVORE: %d\n", treeSize);
		printf("TAMANHO ARQUIVO: %d\n", fileSize);
	}

	int treePos = 0;
	byte_t treeStr[treeSize];
	for (int i = 0; i < treeSize; i++) {
		fscanf(file, "%c", &treeStr[i]);
	}

	huffnode_t *tree = buildHuffTree(treeStr, &treePos);

	DEBUG {
		printf("---- INICIO DA ARVORE ----\n");
		printTreePreorder(tree);
		printf("\n---- FIM DA ARVORE ----\n");
	}

	byte_t currentByte;
	huffnode_t *node = tree;
	for (int i = fileSize; i > 1; i--) {
		fscanf(file, "%c", &currentByte);

		for (int j = 7; j >= 0; j--) {
			if (getBit(currentByte, j))
				node = node->right;
			else
				node = node->left;

			if (isLeaf(node)) {
				fprintf(output, "%c", *((byte_t *)node->item));
				node = tree;
			}
		}
	}

	fscanf(file, "%c", &currentByte);

	for (int i = 7; i >= trashSize; i--) {
		if (getBit(currentByte, i))
			node = node->right;
		else
			node = node->left;

		if (isLeaf(node)) {
			fprintf(output, "%c", *((byte_t *)node->item));
			node = tree;
		}
	}

	fclose(file);
	fclose(output);

	return 0;
}
