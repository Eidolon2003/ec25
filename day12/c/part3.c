#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FIELD_DIM 256

char field[FIELD_DIM][FIELD_DIM];
size_t fieldX = 0;
size_t fieldY = 0;

size_t part1(size_t x, size_t y, size_t c) {
#ifdef TESTING
	printf("x=%zu, y=%zu\n", x, y);
#endif
	char this = field[y][x];
	field[y][x] = (char)127;
	if (x < fieldX-1 && field[y][x+1] <= this) {
		c = part1(x+1, y, c);
	}
	if (y < fieldY-1 && field[y+1][x] <= this) {
		c = part1(x, y+1, c);
	}
	if (x > 0 && field[y][x-1] <= this) {
		c = part1(x-1, y, c);
	}
	if (y > 0 && field[y-1][x] <= this) {
		c = part1(x, y-1, c);
	}
	return c+1;
}

size_t part3() {
	size_t ans = 0;
	char backup[FIELD_DIM][FIELD_DIM];	// Backup of the current field state
	char best[FIELD_DIM][FIELD_DIM];	// The best found field so far
	
	for (size_t i = 0; i < 3; i++) {
		memcpy(backup, field, FIELD_DIM*FIELD_DIM);

		size_t greatest = 0;
		for (size_t y = 0; y < fieldY; y++) {
			for (size_t x = 0; x < fieldX; x++) {
				if (field[y][x] > '9') continue;
				
				size_t candidate = part1(x, y, 0);
				if (candidate > greatest) {
					memcpy(best, field, FIELD_DIM*FIELD_DIM);
					greatest = candidate;
				}
				memcpy(field, backup, FIELD_DIM*FIELD_DIM);	// Reset the field for the next test
			}		
		}
		ans += greatest;
		memcpy(field, best, FIELD_DIM*FIELD_DIM);
	}
	return ans;
}

int main() {
#ifdef TESTING
	FILE *f = fopen("../testp3.txt", "r");
#else
	FILE *f = fopen("../everybody_codes_e2025_q12_p3.txt", "r");
#endif
	if (!f) {
		puts("Couldn't open file");
		return 1;
	}
	
	char *line = NULL; 
	size_t buflen = 0;
	ssize_t bytes;
	while ((bytes = getline(&line, &buflen, f)) != -1) {
		fieldX = bytes;
		if (line[bytes-1] == '\n') fieldX--;	// Yet again the input not ending with \n causes issues
		memcpy(&field[fieldY++][0], line, fieldX);
	}
	free(line);

	printf("%zux%zu\n", fieldX, fieldY);
	for (size_t y = 0; y < fieldY; y++) {
		for (size_t x = 0; x < fieldX; x++) {
			putchar(field[y][x]);
		}
		putchar('\n');
	}
	putchar('\n');


	size_t ans = part3();
	printf("part3: %zu\n", ans);
}
