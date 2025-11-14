#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//#define TESTING	// Expect 12

#ifdef TESTING
#define NUM_SEQS 7
#else
#define NUM_SEQS 500
#endif

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

void mergeFam(int *famIDs, int old, int new) {
	if (new == -1 || old == -1) return;
	if (new == old) return;
	for (size_t i = 0; i < NUM_SEQS; i++) {
		if (famIDs[i] == old) famIDs[i] = new;
	}
}

int main() {
#ifdef TESTING
	FILE *f = fopen("../p3test.txt", "r");
#else
	FILE *f = fopen("../everybody_codes_e2025_q09_p3.txt", "r");
#endif
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

#ifdef TESTING
	// Print the sequences
	for (size_t s = 0; s < NUM_SEQS; s++) {
		for (size_t i = 0; i < seqLen; i++) {
			putchar(seqs[s][i]);
		}
		putchar('\n');
	}
	printf("seqLen = %zu\n", seqLen);
#endif

	int famIDs[NUM_SEQS];
	memset(famIDs, 0xFF, sizeof(famIDs));
	size_t numFams = 0;
	
	for (size_t child = 0; child < NUM_SEQS; child++) {
		for (size_t par1 = 0; par1 < NUM_SEQS; par1++) {
			if (child == par1) continue;
			for (size_t par2 = par1+1; par2 < NUM_SEQS; par2++) {
				if (child == par2 || par1 == par2) continue;
				assert(child != par1 && par1 != par2);

				if (isChild(seqs[child], seqs[par1], seqs[par2], seqLen)) {
					int numneg = (famIDs[child]==-1)+(famIDs[par1]==-1)+(famIDs[par2]==-1);
					
					// If the child and both parents aren't part of a family yet, make a new family
					if (numneg == 3) {
#ifdef TESTING
						printf("%zu, %zu, and %zu form new fam %zu\n", child+1, par1+1, par2+1, numFams);
#endif
						famIDs[child] = famIDs[par1] = famIDs[par2] = numFams++;
						continue;
					}

					// If only one is already part of a family, the other two simply join
					if (numneg == 2) {
						int sum = famIDs[child]+famIDs[par1]+famIDs[par2];
						famIDs[child] = famIDs[par1] = famIDs[par2] = sum+2;
#ifdef TESTING
						printf("Two of %zu, %zu, and %zu join fam %d\n", child+1, par1+1, par2+1, sum+2);
#endif
						continue;
					}

					// If two or three are already in, some merging might have to occur.
					// Skip checking and just make a new family
#ifdef TESTING
					printf("%zu, %zu, and %zu merge into fam %zu\n", child+1, par1+1, par2+1, numFams);
#endif
					if (famIDs[child] == -1)
						famIDs[child] = numFams;
					else
						mergeFam(famIDs, famIDs[child], numFams);
					if (famIDs[par1] == -1)
						famIDs[par1] = numFams;
					else
						mergeFam(famIDs, famIDs[par1], numFams);
					if (famIDs[par2] == -1)
						famIDs[par2] = numFams;
					else
						mergeFam(famIDs, famIDs[par2], numFams);

					numFams++;
				}
			}
		}
	}
	assert(numFams < NUM_SEQS);

#ifdef TESTING
	// Print all fams for debug
	for (size_t i = 0; i < NUM_SEQS; i++) {
		printf("%zu is in fam %d\n", i+1, famIDs[i]);
	}
#endif	

	size_t famCounts[NUM_SEQS] = {0};
	for (size_t i = 0; i < NUM_SEQS; i++) {
		famCounts[famIDs[i]]++;
	}

	size_t maxCount = 0;
	int maxFam = 0;
	for (size_t i = 0; i < numFams; i++) {
		if (famCounts[i] > maxCount) {
			maxCount = famCounts[i];
			maxFam = i;
		}
	}
#ifdef TESTING
	printf("Family %d has the most members\n", maxFam);
#endif

	size_t sum = 0;
	for (size_t i = 0; i < NUM_SEQS; i++) {
		if (famIDs[i] == maxFam) sum += i+1;
	}
	printf("part3: %zu\n", sum);
}
