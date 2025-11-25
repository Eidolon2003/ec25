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

// Euclid's algorithm!
size_t gcd2(size_t a, size_t b) {
	if (a == 0)
	    return b;
	return gcd2(b%a, a);
}

size_t lcm2(size_t a, size_t b) {
	return (a*b) / gcd2(a, b);
}

size_t lcm(int *nums, size_t nnums) {
	if (nnums == 0) return 0;
	if (nnums == 1) return nums[0];
	if (nnums == 2) return lcm2(nums[0], nums[1]);
	
	size_t x = lcm2(nums[0], nums[1]);
	for (size_t i = 2; i < nnums; i++) {
		x = lcm2(x, nums[i]);
	}
	return x;
}

int main() {
#ifdef TESTING
	//FILE *f = fopen("../testp3.txt", "r");
	FILE *f = fopen("../everybody_codes_e2025_q16_p3.txt", "r");
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
	part2(nums, &nnums);

	size_t cols = 0;
	size_t orig_nnums = nnums;
	for (size_t blocks_remaining = BLOCKS; blocks_remaining > 0; nnums--) {
#ifdef TESTING
		printf("\nremaining: %zu\n", blocks_remaining);
#endif
	
		// The length of a repetition is equal to the LCM of the numbers in question
		size_t rep_len = lcm(nums, nnums);
#ifdef TESTING
		printf("LCM: %zu\n", rep_len);
#endif

		// Use part1 to compute the number of blocks used per repetition
		size_t blocks_per = part1(nums, nnums, rep_len);
#ifdef TESTING
		printf("blocks_per: %zu\n", blocks_per);
#endif

		// Compute how many columns that just knocked out
		size_t num_reps = blocks_remaining / blocks_per;
		if (num_reps == 0) continue;

		size_t b,c;
		do {
			c = num_reps * rep_len;
			b = part1(nums, orig_nnums, c);
		} while (b > blocks_remaining && num_reps--);

		cols += c;
		blocks_remaining -= b;

#ifdef TESTING
		printf("num_reps: %zu\n", num_reps);
		printf("new blocks: %zu\n", b);
#endif

		// If there are still blocks left to place,
		// check again with the highest number removed
		//
		// We know that the highest number won't come into play because 
		// there aren't enough columns remaining for it to place one again.
		// 
		// Removing it from the list makes the lcm smaller for next round
		// nnums-- in the for loop
	}

	printf("part3: %zu\n", cols-1);
}
