#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	FILE *f = fopen("../everybody_codes_e2025_q04_p2.txt", "r");
	if (!f) {
		puts("Failed to open file");
		return 1;
	}
	
	char buf[16];
	fread(buf, 1, 16, f);
	long long first_teeth = atoi(buf);
	

	fseek(f, -16, SEEK_END);
	fread(buf, 1, 16, f);
	char *last = strrchr(buf, '\n')+1;
	long long last_teeth = atoi(last);

	//int total_teeth = first_teeth * 2025;
	//int final_rotations = total_teeth / last_teeth;

	//final_rotations = (first_teeth*first_rotations) / last_teeth
	//last_teeth*final_rotations = first_teeth*first_rotations
	//first_rotations = (last_teeth*final_rotations) / first_teeth
	long long first_rotations = (last_teeth * 10000000000000) / first_teeth;
	printf("The first gear must rotate %lld times\n", first_rotations+1);

	fclose(f);
}
