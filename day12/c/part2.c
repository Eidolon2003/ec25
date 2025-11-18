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

int main() {
#ifdef TESTING
	FILE *f = fopen("../testp2.txt", "r");
#else
	FILE *f = fopen("../everybody_codes_e2025_q12_p2.txt", "r");
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


	size_t ans = part1(0, 0, 0);
	ans = part1(fieldX-1, fieldY-1, ans);
	printf("part2: %zu\n", ans);
}
