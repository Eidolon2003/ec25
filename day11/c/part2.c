#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COLS 128

int main() {
#ifdef TESTING
	FILE *f = fopen("../testp2.txt", "r");
#else
	FILE *f = fopen("../everybody_codes_e2025_q11_p2.txt", "r");
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
	for (int done = 0; !done; rounds++) {
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
	for (int done = 0; !done; rounds++) {
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

	// We overcounted by two because phase 1 and 2
	// both have a last loop where no work is done
	rounds -= 2;

	printf("part2: %d\n", rounds);
}
