#include "../structures/huffnode.c"

#ifndef NOVOBYTE
#define NOVOBYTE

byte_t *novoByte(byte_t c) {
	byte_t *ptr = (byte_t *)malloc(sizeof(byte_t));
	*ptr = c;
	return ptr;
}

#endif

void test_createHuffnode() {
	huffnode_t *nodeA = createHuffnode((void *)novoByte('A'), 1, NULL, NULL);

	CU_ASSERT(*(byte_t *)nodeA->item == 'A');
	CU_ASSERT(nodeA->frequency == 1);
	CU_ASSERT_PTR_NULL(nodeA->left);
	CU_ASSERT_PTR_NULL(nodeA->right);

	huffnode_t *nodeB = createHuffnode((void *)novoByte('B'), 2, NULL, NULL);

	CU_ASSERT(*((byte_t *)nodeB->item) == 'B');
	CU_ASSERT(nodeB->frequency == 2);
	CU_ASSERT_PTR_NULL(nodeB->left);
	CU_ASSERT_PTR_NULL(nodeB->right);

	huffnode_t *nodeC = merge(nodeA, nodeB);

	CU_ASSERT(*((byte_t *)nodeC->item) == '*');
	CU_ASSERT(nodeC->frequency == 3);
	CU_ASSERT_PTR_EQUAL(nodeC->left, nodeA);
	CU_ASSERT_PTR_EQUAL(nodeC->right, nodeB);
}

void test_merge() {
	huffnode_t *nodeA = createHuffnode((void *)novoByte('A'), 1, NULL, NULL);
	huffnode_t *nodeB = createHuffnode((void *)novoByte('B'), 2, NULL, NULL);

	huffnode_t *merge1 = merge(nodeA, nodeB);

	CU_ASSERT(*((byte_t *)merge1->item) == '*');
	CU_ASSERT(merge1->frequency == 3);
	CU_ASSERT_PTR_EQUAL(merge1->left, nodeA);
	CU_ASSERT_PTR_EQUAL(merge1->right, nodeB);

	huffnode_t *nodeC = createHuffnode((void *)novoByte('C'), 3, NULL, NULL);
	huffnode_t *nodeD = createHuffnode((void *)novoByte('D'), 4, NULL, NULL);

	huffnode_t *merge2 = merge(nodeC, nodeD);

	CU_ASSERT(*((byte_t *)merge2->item) == '*');
	CU_ASSERT(merge2->frequency == 7);
	CU_ASSERT_PTR_EQUAL(merge2->left, nodeC);
	CU_ASSERT_PTR_EQUAL(merge2->right, nodeD);

	huffnode_t *merge3 = merge(merge1, merge2);

	CU_ASSERT(*((byte_t *)merge3->item) == '*');
	CU_ASSERT(merge3->frequency == 10);
	CU_ASSERT_PTR_EQUAL(merge3->left, merge1);
	CU_ASSERT_PTR_EQUAL(merge3->right, merge2);
}

void test_isLeaf() {
	huffnode_t *nodeA = createHuffnode((void *)novoByte('A'), 1, NULL, NULL);
	huffnode_t *nodeB = createHuffnode((void *)novoByte('B'), 2, NULL, NULL);

	CU_ASSERT(isLeaf(nodeA) == 1);
	CU_ASSERT(isLeaf(nodeB) == 1);

	huffnode_t *nodeC = createHuffnode((void *)novoByte('*'), 3, nodeA, nodeB);

	CU_ASSERT(isLeaf(nodeC) == 0);

	huffnode_t *nodeD = merge(nodeA, nodeB);

	CU_ASSERT(isLeaf(nodeD) == 0);
}

void test_getTreeSize() {
	huffnode_t *nodeA = createHuffnode((void *)novoByte('A'), 1, NULL, NULL);
	huffnode_t *nodeB = createHuffnode((void *)novoByte('B'), 2, NULL, NULL);

	CU_ASSERT(getTreeSize(nodeA) == 1);
	CU_ASSERT(getTreeSize(nodeB) == 1);

	huffnode_t *merged1 = merge(nodeA, nodeB);

	CU_ASSERT(getTreeSize(merged1) == 3);

	huffnode_t *nodeC = createHuffnode((void *)novoByte('A'), 1, NULL, NULL);
	huffnode_t *nodeD = createHuffnode((void *)novoByte('B'), 2, NULL, NULL);

	huffnode_t *merged2 = merge(nodeC, nodeD);

	CU_ASSERT(getTreeSize(merged2) == 3);

	huffnode_t *tree = merge(merged1, merged2);

	CU_ASSERT(getTreeSize(tree) == 7);

	// Arvore vazia
	CU_ASSERT(getTreeSize(NULL) == 0);
}
