#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define TESTING

#ifdef TESTING
//#define REPETITIONS 2
//#define DISTANCE 10
#define REPETITIONS 10
#define DISTANCE 1000
#else
#define REPETITIONS 1000
#define DISTANCE 1000
#endif

size_t count(char *start, char c) {
	size_t n = 0;
	for (size_t i = 0; i < 2*DISTANCE+1; i++) {
		if (start[i] == c) n++;
	}
	return n;
}

int main() {
	FILE *f = fopen("../everybody_codes_e2025_q06_p3.txt", "r");
	if (!f) {
		puts("Couldn't open file");
		return 1;
	}

	char *line;
	size_t linelen;
	ssize_t bytes;
#ifdef TESTING
	// Expected result: 72
	//line = "AABCBABCABCabcabcABCCBAACBCa";
	//linelen = 28;

	// Expected result: 3442321
	line = "AABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCaAABCBABCABCabcabcABCCBAACBCa";
	linelen = 2800;	
	bytes = linelen;
#else
	bytes = getline(&line, &linelen, f);
#endif

	// Allocate enough for 3x the space for a pseudo-circular buffer
	// (not properly mapped in virtual address space, so consuming more memory than it could be)
	char *buf = malloc(3*bytes);
	memcpy(buf, line, bytes);
	memcpy(buf+bytes, line, bytes);
	memcpy(buf+(2*bytes), line, bytes);
	buf += bytes;

	// Loop through the middle of the buffer with a copy on each side
	size_t total = 0;
	for (ssize_t i = 0; i < bytes; i++) {
		if (buf[i] == 'a' ||
			buf[i] == 'b' ||
			buf[i] == 'c'
		) {
			total += count(buf+i-DISTANCE, buf[i]-32);		
		}
	}

	// total*REPETITIONS is higher than the correct value,
	// due to the fact that we're including pairs that fall off the edge.
	
	// To compensate for this, count the number of matches in just one copy of the input
	// without spilling over the edges

	size_t singlecopy = 0;
	for (ssize_t i = 0; i < bytes; i++) {
		if (line[i] == 'a' ||
			line[i] == 'b' ||
			line[i] == 'c'
		) {
			// Get starting and ending indices that are inbounds
			size_t starti = (i-DISTANCE) > 0 ? (i-DISTANCE) : 0;
			size_t endi = (i+DISTANCE+1) < bytes ? (i+DISTANCE+1) : bytes;
			char target = line[i]-32;

			for (size_t j = starti; j < endi; j++) {
				if (line[j] == target) singlecopy++;
				//putchar(line[j]);
			}
			//putchar('\n');
		}
	}

	//printf("bytes=%zu\n", bytes);
	//printf("total=%zu\nsingle=%zu\n", total, singlecopy);

	// Now we can subtract the extra spillover from our overestimate to get the answer
	size_t spillover = total-singlecopy;
	size_t final_result = total*REPETITIONS - spillover;
	printf("Total pairs: %zu\n", final_result);
}
