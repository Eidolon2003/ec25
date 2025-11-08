#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int left, right, middle;
} segment;

#define MAX_SPINE_LENGTH 128
static segment spine[MAX_SPINE_LENGTH];
static size_t next = 0;

void printspine() {
	puts("\n");
	for (size_t i = 0; i < next; i++) {
		printf("%d - %d - %d\n    |\n", spine[i].left, spine[i].middle, spine[i].right);
	}
}

int main() {
	FILE *f = fopen("../everybody_codes_e2025_q05_p1.txt", "r");
	if (!f) {
		puts("Failed to open file");
		return 1;
	}

	char *line;
	size_t linelen;
	ssize_t bytes_read = getline(&line, &linelen, f);
	if (bytes_read == -1) {
		puts("File read error");
		return 1;
	}
	line[bytes_read] = 0;

	// Initialize the spine to all -1
	memset(spine, 0xFF, MAX_SPINE_LENGTH*sizeof(segment));

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
				return 1;
			}
			spine[next++].middle = n;
		}

		printspine();
	}

	// Put together the final result
	char result[128];
	char buf[16];
	result[0] = 0;
	for (size_t i = 0; i < next; i++) {
		snprintf(buf, 16, "%d", spine[i].middle);
		strncat(result, buf, 16);
	}
	printf("\n\nQuality: %s\n", result);

	free(line);
	fclose(f);
}
