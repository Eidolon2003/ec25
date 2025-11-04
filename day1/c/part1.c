#include <stdio.h>
#include <stdlib.h>

int main() {
	FILE *f = fopen("../everybody_codes_e2025_q01_p1.txt", "r");
	if (f == NULL) {
		puts("Couldn't open file");
		return 1;
	}

	char *line;
	size_t len;
	ssize_t bytes_read;
	bytes_read = getline(&line, &len, f);
	if (bytes_read <= 0) {
		puts("Failed reading file");
		return 1;
	}

	puts(line);

	size_t max_index = 0;
	for (size_t i = 0; i < len; i++) {
		if (line[i] == ',') max_index++;
	}

	// Skip blank line
	fgetc(f);

	size_t index = 0;
	char *command;
	size_t command_len;
	while ((bytes_read = getdelim(&command, &command_len, ',', f)) != -1) {
		switch (*command) {
		case 'L': {
			int offset = atoi(command+1);
			index -= offset;
			if (index > max_index) index = 0;
		}	break;

		case 'R': {
			int offset = atoi(command+1);
			index += offset;
			if (index > max_index) index = max_index;
		}	break;

		default:
			puts("unreachable");
			return 1;	
		}

		printf("index after %s: %zu\n", command, index);
	}

	size_t comma_count = 0;
	char *name = NULL;
	for (size_t i = 0; i < len; i++) {
		if (comma_count == index && name == NULL) {
			name = line + i;
		}
		if (line[i] == ',') {
			comma_count++;
		}
		if (comma_count == index+1) {
			line[i] = 0;
		}
	}

	printf("\nMy name is %s\n\n", name);

	free(command);
	free(line);
	fclose(f);
}
