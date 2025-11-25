#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COLS 90

int main() {
#ifdef TESTING
	FILE *f = fopen("../testp1.txt", "r");
#else
	FILE *f = fopen("../everybody_codes_e2025_q16_p1.txt", "r");
#endif
	if (!f) {
		puts("Couldn't open file");
		return 1;
	}

	char *line = NULL;
	size_t buflen = 0;
	getline(&line, &buflen, f);

	size_t total = 0;
	for (char *tok = strtok(line, ","); tok; tok = strtok(NULL, ",")) {
		int n = atoi(tok);
		total += COLS/n;
	}

	printf("part1: %zu\n", total);
}
