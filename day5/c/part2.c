#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct {
	int left, right, middle;
} segment;

#define MAX_SPINE_LENGTH 128
static segment spine[MAX_SPINE_LENGTH];
static size_t next;

// Part 1 factored out into its own function
long long computeQuality(char *line) {
	// Initialize the spine to all -1
	memset(spine, 0xFF, MAX_SPINE_LENGTH*sizeof(segment));
	next = 0;

	// Loop over each number in the list
	for (char *num = strchr(line, ':'); num; num = strchr(num, ',')) {
		int n = atoi(++num);

		// Loop through the spine to place the number
		size_t i = 0;
		for (; i < next; i++) {
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
		if (i == next) {
			if (next == MAX_SPINE_LENGTH) {
				puts("Spine array out of space!");
				exit(1);
			}
			spine[next++].middle = n;
		}
	}

	// Put together the final result
	char result[128];
	char buf[16];
	result[0] = 0;
	for (size_t i = 0; i < next; i++) {
		snprintf(buf, 16, "%d", spine[i].middle);
		strncat(result, buf, 16);
	}
	return atoll(result);
}

int main() {
	FILE *f = fopen("../everybody_codes_e2025_q05_p2.txt", "r");
	if (!f) {
		puts("Failed to open file");
		return 1;
	}

	// Rather than only reading one line, we loop through all lines
	long long max = LLONG_MIN;
	long long min = LLONG_MAX;
	char *line;
	size_t linelen;
	ssize_t bytes_read;

	while ((bytes_read = getline(&line, &linelen, f)) != -1) {
		line[bytes_read] = 0;
		long long q = computeQuality(line);
		if (q > max) max = q;
		if (q < min) min = q;
	}

	printf("Result: %lld\n", max-min);

	free(line);
	fclose(f);
}
