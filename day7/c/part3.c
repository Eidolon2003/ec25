#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

// Lookup table indexed by the first letter of each rule
static char *rules['z'+1];
static size_t lens['z'+1];

#define MAX_NAMES 32
typedef char NAME[12];
static NAME usedNames[MAX_NAMES];
static size_t usedLens[MAX_NAMES];
static size_t usedTotals[MAX_NAMES];
static size_t numNames;

static int validateChar(char c, char next) {
	char *rule = rules[(int)c];
	size_t len = lens[(int)c];

	for (size_t i = 0; i < len; i+=2) {
		if (rule[i] == next) return 1;
	}
	return 0;
}

static int validateName(char *name) {
	for (size_t i = 0; name[i+1]; i++) {
		if (!validateChar(name[i], name[i+1])) return 0;
	}
	return 1;
}

static size_t part3(char *name, size_t total) {
	NAME buf;
	stpncpy(buf, name, sizeof(buf));
	size_t nextIndex = strlen(name);
	char lastChar = name[nextIndex-1];
	char *rule = rules[(int)lastChar];
	size_t len = lens[(int)lastChar];

	if (nextIndex >= 11) return total;
	for (size_t i = 0; i < len; i+=2) {
		buf[nextIndex] = rule[i];
		//assert(validateName(buf));
		if (nextIndex >= 6) {
			//puts(buf);
			total++;
		}
		total = part3(buf, total);
	}

	return total;
}

int main() {
	FILE *f = fopen("../everybody_codes_e2025_q07_p3.txt", "r");
	//FILE *f = fopen("../p3test1.txt", "r"); // Expect 25
	//FILE *f = fopen("../p3test2.txt", "r"); // Expect 1154
	if (!f) {
		puts("Couldn't open file");
		return 1;
	}

	size_t buflen;
	char *names;
	ssize_t bytes = getline(&names, &buflen, f);

	char *line = NULL;
	while ((bytes=getline(&line, &buflen, f)) != -1) {
		// Need this because the last line of the file doesn't end in a new line
		if (line[bytes-1] == '\n') line[bytes-1] = 0;
		size_t index = line[0];
		rules[index] = line+4;
		lens[index] = bytes-4;
		line = NULL;
	}

	for (char *name = strtok(names, ",\n"); name; name = strtok(NULL, ",\n")) {
		if (validateName(name)) {
			// Check if this name doesn't overlap with any previous ones
			size_t len = strlen(name);
			for (size_t i = 0; i < numNames; i++) {
				// If the shorter name is equal to the first part of the longer, they overlap
				size_t lower = len < usedLens[i] ? len : usedLens[i];
				if (strncmp(usedNames[i], name, lower) == 0) {
					// If the new name is shorter, replace the old one
					if (len < usedLens[i]) {
						printf("Replace %s with %s\n", usedNames[i], name);
						usedLens[i] = len;
						usedNames[i][len] = 0;
						usedTotals[i] = part3(name, 0);
					}
					else {
						printf("Skip %s because %s\n", name, usedNames[i]);
					}
					goto nextname;
				}
			}

			printf("Check %s\n", name);
			strncpy(usedNames[numNames], name, sizeof(NAME));
			usedLens[numNames] = len;
			usedTotals[numNames] = part3(name, 0);
			numNames++;
		}
		else {
			printf("Skip %s\n", name);
		}
	nextname:
	}

	size_t total = 0;
	for (size_t i = 0; i < numNames; i++) {
		total += usedTotals[i];
		printf("%s: %zu\n", usedNames[i], usedTotals[i]);
	}
	printf("Total: %zu\n", total);
}
