#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	FILE *f = fopen("../everybody_codes_e2025_q04_p1.txt", "r");
	if (!f) {
		puts("Failed to open file");
		return 1;
	}
	
	char buf[16];
	fread(buf, 1, 16, f);
	int first_teeth = atoi(buf);
	

	fseek(f, -16, SEEK_END);
	fread(buf, 1, 16, f);
	char *last = strrchr(buf, '\n')+1;
	int last_teeth = atoi(last);

	int total_teeth = first_teeth * 2025;
	int final_rotations = total_teeth / last_teeth;
	printf("The final gear will rotate %d times\n", final_rotations);

	fclose(f);
}
