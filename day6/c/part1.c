#include <stdio.h>
#include <stdlib.h>

int main() {
	FILE *f = fopen("../everybody_codes_e2025_q06_p1.txt", "r");
	if (!f) {
		puts("Couldn't open file");
		return 1;
	}

	char *line;
	size_t linelen;
	getline(&line, &linelen, f);

	int mentors = 0;
	int total = 0;
	for (size_t i = 0; i < linelen; i++) {
		if (line[i] == 'A') mentors++;
		else if (line[i] == 'a') total += mentors;
	}

	printf("Total number of pairs: %d\n", total);
}
