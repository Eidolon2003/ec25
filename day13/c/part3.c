#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define DIAL_SIZE (300<<20) // Large enough for p3
static int dial[DIAL_SIZE];

#ifdef TESTING
#define TURNS 20252025
#else
#define TURNS 202520252025
#endif

int main() {
#ifdef TESTING
	FILE *f = fopen("../testp2.txt", "r");
#else
	FILE *f = fopen("../everybody_codes_e2025_q13_p3.txt", "r");
#endif
	if (!f) {
		puts("Couldn't open file");
		return 1;
	}

	size_t dial_center = DIAL_SIZE/2;
	dial[dial_center] = 1;

	size_t left_bound = 0;	// Offset to the leftmost number (inclusive)
	size_t right_bound = 1; // Offset to the next rightmost number (exclusive)

	char *line = NULL;
	size_t buflen = 0;
	while (getline(&line, &buflen, f) != -1) {
		// Clockwise
		int lo = atoi(line);
		int hi = atoi(strchr(line, '-')+1);

//		size_t n = hi - lo + 1;		
//		if (dial_center + right_bound + n >= DIAL_SIZE) {
//			puts("Overflow on right");
//			return 1;
//		}
		
		for (int x = lo; x <= hi; x++) {
			dial[dial_center + (right_bound++)] = x;
		}

		// Counter clockwise
		if (getline(&line, &buflen, f) == -1) break;
		lo = atoi(line);
		hi = atoi(strchr(line, '-')+1);

//		n = hi - lo + 1;
//		if (left_bound + n > dial_center) {
//			puts("Overflow on left");
//			return 1;
//		}

		for (int n = lo; n <= hi; n++) {
			dial[dial_center - (++left_bound)] = n;
		}
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
	
	printf("part3: %d\n", dial[index]);
}
