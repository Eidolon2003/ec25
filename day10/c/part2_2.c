// An interative approach that finishes part2 quickly

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

void swapptr(char **a, char **b) {
	char *tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

void nextMoves(char *dest, char *src, size_t width, size_t len) {
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

	memset(dest, 0, len);
	for (size_t i = 0; i < len; i++) {
		if (src[i] == 1) {
			for (size_t j = 0; j < 8; j++) {
				int pos = i + moveOffsets[j];
				if (pos < 0 || pos > (int)len) continue;
				dest[pos] = 1;
			}
		}
	}
}

size_t captureSheep(char *board, char *moves, char *covers, size_t len) {
	size_t captures = 0;
	for (size_t i = 0; i < len; i++) {
		if (moves[i] == 1 && board[i] == 'S' && covers[i] != '#') {
			board[i] = 'X';
			captures++;
		}
	}	
	return captures;
}

void moveSheep(char *dest, char *src, size_t width, size_t len) {
	memcpy(dest+width, src, len-width);
	memset(dest, 0, width);
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
	size_t boardLen = numRows*boardWidth;
	char *board = malloc(boardLen);
	for (size_t i = 0; i < numRows; i++) {
		memcpy(board+(i*boardWidth), rows[i], boardWidth);
	}

	size_t dragon = 0;
	for (; dragon < boardLen; dragon++) {
		if (board[dragon] == 'D') break;
	}
	assert(dragon < boardLen);

	// Hold an untouched copy of the board for checking covers
	char *covers = malloc(boardLen);
	memcpy(covers, board, boardLen);

	// Allocate a board-sized buffer for figuring mapping moves
	char *moves = calloc(boardLen, 1);
	moves[dragon] = 1;

	// Allocate a swap buffer for moves and sheep
	char *swap = malloc(boardLen);

	// Main loop:
	size_t total = 0;
	for (size_t i = 0; i < MOVES; i++) {
		// Compute the dragon's moves for this round
		nextMoves(swap, moves, boardWidth, boardLen);
		swapptr(&swap, &moves);

		// Check for captured sheep
		total += captureSheep(board, moves, covers, boardLen);

		// Move the sheep
		moveSheep(swap, board, boardWidth, boardLen);
		swapptr(&swap, &board);

		// Check again for captures
		total += captureSheep(board, moves, covers, boardLen);
	}
	
	printf("part2: %zu\n", total);
	
	free(board);
	free(covers);
	free(moves);
	free(swap);
	for (size_t i = 0; i < numRows; i++) free(rows[i]);
	fclose(f);
}
