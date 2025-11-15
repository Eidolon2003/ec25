#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <alloca.h>

// Based on https://github.com/exebook/hashdict.c
// Changed HASHDICT_VALUE_TYPE to size_t in header
// Had to fix enumFunc to take HASHDICT_VALUE_TYPE rather than int only
// Probably should submit a pull request for this...
#include "hashdict.h"

#define MAX_ROWS 128

static char *covers;
static size_t width;
static size_t height;
static size_t len;

static struct dictionary *cache;

typedef struct {
	size_t sheep;
	size_t dragon;
} STATE;

size_t rowof(size_t index) {
	return index/width;
}

char *hash(char *board, size_t dragon) {
	assert(len < 1024);
	static char buf[1024];
	memcpy(buf, board, len);
	(*(size_t*)buf) += dragon;
	return buf;
}

size_t sheepMove(char*, size_t, STATE);
size_t dragonMove(char *board, size_t count, STATE state) {
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

	const int rowOffsets[8] = {
		-2, -2, -1, 1, 2, 2, 1, -1
	};

	char *copy = alloca(len);
	memcpy(copy, board, len);
	
	// Check all possible dragon moves
	for (size_t i = 0; i < 8; i++) {
		STATE nextState = state;
		int nextDragon = state.dragon + moveOffsets[i];

		// Top bounds check
		if (nextDragon >= (int)len || nextDragon < 0) continue;
		// Side bounds check
		if ((rowof(state.dragon) + rowOffsets[i]) != rowof(nextDragon)) continue;
		nextState.dragon = nextDragon;
		
		// If the dragon is moving onto an uncovered sheep, capture it and decrement the counter
		if (copy[nextDragon] == 'S' && covers[nextDragon] != '#') {
			nextState.sheep--;
			if (nextState.sheep == 0) {
				// If there aren't any more sheep, stop the game and count it
				count++;
			}
			else {
				// Move onto the sheep
				copy[nextDragon] = 0;
				count += sheepMove(copy, 0, nextState);
				copy[nextDragon] = 'S';
			}
		}
		else {
			count += sheepMove(copy, 0, nextState);
		}
	}

	return count;
}

size_t sheepMove(char *board, size_t count, STATE state) {
	// Check the cache before anything else
	if (dic_find(cache, hash(board, state.dragon), len)) {
		return *cache->value;
	}

	char *copy = alloca(len);
	memcpy(copy, board, len);

	// Check all possible sheep moves
	size_t sheep = 0;
	size_t sheepMoves = 0;
	for (size_t i = 0; i < len && sheep < state.sheep; i++) {
		if (copy[i] != 'S') continue;
		sheep++;

		size_t next = i+width;
		// If the sheep can move off the edge, do it
		if (next >= len) {
			sheepMoves++;
			// In this case the game is over, a sheep escaped being eaten
			// Do not continue the recursion
		}
		// Otherwise, if the next space is covered, it's safe to move
		else if (covers[next] == '#') {
			sheepMoves++;
			// Check if the sheep is safe all the way to the bottom and exit early
			size_t test = next;
			for (; test < len; test+=width) {
				if (covers[test] != '#') break;
			}
			if (test < len) {
				copy[i] = 0;
				copy[next] = 'S';
				count += dragonMove(copy, 0, state);
				copy[next] = 0;
				copy[i] = 'S';
			}
		}
		// Otherwise if the next space is open and the dragon isn't there
		else if (next != state.dragon) {
			sheepMoves++;
			copy[i] = 0;
			copy[next] = 'S';
			count += dragonMove(copy, 0, state);
			copy[next] = 0;
			copy[i] = 'S';
		}
	}
	if (sheepMoves == 0) {
		count += dragonMove(copy, 0, state);
	}

	// Update cache before returning
	dic_add(cache, hash(board, state.dragon), len);
	return *cache->value = count;
//	return count;
}

int main() {
#ifdef TESTING
	FILE *f = fopen("../testp3.txt", "r");
#else
	FILE *f = fopen("../everybody_codes_e2025_q10_p3.txt", "r");
#endif
	if (!f) {
		puts("Couldn't open file");
		return 1;
	}

	char *rows[MAX_ROWS] = {0};
	size_t height = 0;
	size_t buflen = 0;
	while (getline(rows+height, &buflen, f) != -1) {
		if (++height == MAX_ROWS) {
			puts("Too many rows!");
			return 1;
		}
	}
	width = strlen(rows[0])-1;
	len = width*height;
	fclose(f);

#ifdef TESTING
	printf("numRows = %zu\n", height);
	printf("boardWidth = %zu\n", width);
#endif

	// Copy the board into a flat array for conveniently knight-moving around
	char *board = malloc(len);
	for (size_t i = 0; i < height; i++) {
		memcpy(board+(i*width), rows[i], width);
	}

	// Create an untouched copy of the board for checking covers
	covers = malloc(len);
	memcpy(covers, board, len);

	size_t dragon = 0;
	for (; dragon < len; dragon++) {
		if (board[dragon] == 'D') break;
	}
	assert(dragon < len);

	// The board just represents where the sheep are and nothing else
	size_t sheep = 0;
	for (size_t i = 0; i < len; i++) {
		if (board[i] == 'S') sheep++;
		else board[i] = 0;
	}

	// Initialize the cache
	cache = dic_new(0);

	size_t ans = sheepMove(board, 0, (STATE){sheep, dragon});
	printf("part3: %zu\n", ans);

	dic_delete(cache);
	free(board);
	free(covers);
	for (size_t i = 0; i < height; i++) free(rows[i]);
}
