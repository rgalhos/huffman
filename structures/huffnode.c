#ifndef HUFFNODE_H
#define HUFFNODE_H

#include <stdint.h>

typedef uint8_t byte_t;
//typedef unsigned char byte_t;
//typedef unsigned short uint16_t;

typedef struct huffnode_t {
	void *item;
	long frequency;
	struct huffnode_t *left;
	struct huffnode_t *right;
} huffnode_t;

int setBit(unsigned byte, int i) { return byte | (1 << i); }
int getBit(unsigned byte, int i) { return byte & (1 << i); }

int isLeaf(huffnode_t *node) {
	return (node->left == NULL && node->right == NULL);
}

huffnode_t* createHuffnode(void *item, int frequency, huffnode_t *left, huffnode_t *right) {
	huffnode_t *newNode = (huffnode_t *)malloc(sizeof(huffnode_t));
	newNode->item = item;
	newNode->frequency = frequency;
	newNode->left = left;
	newNode->right = right;

	return newNode;
}

huffnode_t* merge(huffnode_t *node1, huffnode_t *node2) {
	int frequency2 = 0;

	if (node2 != NULL) {
		frequency2 = node2->frequency;
	}

	byte_t *byte = (byte_t *)malloc(sizeof(byte_t));
	*byte = '*';

	return createHuffnode((void *)byte, node1->frequency + frequency2, node1, node2);
}

void printTreePreorder(huffnode_t *root) {
	if (root != NULL) {
		if (isLeaf(root) && (*((byte_t *)root->item) == '*' || *((byte_t *)root->item) =='\\'))
			printf("\\");

		printf("%c", *((byte_t *)root->item));

		printTreePreorder(root->left);
		printTreePreorder(root->right);
	}
}

void writeTreeIntoFile(FILE *file, huffnode_t *root) {
	if (root != NULL) {
		if (isLeaf(root) && (*((byte_t *)root->item) == '*' || *((byte_t *)root->item) =='\\'))
			fprintf(file, "\\");

		fprintf(file, "%c", *((byte_t *)root->item));

		writeTreeIntoFile(file, root->left);
		writeTreeIntoFile(file, root->right);
	}
}

int getTreeSize(huffnode_t *tree) {
	if (tree == NULL)
		return 0;

	int c = (*((byte_t *)tree->item) == '*' || *((byte_t *)tree->item) == '\\') && isLeaf(tree);
	c += getTreeSize(tree->left);
	c += getTreeSize(tree->right);

	return 1 + c;
}

#endif
