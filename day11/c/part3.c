#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COLS 128

int main() {
	FILE *f = fopen("../everybody_codes_e2025_q11_p3.txt", "r");
	if (!f) {
		puts("Couldn't open file");
		return 1;
	}

	unsigned long long cols[MAX_COLS];
	size_t numCols = 0;
	
	char *line = NULL;
	size_t buflen = 0;
	ssize_t bytes;
	while ((bytes = getline(&line, &buflen, f)) != -1) {
		cols[numCols++] = strtoull(line, NULL, 10);
	}
	free(line);

	for (size_t i = 0; i < numCols; i++) {
		printf("%llu\n", cols[i]);
	}
	putchar('\n');

	// Verify that the input is indeed in ascending order
	for (size_t i = 0; i < numCols-1; i++) {
		if (cols[i] >= cols[i+1]) {
			puts("Input not in ascending order!");
			return 1;
		}
	}

	// Find the mean value
	// This is the number all columns would have at the end
	unsigned long long sum = 0;
	for (size_t i = 0; i < numCols; i++) {
		sum += cols[i];
	}
	unsigned long long avg = sum / numCols;

	// Now the number of rounds is the number of ducks that need to be added
	// to those columns that are less than the mean value
	size_t ans = 0;
	for (size_t i = 0; i < numCols; i++) {
		if (cols[i] >= avg) break;
		ans += avg - cols[i];
	}

	printf("part3: %zu\n", ans);
}
