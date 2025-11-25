#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAXLEN (1<<10)
#define BLOCKS 202520252025000

size_t part1(int *nums, size_t nnums, size_t ncols) {
	size_t total = 0;
	for (size_t i = 0; i < nnums; i++) {
		total += ncols/nums[i];
	}
	return total;
}

// Returns the product
// and transforms the nums/nnums array to be the spell rather than the wall fragment
size_t part2(int *nums, size_t *nnums) {
	static int copy[MAXLEN];
	memcpy(copy, nums, *nnums*sizeof(*nums));

	size_t product = 1;
	size_t new_count = 0;
	for (size_t i = 1; i < *nnums; i++) {
		if (copy[i] > 0) {
			product *= i;
			size_t n = i;
			for (size_t j = i; j < *nnums; j+=n) {
				copy[j]--;
			}
			nums[new_count++] = i;
#ifdef TESTING
			printf("%zu\n", i);
#endif
		}
	}

	*nnums = new_count;
	return product;
}

int main() {
#ifdef TESTING
	FILE *f = fopen("../testp3.txt", "r");
	//FILE *f = fopen("../everybody_codes_e2025_q16_p3.txt", "r");
#else
	FILE *f = fopen("../everybody_codes_e2025_q16_p3.txt", "r");
#endif
	if (!f) {
		puts("Couldn't open file");
		return 1;
	}

	char *line = NULL;
	size_t buflen = 0;
	getline(&line, &buflen, f);

	int nums[MAXLEN];
	size_t nnums = 1;	//1-based indexing for this problem
	for (char *tok = strtok(line, ","); tok; tok = strtok(NULL, ",")) {
		if (nnums >= MAXLEN) {
			puts("Array out of space!");
			return 1;
		}
	
		int n = atoi(tok);
		nums[nnums++] = n;
	}

	// Convert the input into its corresponding spell with part2
	// This also changes nnums to zero-based indexing for the rest of the code
	part2(nums, &nnums);

	// Search algorithm
	// Initial guess
	size_t cols = BLOCKS / 2;
	size_t used = part1(nums, nnums, cols);
	size_t iters = 0;
	while (used != BLOCKS) {
	long long offset = BLOCKS - used;
#ifdef TESTING
		printf("%zu %lld\n", used, offset);
#endif

		// Converge proportional to the offset
		// but for larger numbers, it's too much of a swing

		// I'm not 100% sure this will converge for all inputs,
		// but it works very quickly for the test and real input (only 8 iterations)
		if (llabs(offset) >= 2)
			cols += offset>>1;
		else
			cols += offset;
			
		used = part1(nums, nnums, cols);
		size_t next_used = part1(nums, nnums, cols+1);
		if (used < BLOCKS && next_used > BLOCKS) break;
		iters++;
	}
	
	printf("part3: %zu in %zu iterations\n", cols, iters);
}
