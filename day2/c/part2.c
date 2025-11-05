#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define BUFLEN 1<<10
#define GRIDSIZE 101
#define NCYCLES 100

//#define TESTING

static char buf[BUFLEN];

typedef struct {
	long long a, b;
} mycomplex;

void dupcomplex(mycomplex *dest, mycomplex *src) {
	dest->a = src->a;
	dest->b = src->b;
}

// Writes the text representation of c into the buffer str
void complextostr(mycomplex *c, char *str) {
	snprintf(str, BUFLEN, "[%lld,%lld]", c->a, c->b);
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

int shouldEngrave(mycomplex *c) {
	mycomplex r = {0, 0};
	mycomplex divisor = {100000, 100000};

	for (size_t i = 0; i < NCYCLES; i++) {
		mycomplex tmp;
		dupcomplex(&tmp, &r);
		mulcomplex(&r, &tmp);
		divcomplex(&r, &divisor);
		addcomplex(&r, c);
	}

	if (r.a > -1000000 && r.a < 1000000 &&
		r.b > -1000000 && r.b < 1000000)
	{
		return 1;
	}
	else return 0;
}

int main() {
	FILE *f = fopen("../everybody_codes_e2025_q02_p2.txt", "r");
	if (!f) {
		puts("Failed to open file");
		return 1;
	}

	// Read into buf
	size_t bytes_read = fread(buf, 1, BUFLEN, f);
	buf[bytes_read] = 0;
	puts(buf);

	// Initialize the value A from the input
	mycomplex a;
	strtocomplex(&a, buf);

#ifdef TESTING
	a.a = 35300;
	a.b = -64910;
#endif

	unsigned count = 0;
	mycomplex offset;
	mycomplex coord;
	for (size_t x = 0; x < GRIDSIZE; x++) {
		offset.a = x * 10;
		for (size_t y = 0; y < GRIDSIZE; y++) {
			offset.b = y * 10;
			dupcomplex(&coord, &a);
			addcomplex(&coord, &offset);
			if (shouldEngrave(&coord)) {
				count++;
				putchar('x');
			}
			else {
				putchar('.');
			}
		}
		putchar('\n');
	}

	printf("\nCount: %u\n", count);
}
