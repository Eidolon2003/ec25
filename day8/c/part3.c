#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define TESTING	//Expect 7

#ifdef TESTING
#define NUM_NAILS 8
#else
#define NUM_NAILS 256
#endif

typedef struct {
	int begin, end;
} LINE;

#define MAX_LINES 64<<10
static LINE lines[MAX_LINES];
static size_t numLines;

// Returns if point x is on side '0' or '1' of line l
int sideof(LINE *l, int x) {
	// transform the situation so that line l begins at 1
	// This allows you to determine if a point is "left" or "right" of the end point 
	// with a simple greater than or less than comparison
	// without having to worry about wrapping
	int rot = 1 - l->begin;
	
	LINE tl = {1, l->end+rot};
	if (tl.end > NUM_NAILS) tl.end -= NUM_NAILS;	// manual "mod" operation
	else if (tl.end < 1) tl.end += NUM_NAILS;
	
	int tx = x + rot;
	if (tx > NUM_NAILS) tx -= NUM_NAILS;
	else if (tx < 1) tx += NUM_NAILS;
	
	return tx < tl.end; 
}

int crosses(LINE *a, LINE *b) {
	if (a->begin == b->begin || a->end == b->begin ||
		a->begin == b->end || a->end == b->end
	) {
		return 0;
	}

	return sideof(a, b->begin) != sideof(a, b->end);
}

int main() {
	FILE *f = fopen("../everybody_codes_e2025_q08_p3.txt", "r");
	if (!f) {
		puts("Couldn't open file");
		return 1;
	}

	char *line;
	size_t lineBufferLen;
#ifdef TESTING
	lineBufferLen = 64<<10;
	line = malloc(lineBufferLen);
	strcpy(line, "1,5,2,6,8,4,1,7,3,6");
#else 
	getline(&line, &lineBufferLen, f);
#endif

	int prevn = atoi(line);
	char *nstr = strchr(line, ',') + 1;
	for (nstr = strtok(nstr, ","); nstr; nstr = strtok(NULL, ",")) {
		int n = atoi(nstr);

		if (numLines == MAX_LINES) {
			puts("Lines array out of space");
			return 1;
		}
		lines[numLines].begin = prevn;
		lines[numLines].end = n;
		numLines++;
		
		prevn = n;
	}

	size_t maxCut = 0;
	for (int b = 1; b <= NUM_NAILS; b++) {
		for (int e = b; e <= NUM_NAILS; e++) {
			LINE l = { b, e };

			size_t cuts = 0;
			for (size_t i = 0; i < numLines; i++) {
				if (crosses(&l, lines+i)) cuts++;

				// Account for splitting long-wise
				if ((lines[i].begin == b || lines[i].begin == e) &&
					(lines[i].end == b || lines[i].end == e)
				) {
					cuts++;
				}
			}

			if (cuts > maxCut) {
				maxCut = cuts;
				printf("(%d, %d) cuts %zu\n", b, e, cuts);
			}
		}
 	}

	printf("Max number of cuts: %zu\n", maxCut);
	
	free(line);
	fclose(f);
}
