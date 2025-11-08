#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static int found[256];

int main() {
	FILE *f = fopen("../everybody_codes_e2025_q03_p3.txt", "r");
	if (!f) {
		puts("Couldn't open file");
		return 1;
	}

	char *line;
	size_t linelen;
	ssize_t bytes_read = getline(&line, &linelen, f);

	char *num = line;
	do {
		int n = atoi(num);
		assert(n < 256 && n >= 0);
		found[n] += 1;
		num = strchr(num, ',');
	} while(num++);	//Inc num to skip comma

	int max = 0;
	for (size_t i = 0; i < 256; i++) {
		printf("%d\t", found[i]);
	}
	printf("Minimum no. of sets: %d\n", max);
}
