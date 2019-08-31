#include "../structures/hashtable.c"

long *novoLong(long num) {
	long *ptr = (long *)malloc(sizeof(long));
	*ptr = num;
	return ptr;
}

void test_has() {
	hashtable_t *table = createHashtable(16);

	put(table, novoLong(42), 1);
	put(table, novoLong(69), 2);
	put(table, novoLong(666), 4);
	put(table, novoLong(1337), 8);
	put(table, novoLong(2048), 16);

	CU_ASSERT_TRUE(has(table, 1));
	CU_ASSERT_TRUE(has(table, 2));
	CU_ASSERT_TRUE(has(table, 4));
	CU_ASSERT_TRUE(has(table, 8));
	CU_ASSERT_TRUE(has(table, 16));

	CU_ASSERT_FALSE(has(table, 3));
	CU_ASSERT_FALSE(has(table, 6));
	CU_ASSERT_FALSE(has(table, 9));
	CU_ASSERT_FALSE(has(table, 12));
	CU_ASSERT_FALSE(has(table, 15));
}

void test_put_get() {
	hashtable_t *table = createHashtable(16);

	put(table, novoLong(42), 1);
	put(table, novoLong(69), 2);
	put(table, novoLong(666), 4);
	put(table, novoLong(1337), 8);
	put(table, novoLong(2048), 16);

	CU_ASSERT_EQUAL(*((long *)get(table, 1)), 42L);
	CU_ASSERT_EQUAL(*((long *)get(table, 2)), 69L);
	CU_ASSERT_EQUAL(*((long *)get(table, 4)), 666L);
	CU_ASSERT_EQUAL(*((long *)get(table, 8)), 1337L);
	CU_ASSERT_EQUAL(*((long *)get(table, 16)), 2048L);

	CU_ASSERT_PTR_NULL(get(table, 3));
	CU_ASSERT_PTR_NULL(get(table, 6));
	CU_ASSERT_PTR_NULL(get(table, 9));
	CU_ASSERT_PTR_NULL(get(table, 12));
	CU_ASSERT_PTR_NULL(get(table, 15));
}

void test_delete() {
	hashtable_t *table = createHashtable(16);

	put(table, novoLong(42), 1);
	put(table, novoLong(69), 2);
	put(table, novoLong(666), 4);
	put(table, novoLong(1337), 8);
	put(table, novoLong(2048), 16);

	CU_ASSERT_TRUE(has(table, 1));
	CU_ASSERT_TRUE(delete(table, 1));
	CU_ASSERT_FALSE(has(table, 1));
	CU_ASSERT_PTR_NULL(get(table, 1));

	CU_ASSERT_TRUE(has(table, 2));
	CU_ASSERT_TRUE(delete(table, 2));
	CU_ASSERT_FALSE(has(table, 2));
	CU_ASSERT_PTR_NULL(get(table, 2));

	// Testando com um elemento que não existe na hashtable
	CU_ASSERT_FALSE(has(table, 3));
	CU_ASSERT_PTR_NULL(get(table, 3));
	CU_ASSERT_FALSE(delete(table, 3));
	CU_ASSERT_FALSE(has(table, 3));
	CU_ASSERT_PTR_NULL(get(table, 3));
}
