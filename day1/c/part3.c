#include <stdio.h>
#include <stdlib.h>

int main() {
	FILE *f = fopen("../everybody_codes_e2025_q01_p3.txt", "r");
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

	char *names[1024];
	names[0] = line;

	size_t max_index = 0;
	for (size_t i = 0; i < len; i++) {
		if (line[i] == ',') {
			max_index++;
			names[max_index] = line + i + 1;
		}
	}	

	// Skip blank line
	fgetc(f);
	
	char *command;
	size_t command_len;
	while ((bytes_read = getdelim(&command, &command_len, ',', f)) != -1) {
		switch (*command) {
		case 'L': {
			int offset = atoi(command+1) % (max_index+1);
			int swap = max_index - offset + 1;
			char *tmp = names[0];
			names[0] = names[swap];
			names[swap] = tmp;
		}	break;

		case 'R': {
			int offset = atoi(command+1) % (max_index+1);
			char *tmp = names[0];
			names[0] = names[offset];
			names[offset] = tmp;
		}	break;

		default:
			puts("unreachable");
			return 1;	
		}
	}

	for (char *c = names[0]; c < line + len; c++) {
		if (*c == ',') {
			*c = 0;
			break;
		}
	}

	printf("\nMy second parent's name is %s\n\n", names[0]);

	free(command);
	free(line);
	fclose(f);
}
