#include <stdio.h>
#define IN   1  /* inside a word */
#define OUT  0  /* outside a word */

/* count lines, words, and characters in hardcoded string */
/* native pointers*/

int main() {

	int nl, nw, nc, state, i;
	char x1[100] = "The quick brown fox jumped over the lazy dog.\n";
	char *x1ptr = x1;

	nl = nw = nc = 0;
	state = OUT;

	while(*x1ptr != '\0') {
		++nc;

		if (*x1ptr  == '\n') {
			++nl;
		}
		if (*x1ptr == ' '|| *x1ptr  == '\n' || *x1ptr == '\t') {
			state = OUT;
		} else if (state == OUT) {
			state = IN;
			++nw;
		}

		*x1ptr++;
	}

	printf("lines: %d, words: %d, characters: %d\n", nl, nw, nc);
	return 0;
}