#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
	int in, out;
} gear;

#define MAX_GEARS 128
static gear gears[MAX_GEARS];
static size_t nextGear;

int main() {
	FILE *f = fopen("../everybody_codes_e2025_q04_p3.txt", "r");
	if (!f) {
		puts("Failed to open file");
		return 1;
	}

	// We actually have to read the whole file now...
	char *line;
	size_t linelen;
	ssize_t bytes_read = getline(&line, &linelen, f);
	int first_teeth = atoi(line) * 100;

	while ((bytes_read = getline(&line, &linelen, f)) != -1) {
		if (nextGear == MAX_GEARS) {
			puts("Array out of space");
			return 1;
		}

		char *second = strchr(line, '|');
		if (second) {
			gears[nextGear++] = (gear){
				.in = atoi(line),
				.out = atoi(second+1)
			};
		}
		else {
			gears[nextGear++] = (gear){
				.in = atoi(line),
				.out = 1
			};
		}
	}

	double turns = first_teeth;
	for (size_t i = 0; i < nextGear; i++) {
		//printf("%d | %d\n", gears[i].in, gears[i].out );
		turns = turns * (double)gears[i].out / (double)gears[i].in;
	}

	printf("The final gear makes %lld turns after 100 turns of the first\n", (long long)turns);

	fclose(f);
}
