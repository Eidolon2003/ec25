#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Lookup table indexed by the first letter of each rule
static char *rules['z'+1];
static size_t lens['z'+1];

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

int main() {
	FILE *f = fopen("../everybody_codes_e2025_q07_p2.txt", "r");
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

	size_t index = 1;
	size_t sum = 0;
	for (char *name = strtok(names, ","); name; name = strtok(NULL, ",\n")) {
		if (validateName(name)) sum += index;
		index++;
	}

	printf("Sum: %zu\n", sum);
}
