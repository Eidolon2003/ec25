#include <stdio.h>
#include <stdlib.h>

int main() {
	FILE *f = fopen("../everybody_codes_e2025_q06_p2.txt", "r");
	if (!f) {
		puts("Couldn't open file");
		return 1;
	}

	char *line;
	size_t linelen;
	getline(&line, &linelen, f);

	int mentorsA = 0;
	int mentorsB = 0;
	int mentorsC = 0;
	int total = 0;
	for (size_t i = 0; i < linelen; i++) {
		if (line[i] == 'A') mentorsA++;
		else if (line[i] == 'a') total += mentorsA;

		else if (line[i] == 'B') mentorsB++;
		else if (line[i] == 'b') total += mentorsB;

		else if (line[i] == 'C') mentorsC++;
		else if (line[i] == 'c') total += mentorsC;
	}

	printf("Total number of pairs: %d\n", total);
}
