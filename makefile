main:
	gcc main.c -o main

tests:
	gcc ./cunit_tests/test_main.c -o tests -lcunit
