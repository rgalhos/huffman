#include "../structures/heap.c"
#include "../structures/huffnode.c"

long getValueInt(heap_t *heap, int index) {
	return (long)(*((int *)peek(heap, index)));
}

long getFrequency(heap_t *heap, int index) {
	return ((huffnode_t *)peek(heap, index))->frequency;
}

int *novoInt(int n) {
	int *ptr = (int *)malloc(sizeof(int));
	*ptr = n;
	return ptr;
}

#ifndef NOVOBYTE
#define NOVOBYTE

byte_t *novoByte(byte_t c) {
	byte_t *ptr = (byte_t *)malloc(sizeof(byte_t));
	*ptr = c;
	return ptr;
}

#endif

void test_createHeap() {
	heap_t *heap = createHeap(16, getValueInt);

	CU_ASSERT(heap->maxSize == 16);
	CU_ASSERT(heap->size == 0);
	CU_ASSERT_PTR_EQUAL(heap->getter, getValueInt);

	// Assegurar que todos os elementos da array são NULL
	for (int i = 0; i < 16; i++) {
		CU_ASSERT_PTR_NULL(heap->data[i]);
	}
}

void test_peek() {
	heap_t *heap = createHeap(16, getValueInt);

	enqueue(heap, novoInt(1));
	enqueue(heap, novoInt(8));
	enqueue(heap, novoInt(16));

	CU_ASSERT(*((int *)peek(heap, 1)) == 1);
	CU_ASSERT(*((int *)peek(heap, 2)) == 8);
	CU_ASSERT(*((int *)peek(heap, 3)) == 16);
}

void test_enqueue() {
	heap_t *heap = createHeap(16, getValueInt);

	enqueue(heap, novoInt(1));
	enqueue(heap, novoInt(2));

	CU_ASSERT(*((int *)heap->data[1]) == 1);
	CU_ASSERT(*((int *)heap->data[2]) == 2);

	enqueue(heap, novoInt(-1));
	enqueue(heap, novoInt(-2));

	CU_ASSERT(*((int *)heap->data[1]) == -2);
	CU_ASSERT(*((int *)heap->data[2]) == -1);
	CU_ASSERT(*((int *)heap->data[3]) == 1);
	CU_ASSERT(*((int *)heap->data[4]) == 2);

	//////////////////////////////////////////////////////

	// A array da heap suporta 16 elementos. Já temos 4, vamos colocar mais 12
	for (int i = 0; i < 12; i++) {
		enqueue(heap, (void *)novoInt(i));
	}

	// Nesse momento a array já está cheia

	CU_ASSERT(getHeapSize(heap) == 16)

	// Isso vai acabar printando "heap overflow (16 items)" na tela
	enqueue(heap, (void *)0xC0FFEE);

	CU_ASSERT(getHeapSize(heap) == 16);

	//////////////////////////////////////////////////////

	heap = createHeap(16, getFrequency);

	enqueue(heap, createHuffnode((void *)novoByte('A'), 128, NULL, NULL));
	CU_ASSERT(*((char *)((huffnode_t *)heap->data[1])->item) == 'A');
	CU_ASSERT(((huffnode_t *)heap->data[1])->frequency == 128);
	CU_ASSERT_PTR_NULL(((huffnode_t *)heap->data[1])->left);
	CU_ASSERT_PTR_NULL(((huffnode_t *)heap->data[1])->right);

	enqueue(heap, createHuffnode((void *)novoByte('B'), 64, NULL, NULL));
	CU_ASSERT(*((char *)((huffnode_t *)heap->data[1])->item) == 'B');
	CU_ASSERT(((huffnode_t *)heap->data[1])->frequency == 64);
	CU_ASSERT_PTR_NULL(((huffnode_t *)heap->data[1])->left);
	CU_ASSERT_PTR_NULL(((huffnode_t *)heap->data[1])->right);

	CU_ASSERT(*((char *)((huffnode_t *)heap->data[2])->item) == 'A');
	CU_ASSERT(((huffnode_t *)heap->data[2])->frequency == 128);
	CU_ASSERT_PTR_NULL(((huffnode_t *)heap->data[2])->left);
	CU_ASSERT_PTR_NULL(((huffnode_t *)heap->data[2])->right);
}

void test_dequeue() {
	heap_t *heap = createHeap(16, getValueInt);

	enqueue(heap, novoInt(1));
	enqueue(heap, novoInt(2));

	int dequeued = *((int *)dequeue(heap));
	CU_ASSERT(dequeued == 1);

	dequeued = *((int *)dequeue(heap));
	CU_ASSERT(dequeued == 2);

	// Heap vazia deve retornar NULL
	// Deve printar "heap underflow" na tela
	CU_ASSERT_PTR_NULL(dequeue(heap));
	CU_ASSERT_PTR_NULL(dequeue(heap));

	enqueue(heap, novoInt(1));
	enqueue(heap, novoInt(2));
	enqueue(heap, novoInt(-1));
	enqueue(heap, novoInt(-2));

	dequeued = *((int *)dequeue(heap));
	CU_ASSERT(dequeued == -2);
	dequeued = *((int *)dequeue(heap));
	CU_ASSERT(dequeued == -1);
	dequeued = *((int *)dequeue(heap));
	CU_ASSERT(dequeued == 1);
	dequeued = *((int *)dequeue(heap));
	CU_ASSERT(dequeued == 2);
}

void test_isHeapEmpty() {
	heap_t *heap = createHeap(16, getValueInt);

	CU_ASSERT_TRUE(isHeapEmpty(heap));

	enqueue(heap, (void *)0xC0FFEE);

	CU_ASSERT_FALSE(isHeapEmpty(heap));

	dequeue(heap);

	CU_ASSERT_TRUE(isHeapEmpty(heap));
}

void test_getHeapSize() {
	heap_t *heap = createHeap(16, getValueInt);

	enqueue(heap, novoInt(1));
	enqueue(heap, novoInt(2));

	CU_ASSERT(getHeapSize(heap) == 2);

	enqueue(heap, novoInt(128));
	enqueue(heap, novoInt(256));

	CU_ASSERT(getHeapSize(heap) == 4);

	dequeue(heap);

	CU_ASSERT(getHeapSize(heap) == 3);
}
