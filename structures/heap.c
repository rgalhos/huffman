#ifndef HEAP_H
#define HEAP_H

#ifndef swap
#define swap(x, y, T) { T aux = x; x = y; y = aux; }
#endif

typedef struct heap_t {
	long (*getter)(struct heap_t *, int);
	int maxSize;
	int size;
	void **data;
} heap_t;

int getParentIndex(int index) {
    return index >> 1;
}

int getLeftIndex(int index) {
    return index << 1;
}

int getRightIndex(int index) {
    return (index << 1) + 1;
}

int isHeapEmpty(heap_t *heap) {
    return !heap->size;
}

 int getHeapSize(heap_t *heap) {
    return heap->size;
}

void* peek(heap_t *heap, int index) {
    return heap->data[index];
}

heap_t* createHeap(int maxSize, long (*getter)(heap_t *, int)) {
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
	int smallest = i;

	if (leftIndex <= heap->size && heap->getter(heap, leftIndex) <= heap->getter(heap, i)) {
		smallest = leftIndex;
	}

	if (rightIndex <= heap->size && heap->getter(heap, rightIndex) <= heap->getter(heap, smallest)) {
		smallest = rightIndex;
	}

	if (heap->data[i] != heap->data[smallest]) {
		swap(heap->data[i], heap->data[smallest], void *);
		minHeapify(heap, smallest);
	}
}

void enqueue(heap_t *heap, void *item) {
	if (heap->size >= heap->maxSize) {
		fprintf(stderr, "Heap overflow (%d items)\n", heap->size);
		return;
	}

	heap->data[++heap->size] = item;
	
	int key_index = heap->size;
	int parent_index = getParentIndex(heap->size);
	
	while(parent_index >= 1 &&  heap->getter(heap, key_index) <= heap->getter(heap, parent_index) ){
		swap(heap->data[key_index], heap->data[parent_index], void *);
		key_index = parent_index;
		parent_index = getParentIndex(key_index);
	}
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
