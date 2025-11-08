#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct {
	int left, right, middle;
} segment;

#define MAX_SPINE_LENGTH 128
static segment spine[MAX_SPINE_LENGTH];
static size_t nextSeg;

typedef struct {
	long long quality;
	int id;
	segment spine[MAX_SPINE_LENGTH];
} sword;

#define MAX_NUM_SWORDS 1024
static sword swords[MAX_NUM_SWORDS];
static size_t nextSword = 0;

// Part 1 factored out into its own function
long long computeQuality(char *line) {
	// Initialize the spine to all -1
	memset(spine, 0xFF, MAX_SPINE_LENGTH*sizeof(segment));
	nextSeg = 0;

	// Loop over each number in the list
	for (char *num = strchr(line, ':'); num; num = strchr(num, ',')) {
		int n = atoi(++num);

		// Loop through the spine to place the number
		size_t i = 0;
		for (; i < nextSeg; i++) {
			if (spine[i].middle == -1) {
				spine[i].middle = n;
				break;
			}
			if (n < spine[i].middle && spine[i].left == -1) {
				spine[i].left = n;
				break;
			}
			if (n > spine[i].middle && spine[i].right == -1) {
				spine[i].right = n;
				break;
			}
		}
		if (i == nextSeg) {
			if (nextSeg == MAX_SPINE_LENGTH) {
				puts("Spine array out of space!");
				exit(1);
			}
			spine[nextSeg++].middle = n;
		}
	}

	// Put together the final result
	char result[128];
	char buf[16];
	result[0] = 0;
	for (size_t i = 0; i < nextSeg; i++) {
		snprintf(buf, 16, "%d", spine[i].middle);
		strncat(result, buf, 16);
	}
	return atoll(result);
}

// Return the result of concatenating a spinal segment horizontally
long long concatSegment(const segment *seg) {
	char result[128];
	char buf[16];
	result[0] = '0';
	result[1] = 0;
	
	if (seg->left != -1) {
		snprintf(buf, 16, "%d", seg->left);
		strncat(result, buf, 16);
	}
	snprintf(buf, 16, "%d", seg->middle);
	strncat(result, buf, 16);
	if (seg->right != -1) {
		snprintf(buf, 16, "%d", seg->right);
		strncat(result, buf, 16);
	}
	return atoll(result);
}

// This function returns the comparison between two sword structs for qsort
int compareSwords(const void *a, const void *b) {
	const sword *sa = a;
	const sword *sb = b;

	if (sa->quality > sb->quality) return -1;
	if (sa->quality < sb->quality) return 1;

	for (size_t i = 0; i < MAX_SPINE_LENGTH; i++) {
		if (sa->spine[i].middle == -1) break;
		
		long long sega = concatSegment(sa->spine+i);
		long long segb = concatSegment(sb->spine+i);
		if (sega > segb) return -1;
		if (sega < segb) return 1;
	}
	
	if (sa->id > sb->id) return -1;
	return 1;
}

int main() {
	FILE *f = fopen("../everybody_codes_e2025_q05_p3.txt", "r");
	if (!f) {
		puts("Failed to open file");
		return 1;
	}

	char *line;
	size_t linelen;
	ssize_t bytes_read;
	while ((bytes_read = getline(&line, &linelen, f)) != -1) {
		line[bytes_read] = 0;
		int id = atoi(line);
		long long q = computeQuality(line);

		if (nextSword == MAX_NUM_SWORDS) {
			puts("Swords array out of space");
			return 1;
		}
		else {
			swords[nextSword] = (sword){.quality=q, .id=id};
			memcpy(swords[nextSword].spine, spine, sizeof(spine));
			nextSword++;
		}
	}

	// Sort the swords in descending order
	qsort(swords, nextSword, sizeof(sword), compareSwords);

	// Compute the final checksum
	long long ans = 0;
	for (size_t i = 0; i < nextSword; i++) {
		//printf("%zu:  \t%d\t%lld", i+1, swords[i].id, swords[i].quality);
		long long check = swords[i].id * (i+1);
		//printf("  \t%lld\n", check);
		ans += check;
	}	

	printf("Checksum: %lld\n", ans);

	free(line);
	fclose(f);
}
