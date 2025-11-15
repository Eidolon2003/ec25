// This works for the test case, but 20 moves deep is too much for this algorithm
// I let it run for 10 minutes before I gave up on the poor thing :(

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef TESTING
#define MOVES 3
#else
#define MOVES 20
#endif

#define MAX_ROWS 128

// Return number of hits, -1 on OOB
// Change for part2: pass the depth to this function to allow for sheep moving
ssize_t indexBoard(char *board, size_t width, size_t height, int index, size_t depth) {
	size_t count = 0;
	int boardLen = (int)width*height;

	// The dragon still can't go off the edge of the board
	if (index >= boardLen || index < (int)(depth*width)) return -1;

	// Check if there's a cover under the dragon's "real" position
	if (board[index] == '#') return 0;

	// Move the dragon up to account for the sheep moving down
	index -= depth*width;

	// stop recursion if you hit a spot another dragon has already been
	// to save on repeated computation
	if (board[index] == 'X') {
		return -1;
	}
	else if (board[index] == 'S') {
		board[index] = 'X';	// Avoid double counting
		count++;
	}

	// Move the dragon up one more row to account for sheep moving into it
	index -= width;
	if (index < 0) return count;
	if (board[index] == 'S') {
		board[index] = 'X';
		count++;
	}
	
	return count;
}

size_t part2(char *board, const size_t width, const size_t height, int dragon, size_t count, size_t depth) {
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
		ssize_t x = indexBoard(board, width, height, nextPos, depth);
		if (x != -1) {
			count += x;
			count = part2(board, width, height, nextPos, count, depth+1);
		}
		else {
#ifdef TESTING
		puts("skip");
#endif
		}
#ifdef TESTING
		printf("Depth %zu: Check pos %d\n", depth, nextPos);
#endif
	}

	return count;
}

int main() {
#ifdef TESTING
	FILE *f = fopen("../testp2.txt", "r");	// Expect 27
#else
	FILE *f = fopen("../everybody_codes_e2025_q10_p2.txt", "r");
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

	size_t ans = part2(board, boardWidth, numRows, dragon, 0, 0);
	printf("part2: %zu\n", ans);


	free(board);
	for (size_t i = 0; i < numRows; i++) free(rows[i]);
	fclose(f);
}
