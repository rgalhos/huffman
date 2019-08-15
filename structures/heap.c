#ifndef HEAP_H
#define HEAP_H

#ifndef swap
#define swap(x, y, T) { T aux = x; x = y; y = aux; }
#endif

typedef struct heap_t {
	int (*getter)(struct heap_t *, int);
	int maxSize;
	int size;
	void **data;
} heap_t;

int getParentIndex(int index) { return index >> 1; }
int getLeftIndex(int index) { return index << 1; }
int getRightIndex(int index) { return (index << 1) + 1; }
int isHeapEmpty(heap_t *heap) { return !heap->size; }
int getHeapSize(heap_t *heap) { return heap->size; }
void* peek(heap_t *heap, int index) { return heap->data[index]; }

heap_t* createHeap(int maxSize, int (*getter)(heap_t *, int)) {
	heap_t *newHeap = (heap_t *)malloc(sizeof(heap_t));
	newHeap->data = (void **)malloc(maxSize * sizeof(void *));
	newHeap->maxSize = maxSize;
	newHeap->getter = getter;
	newHeap->size = 0;

	return newHeap;
}

void minHeapify(heap_t *heap, int i) {
	int leftIndex = getLeftIndex(i);
	int rightIndex = getRightIndex(i);
	int largest;

	if (leftIndex <= heap->size && heap->getter(heap, leftIndex) < heap->getter(heap, i)) {
		largest = leftIndex;
	} else {
		largest = i;
	}

	if (rightIndex <= heap->size && heap->getter(heap, rightIndex) < heap->getter(heap, largest)) {
		largest = rightIndex;
	}

	if (heap->data[i] != heap->data[largest]) {
		swap(heap->data[i], heap->data[largest], void *);
		minHeapify(heap, largest);
	}
}

void buildMinHeap(heap_t *heap) {
	for (int i = 1; i < heap->size / 2; i++) {
		minHeapify(heap, i);
	}
}

void enqueue(heap_t *heap, void *item) {
	if (heap->size >= heap->maxSize) {
		fprintf(stderr, "Heap overflow (%d items)\n", heap->size);
		return;
	}

	heap->data[++heap->size] = item;

	buildMinHeap(heap);
}

void* dequeue(heap_t *heap) {
	if (isHeapEmpty(heap)) {
		fprintf(stderr, "Heap underflow\n");
		return NULL;
	}

	void *item = heap->data[1];
	heap->data[1] = heap->data[heap->size--];
	minHeapify(heap, 1);

	return item;
}

#endif