#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures/heap.c"
#include "structures/huffnode.c"
#include "structures/hashtable.c"

#ifndef DEBUG
#define DEBUG if (0)
#endif

typedef unsigned short uint16_t; // hã. talvez mudar isso ai

long getFrequency(heap_t *heap, int index) {
    return ((huffnode_t *)peek(heap, index))->frequency;
}

void buildBytes(huffnode_t *tree, int current, int treeLevel, hashtable_t *map) {
	if (isLeaf(tree)) {
		DEBUG printf("LEAF: %c [%d] \t%d\n", tree->byte, current, treeLevel);

		uint16_t *currentByte = (uint16_t *)malloc(2 * sizeof(uint16_t));
		currentByte[0] = treeLevel;
		currentByte[1] = current;

		put(map, (void *)currentByte, (unsigned)tree->byte);

		return;
	}

	buildBytes(tree->left, current << 1, treeLevel + 1, map);
	buildBytes(tree->right, (current << 1) | 1, treeLevel + 1, map);
}

int compress(char *fileName) {
	FILE *file = fopen(fileName, "rb");

	if (file == NULL) {
		fprintf(stderr, "file not found. terminating application\n");
		return 1;
	}

	char outputName[strlen(fileName) + 6];
	strcpy(outputName, fileName);
	strcat(outputName, ".huff");

	FILE *output = fopen(outputName, "wb");

	heap_t *heap = createHeap(256, getFrequency);
	long occurrences[256] = { 0 };

	byte_t currentByte;
	while (fscanf(file, "%c", &currentByte) != EOF) {
		occurrences[currentByte]++;
	}

	for (int i = 0; i < 256; i++) {
		if (occurrences[i]) {
			huffnode_t *node = createHuffnode((byte_t)i, occurrences[i], NULL, NULL);
			enqueue(heap, node);
		}
	}
	
	buildMinHeap(heap);

	DEBUG printf("A heap tem %d elementos\n", getHeapSize(heap));

	while (getHeapSize(heap) > 1) {
		huffnode_t *node1 = (huffnode_t *)dequeue(heap);
		huffnode_t *node2 = (huffnode_t *)dequeue(heap);
		huffnode_t *merged = merge(node1, node2);

		enqueue(heap, merged);

		DEBUG printf("Merged %X [%ld] with %X [%ld] --> %c [%ld]\n", node1->byte, node1->frequency, node2->byte, node2->frequency, merged->byte, merged->frequency);
	}

	DEBUG printf("A heap tem %d elementos\n", getHeapSize(heap));

	huffnode_t *tree = (huffnode_t *)dequeue(heap);
	int treeSize = 1 + getTreeSize(tree); // TO DO: arrumar a funcao pra n precisar somar 1 sempre

	DEBUG {
		printf("---- INICIO ARVORE ----\n");
		printTreePreorder(tree);
		printf("\n---- FIM DA ARVORE----\n >> %d elementos na arvore\n", treeSize);
	}

	hashtable_t *byteTable = createHashtable(256);
	buildBytes(tree, 0, 0, byteTable);

	rewind(file);

	byte_t header[2];
	header[1] = (byte_t)treeSize;

	fprintf(output, "?%c", header[1]);
	writeTreeIntoFile(output, tree);

	byte_t compressedByte = 0;
	int trashSize = 0;
	int cont = 0;

	while (fscanf(file, "%c", &currentByte) != EOF) {
		uint16_t *data = (uint16_t *)get(byteTable, currentByte);
		int numBits = (int)data[0];
		uint16_t bits = data[1];

		trashSize = 16 - numBits;

		for (int i = numBits - 1; i >= 0; i--) {
			compressedByte |= (bits & (1 << i)) > 0; // (bits & (1 << i)) >> i;

			if (cont == 7) {
				fprintf(output, "%c", compressedByte);
				compressedByte = 0;
				cont = -1;
			}

			compressedByte <<= 1;
			cont++;
		}
	}

	trashSize = trashSize % 8;
	compressedByte <<= trashSize;
	fprintf(output, "%c", compressedByte);

	DEBUG {
		printf("TAMANHO DO LIXO: %d\n", trashSize);
		printf("TAMANHO DA ARVORE: %d\n", treeSize);
	}

	header[0] = trashSize << 5;
	header[0] += (byte_t)(treeSize >> 8);

	fseek(output, 0, SEEK_SET);
	fwrite(&header[0], sizeof(byte_t), 1, output);

	fclose(file);
	fclose(output);

	return 0;
}