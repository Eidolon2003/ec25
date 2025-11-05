#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFLEN 1<<10
#define NCYCLES 3

//#define TESTING

typedef struct {
	int a, b;
} mycomplex;

void dupcomplex(mycomplex *dest, mycomplex *src) {
	dest->a = src->a;
	dest->b = src->b;
}

// Writes the text representation of c into the buffer str
void complextostr(mycomplex *c, char *str) {
	snprintf(str, BUFLEN, "[%d,%d]", c->a, c->b);
}

// Parses the first complex from the input string
void strtocomplex(mycomplex *c, char *str) {
	char *begin = strchr(str, '[');
	char *delim = strchr(str, ',');
	c->a = atoi(begin+1);
	c->b = atoi(delim+1);
}

// [X1,Y1] * [X2,Y2] = [X1 * X2 - Y1 * Y2, X1 * Y2 + Y1 * X2]
void mulcomplex(mycomplex *dest, mycomplex *src) {
	mycomplex tmp;
	dupcomplex(&tmp, dest);
	dest->a = (tmp.a * src->a) - (tmp.b * src->b);
	dest->b = (tmp.a * src->b) + (src->a * tmp.b);
}

// [X1,Y1] / [X2,Y2] = [X1 / X2, Y1 / Y2]
void divcomplex(mycomplex *dest, mycomplex *src) {
	dest->a = dest->a / src->a;
	dest->b = dest->b / src->b;
}

// [X1,Y1] + [X2,Y2] = [X1 + X2, Y1 + Y2]
void addcomplex(mycomplex *dest, mycomplex *src) {
	dest->a = dest->a + src->a;
	dest->b = dest->b + src->b;
}

int main() {
	FILE *f = fopen("../everybody_codes_e2025_q02_p1.txt", "r");
	if (!f) {
		puts("Failed to open file");
		return 1;
	}

	// Allocate a buffer, read the file into that buffer, zero terminate
	char buf[BUFLEN];
	size_t bytes_read = fread(buf, 1, BUFLEN, f);
	buf[bytes_read] = 0;
	puts(buf);

	// Initialize the value A from the input
	mycomplex a;
	strtocomplex(&a, buf);

#ifdef TESTING
	a.a = 25;
	a.b = 9;
#endif

	// Initialize the value x to [0,0]
	mycomplex r;
	r.a = r.b = 0;

	// Initialze [10,10] for later
	mycomplex ten;
	ten.a = ten.b = 10;

	for (size_t i = 0; i < NCYCLES; i++) {
		mycomplex tmp;	// Need this to avoid aliasing between the two operands

#ifdef TESTING
		printf("start cycle %zu\n", i);
#endif

		// R = R * R
		dupcomplex(&tmp, &r);
		mulcomplex(&r, &tmp);

#ifdef TESTING
		complextostr(&r, buf);
		printf("R = %s\n", buf);
#endif

		// R = R / 10
		divcomplex(&r, &ten);

#ifdef TESTING
		complextostr(&r, buf);
		printf("R = %s\n", buf);
#endif

		// R = R + A
		addcomplex(&r, &a);

#ifdef TESTING
		complextostr(&r, buf);
		printf("R = %s\n", buf);
#endif
	}

	complextostr(&r, buf);
	printf("Final R = %s\n", buf);
}
