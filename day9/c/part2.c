#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define NUM_SEQS 100

int isChild(char *child, char *par1, char *par2, size_t seqLen) {
	for (size_t i = 0; i < seqLen; i++) {
		if (child[i] != par1[i] &&
			child[i] != par2[i])
		{
			return 0;
		}
	}
	return 1;
}

size_t computeSimilarity(char *seqa, char *seqb, size_t seqLen) {
	size_t count = 0;
	for (size_t i = 0; i < seqLen; i++) {
		count += seqa[i] == seqb[i];
	}
	return count;
}

int main() {
	FILE *f = fopen("../everybody_codes_e2025_q09_p2.txt", "r");
	if (!f) {
		puts("Couldn't open file");
		return 1;
	}

	char *lines[NUM_SEQS] = {NULL};
	char *seqs[NUM_SEQS];
	size_t seqLen;
	for (size_t i = 0; i < NUM_SEQS; i++) {
		size_t buflen;
		seqLen = getline(lines+i, &buflen, f);
		seqs[i] = strchr(lines[i], ':') + 1;
	}
	seqLen -= seqs[NUM_SEQS-1] - lines[NUM_SEQS-1];

/*
	// Print the sequences
	for (size_t s = 0; s < NUM_SEQS; s++) {
		for (size_t i = 0; i < seqLen; i++) {
			putchar(seqs[s][i]);
		}
		putchar('\n');
	}
	printf("seqLen = %zu\n", seqLen);
*/

	// brute force ftw
	size_t total = 0;
	for (size_t child = 0; child < NUM_SEQS; child++) {
		for (size_t par1 = 0; par1 < NUM_SEQS; par1++) {
			if (child == par1) continue;
			for (size_t par2 = par1+1; par2 < NUM_SEQS; par2++) {
				if (child == par2 || par1 == par2) continue;
				assert(child != par1 && par1 != par2);

				if (isChild(seqs[child], seqs[par1], seqs[par2], seqLen)) {
					printf("Child %zu has parents %zu and %zu\n", child, par1, par2);
					total += computeSimilarity(seqs[child], seqs[par1], seqLen) *
						computeSimilarity(seqs[child], seqs[par2], seqLen);
					continue;
				}
			}
		}
	}

	printf("part2: %zu\n", total);
}
