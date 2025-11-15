#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef TESTING
#define MOVES 3
#else
#define MOVES 4
#endif

#define MAX_ROWS 128

// Return 1 on hit, return 0 on miss or OOB
size_t indexBoard(char *board, size_t width, size_t height, int index) {
	int boardLen = (int)width*height;
	if (index >= boardLen || index < 0) return 0;
	else if (board[index] == 'S') {
		board[index] = 'X';	// Avoid double counting
		return 1;
	}
	return 0;
}

size_t part1(char *board, const size_t width, const size_t height, int dragon, size_t count, size_t depth) {
	if (depth == MOVES) return count;

	// There are 8 possible knight-moves to check
	const int moveOffsets[8] = {
		// Up and left
		-2*width-1,
		// Up and right
		-2*width+1,
		// Right and up
		-1*width+2,
		// Right and down
		1*width+2,
		// Down and right
		2*width+1,
		// Down and left
		2*width-1,
		// Left and down
		1*width-2,
		// Left and up
		-1*width-2		
	};

	for (size_t i = 0; i < 8; i++) {
		int nextPos = dragon+moveOffsets[i];
		count += indexBoard(board, width, height, nextPos);
		count = part1(board, width, height, nextPos, count, depth+1);
#ifdef TESTING
		printf("Depth %zu: Check pos %d\n", depth, nextPos);
#endif
	}

	return count;
}

int main() {
#ifdef TESTING
	FILE *f = fopen("../testp1.txt", "r");	// Expect 27
#else
	FILE *f = fopen("../everybody_codes_e2025_q10_p1.txt", "r");
#endif
	if (!f) {
		puts("Couldn't open file");
		return 1;
	}

	char *rows[MAX_ROWS] = {0};
	size_t numRows = 0;
	size_t buflen = 0;
	while (getline(rows+numRows, &buflen, f) != -1) {
		if (++numRows == MAX_ROWS) {
			puts("Too many rows!");
			return 1;
		}
	}
	size_t boardWidth = strlen(rows[0])-1;

#ifdef TESTING
	printf("numRows = %zu\n", numRows);
	printf("boardWidth = %zu\n", boardWidth);
#endif

	// Copy the board into a flat array for conveniently knight-moving around
	char *board = malloc(numRows*boardWidth);
	for (size_t i = 0; i < numRows; i++) {
		memcpy(board+(i*boardWidth), rows[i], boardWidth);
	}

	size_t dragon = 0;
	for (; dragon < boardWidth*numRows; dragon++) {
		if (board[dragon] == 'D') break;
	}
	assert(dragon < boardWidth*numRows);

	size_t ans = part1(board, boardWidth, numRows, dragon, 0, 0);
	printf("part1: %zu\n", ans);


	free(board);
	for (size_t i = 0; i < numRows; i++) free(rows[i]);
	fclose(f);
}
