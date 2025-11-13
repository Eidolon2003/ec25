#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define TESTING	//Expect 21

#ifdef TESTING
#define NUM_NAILS 8
#else
#define NUM_NAILS 256
#endif

typedef struct {
	int begin, end;
} line;

#define MAX_LINES 64<<10
static line lines[MAX_LINES];
static size_t numLines;

// Returns if point x is on side '0' or '1' of line l
int sideof(line *l, int x) {
	// transform the situation so that line l begins at 1
	// This allows you to determine if a point is "left" or "right" of the end point 
	// with a simple greater than or less than comparison
	// without having to worry about wrapping
	int rot = 1 - l->begin;
	
	line tl = {1, l->end+rot};
	if (tl.end > NUM_NAILS) tl.end -= NUM_NAILS;	// manual "mod" operation
	else if (tl.end < 1) tl.end += NUM_NAILS;
	
	int tx = x + rot;
	if (tx > NUM_NAILS) tx -= NUM_NAILS;
	else if (tx < 1) tx += NUM_NAILS;
	
	return tx < tl.end; 
}

int main() {
	FILE *f = fopen("../everybody_codes_e2025_q08_p2.txt", "r");
	if (!f) {
		puts("Couldn't open file");
		return 1;
	}

	char *line;
	size_t lineBufferLen;
#ifdef TESTING
	lineBufferLen = 64<<10;
	line = malloc(lineBufferLen);
	strcpy(line, "1,5,2,6,8,4,1,7,3,5,7,8,2");
#else 
	getline(&line, &lineBufferLen, f);
#endif

	int prevn = atoi(line);
	char *nstr = strchr(line, ',') + 1;
	size_t count = 0;
	for (nstr = strtok(nstr, ","); nstr; nstr = strtok(NULL, ",")) {
		int n = atoi(nstr);

		for (size_t i = 0; i < numLines; i++) {
			// If two lines share a point, they can't cross
			if (n == lines[i].begin || n == lines[i].end ||
				prevn == lines[i].begin || prevn == lines[i].end
			) {
				continue;
			}

			if (sideof(lines+i, prevn) != sideof(lines+i, n)) {
				//printf("(%d,%d) crosses (%d,%d)\n", lines[i].begin, lines[i].end, prevn, n);
				count++;
			}
			else {
				//printf("(%d,%d) doesn't cross (%d,%d)\n", lines[i].begin, lines[i].end, prevn, n);
			}
		}

		// Add the new line
		if (numLines == MAX_LINES) {
			puts("Lines array out of space");
			return 1;
		}
		lines[numLines].begin = prevn;
		lines[numLines].end = n;
		numLines++;
		
		prevn = n;
	}

	printf("Number of string crossings: %zu\n", count);
	
	free(line);
	fclose(f);
}
