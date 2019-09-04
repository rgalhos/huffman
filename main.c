#define DEBUG if (0)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extract.c"
#include "compress.c"

int main(int argc, char **argv) {
	if (argc < 2) {
		printf(
				"Usage:\n"
				"%s \033[01;33m-c\033[0m \033[0;36m[FILE]\033[0m \t to compress a file\n"
				"%s \033[01;33m-e\033[0m \033[0;36m[FILE]\033[0m \t to extract a file\n",
			argv[0], argv[0]
		);

		return 1;
	}
	
	if (!strcmp(argv[1], "-c")) {
		return compress(argv[2]);
	} else if (!strcmp(argv[1], "-e")) {
		return extract(argv[2]);
	} else {
		printf(
				"Usage:\n"
				"%s \033[01;33m-c\033[0m \033[0;36m[FILE]\033[0m \t to compress a file\n"
				"%s \033[01;33m-e\033[0m \033[0;36m[FILE]\033[0m \t to extract a file\n",
			argv[0], argv[0]
		);
	}
	
	return 1;
}
