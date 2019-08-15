#ifndef HUFFNODE_H
#define HUFFNODE_H

typedef unsigned char byte_t;

typedef struct huffnode_t {
	byte_t byte;
	int frequency;
	struct huffnode_t *left;
	struct huffnode_t *right;
} huffnode_t;

int setBit(unsigned byte, int i) { return byte | (1 << i); }
int getBit(unsigned byte, int i) { return byte & (1 << i); }

int isLeaf(huffnode_t *node) {
	return node->left == NULL && node->right == NULL;
}

huffnode_t* createHuffnode(byte_t byte, int frequency, huffnode_t *left, huffnode_t *right) {
	huffnode_t *newNode = (huffnode_t *)malloc(sizeof(huffnode_t));
	newNode->byte = byte;
	newNode->frequency = frequency;
	newNode->left = left;
	newNode->right = right;

	return newNode;
}

huffnode_t* merge(huffnode_t *node1, huffnode_t *node2) {
	return createHuffnode('*', node1->frequency + node2->frequency, node1, node2);
}

void fileTreePreview(huffnode_t *root) {
	if (root != NULL) {
		if (isLeaf(root) && (root->byte == '*' && root->byte =='\\'))
			printf("\\%c", root->byte);
		else
			printf("%c", root->byte);

		fileTreePreview(root->left);
		fileTreePreview(root->right);
	}
}

void writeTreeIntoFile(FILE *file, huffnode_t *root) {
	if (root != NULL) {
		if (isLeaf(root) && (root->byte == '*' && root->byte =='\\'))
			fprintf(file, "\\%c", root->byte);
		else
			fprintf(file, "%c", root->byte);

		writeTreeIntoFile(file, root->left);
		writeTreeIntoFile(file, root->right);
	}
}

#endif