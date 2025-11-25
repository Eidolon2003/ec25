#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 256

int main() {
#ifdef TESTING
	FILE *f = fopen("../testp2.txt", "r");
#else
	FILE *f = fopen("../everybody_codes_e2025_q16_p2.txt", "r");
#endif
	if (!f) {
		puts("Couldn't open file");
		return 1;
	}

	char *line = NULL;
	size_t buflen = 0;
	getline(&line, &buflen, f);

	int nums[MAXLEN];
	size_t ncols = 1;	//1-based indexing for this problem
	for (char *tok = strtok(line, ","); tok; tok = strtok(NULL, ",")) {
		int n = atoi(tok);
		nums[ncols++] = n;
	}

	size_t product = 1;
	for (size_t i = 1; i < ncols; i++) {
		if (nums[i] > 0) {
			printf("%zu\n", i);
			product *= i;
			size_t n = i;
			for (size_t j = i; j < ncols; j+=n) {
				nums[j]--;
			}
		}
	}

	printf("part2: %zu\n", product);
}
