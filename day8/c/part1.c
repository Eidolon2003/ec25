#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_NAILS 32

int main() {
	FILE *f = fopen("../everybody_codes_e2025_q08_p1.txt", "r");
	if (!f) {
		puts("Couldn't open file");
		return 1;
	}

	char *line;
	size_t lineBufferLen;
	ssize_t lineBytes = getline(&line, &lineBufferLen, f);

	int prevn = atoi(line);
	char *nstr = strchr(line, ',') + 1;
	size_t count = 0;
	for (nstr = strtok(nstr, ","); nstr; nstr = strtok(NULL, ",")) {
		int n = atoi(nstr);
		if (abs(n-prevn) == NUM_NAILS/2) count++;
		prevn = n;
	}

	printf("Center crossings: %zu\n", count);
}
