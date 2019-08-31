#include <stdio.h>
#include <stdlib.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "./test_heap.c"
#include "./test_huffnode.c"
#include "./test_hashtable.c"

int init_suite(void) {
	return 0;
}

int clean_suite(void) {
	return 0;
}

int start_tests(void) {
	CU_pSuite pSuite = NULL;

	// Initialize CUnit test registry
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	// Add suite to registry
	pSuite = CU_add_suite("Basic_Test_Suite", init_suite, clean_suite);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/*
	 * Início dos testes
	 */
	/*
	 * Testes da hashtable
	 */
	if (NULL == CU_add_test(pSuite, "test_has", test_has)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_put_get", test_put_get)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_delete", test_delete)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/*
	 * Testes da árvore
	 */
	if (NULL == CU_add_test(pSuite, "test_createHuffnode", test_createHuffnode)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_merge", test_merge)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_isLeaf", test_isLeaf)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_getTreeSize", test_getTreeSize)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/*
	 * Testes da heap
	 */
	if (NULL == CU_add_test(pSuite, "test_createHeap", test_createHeap)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_peek", test_peek)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_enqueue", test_enqueue)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_dequeue", test_dequeue)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_isHeapEmpty", test_isHeapEmpty)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_getHeapSize", test_getHeapSize)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	CU_basic_run_tests();
	CU_cleanup_registry();

	return CU_get_error();
}

int main() {
	start_tests();
}
