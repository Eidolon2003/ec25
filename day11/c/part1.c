#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COLS 128
#define NUM_ROUNDS 10

size_t compute_checksum(int *arr, size_t len) {
	size_t n = 0;
	for (size_t i = 0; i < len; i++) {
		n += arr[i] * (i+1);
	}
	return n;
}

int main() {
#ifdef TESTING
	FILE *f = fopen("../testp1.txt", "r");
#else
	FILE *f = fopen("../everybody_codes_e2025_q11_p1.txt", "r");
#endif
	if (!f) {
		puts("Couldn't open file");
		return 1;
	}

	int cols[MAX_COLS];
	size_t numCols = 0;
	memset(cols, 0, sizeof(cols));

	char *line;
	size_t buflen;
	ssize_t bytes;
	while ((bytes = getline(&line, &buflen, f)) != -1) {
		cols[numCols++] = atoi(line);
	}

#ifdef TESTING
	for (size_t i = 0; i < numCols; i++) {
		printf("%d\n", cols[i]);
	}
	putchar('\n');
#endif

	// Phase 1:
	int rounds = 0;
	for (int done = 0; rounds <= NUM_ROUNDS && !done; rounds++) {
		done = 1;
		for (size_t i = 0; i < numCols-1; i++) {
			if (cols[i+1] < cols[i]) {
				cols[i+1]++;
				cols[i]--;
				done = 0;
			}
		}
#ifdef TESTING
		printf("Phase 1, round %d\n", rounds);
#endif
	}
	
	// Phase 2:
	for (int done = 0; rounds <= NUM_ROUNDS && !done; rounds++) {
		done = 1;
		for (size_t i = 0; i < numCols-1; i++) {
			if (cols[i+1] > cols[i]) {
				cols[i+1]--;
				cols[i]++;
				done = 0;
			}
		}
#ifdef TESTING
		printf("Phase 2, round %d\n", rounds);
#endif
	}

	size_t checksum = compute_checksum(cols, numCols);
	printf("part1: %zu\n", checksum);
}
