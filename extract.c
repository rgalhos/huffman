#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures/huffnode.c"

#ifndef DEBUG
#define DEBUG if (0)
#endif

huffnode_t* buildHuffTree(char *str, int *pos) {
	huffnode_t *node = createHuffnode(str[*pos], -1, NULL, NULL);

	(*pos)++;

	if (node->byte == '*') {
		node->left = buildHuffTree(str, pos);
		node->right = buildHuffTree(str, pos);
	} else if (node->byte == '\\') {
		node->byte = str[*pos];
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
	
	if (strstr(fileName, ".huff") == NULL) {
		printf("THAT'S NOT A .huff FILE WHAT AM I SUPOSSED TO DO? urrrgh I'M OUT\n");
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
	for (int j = fileSize; j > 1; j--) {
		fscanf(file, "%c", &currentByte);
		
		for (int j = 7; j >= 0; j--) {
			if (getBit(currentByte, j))
				node = node->right;
			else
				node = node->left;
			
			if (isLeaf(node)) {
				fprintf(output, "%c", (byte_t)node->byte);
				node = tree;
			}
		}
	}
	
	for (int i = 7; i >= trashSize; i--) {
		if (getBit(currentByte, i))
			node = node->right;
		else
			node = node->left;
		
		if (isLeaf(node)) {
			fprintf(output, "%c", (byte_t)node->byte);
		}
	}
	
	fclose(file);
	fclose(output);
	
	return 0;
}
