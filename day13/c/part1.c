#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define DIAL_SIZE 1024
#define TURNS 2025

int main() {
#ifdef TESTING
	FILE *f = fopen("../testp1.txt", "r");
#else
	FILE *f = fopen("../everybody_codes_e2025_q13_p1.txt", "r");
#endif
	if (!f) {
		puts("Couldn't open file");
		return 1;
	}

	int dial[DIAL_SIZE];
	size_t dial_center = DIAL_SIZE/2;
	dial[dial_center] = 1;

	size_t left_bound = 0;	// Offset to the leftmost number (inclusive)
	size_t right_bound = 1; // Offset to the next rightmost number (exclusive)

	char *line = NULL;
	size_t buflen = 0;
	while (getline(&line, &buflen, f) != -1) {
		dial[dial_center + (right_bound++)] = atoi(line);
		if (getline(&line, &buflen, f) == -1) break;
		dial[dial_center - (++left_bound)] = atoi(line);
	}
	free(line);
	fclose(f);
	size_t dial_width = right_bound+left_bound;

#ifdef TESTING
	printf("width %zu\n", dial_width);
	for (size_t i = dial_center-left_bound; i < dial_center+right_bound; i++) {
		printf("%d\n", dial[i]);
	}
	putchar('\n');
#endif

	size_t index = ((TURNS+left_bound) % dial_width) + dial_center-left_bound;
	assert(index >= dial_center-left_bound && index < dial_center+right_bound);
	
	printf("part1: %d\n", dial[index]);
}
