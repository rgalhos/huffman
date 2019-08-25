#ifndef HASHTABLE_H
#define HASHTABLE_H

typedef struct element_t {
	int key;
	void *item;
} element_t;

typedef struct hashtable_t {
	int maxSize;
	element_t **items;
} hashtable_t;

hashtable_t *createHashtable(int maxSize) {
	hashtable_t *newTable = (hashtable_t *)malloc(sizeof(hashtable_t));
	newTable->items = (element_t **)calloc(maxSize, sizeof(element_t *));
	newTable->maxSize = maxSize;

	return newTable;
}

int has(hashtable_t *table, int key) {
	return table->items[key] != NULL;
}

int put(hashtable_t *table, void *node, int value) {
	element_t *element = (element_t *)malloc(sizeof(element));
	element->key = value;
	element->item = node;
	table->items[element->key] = element;

	return element->key;
}

void* get(hashtable_t *table, int key) {
	if (table->items[key] == NULL)
		return NULL;

	return table->items[key]->item;
}

int delete(hashtable_t *table, int key) {
	if (table->items[key] == NULL)
		return 0;

	free(table->items[key]);
	table->items[key] = NULL;

	return 1;
}

#endif
