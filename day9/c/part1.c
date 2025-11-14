#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define NUM_SEQS 3

size_t identifyChild(char *seqs[], size_t seqLen) {
	for (size_t i = 0; i < NUM_SEQS; i++) {
		for (size_t j = 0; j < seqLen; j++) {
			unsigned matches = 0;
			for (size_t k = 0; k < NUM_SEQS; k++) {
				matches += seqs[i][j] == seqs[k][j];
			}
			if (matches < 2) goto next;
		}
		return i;
	next:
	}
	assert(0);
}

size_t computeSimilarity(char *seqa, char *seqb, size_t seqLen) {
	size_t count = 0;
	for (size_t i = 0; i < seqLen; i++) {
		count += seqa[i] == seqb[i];
	}
	return count;
}

int main() {
	FILE *f = fopen("../everybody_codes_e2025_q09_p1.txt", "r");
	if (!f) {
		puts("Couldn't open file");
		return 1;
	}

	char *lines[NUM_SEQS];
	char *seqs[NUM_SEQS];
	size_t seqLen;

	for (size_t i = 0; i < NUM_SEQS; i++) {
		size_t buflen;
		seqLen = getline(lines+i, &buflen, f);
		seqs[i] = strchr(lines[i], ':') + 1;
	}
	seqLen -= seqs[0] - lines[0];

/*
	// Print the sequences
	for (size_t s = 0; s < NUM_SEQS; s++) {
		for (size_t i = 0; i < seqLen; i++) {
			putchar(seqs[s][i]);
		}
		putchar('\n');
	}
*/
	
	size_t childIndex = identifyChild(seqs, seqLen);

	size_t similarities[NUM_SEQS] = {0};
	for (size_t i = 0; i < NUM_SEQS; i++) {
		if (i == childIndex) continue;
		similarities[i] = computeSimilarity(seqs[childIndex], seqs[i], seqLen);
	}
	similarities[childIndex] = 1;

	size_t product = 1;
	for (size_t i = 0; i < NUM_SEQS; i++) {
		product *= similarities[i];
	}

	printf("part1: %zu\n", product);
}
