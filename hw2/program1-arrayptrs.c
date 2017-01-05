#include <stdio.h>
#define IN   1  /* inside a word */
#define OUT  0  /* outside a word */

/* count lines, words, and characters in hardcoded string */
/* array-style pointers*/

int main() {

	int nl, nw, nc, state, i;
	char x1[100] = "The quick brown fox jumped over the lazy dog.\n";
	char *x1ptr = x1;

	i = nl = nw = nc = 0;
	state = OUT;

	while(*(x1ptr + i) != '\0') {
		++nc;

		if (*(x1ptr + i)  == '\n') {
			++nl;
		}
		if (*(x1ptr + i)  == ' '|| *(x1ptr + i)  == '\n' || *(x1ptr + i)  == '\t') {
			state = OUT;
		} else if (state == OUT) {
			state = IN;
			++nw;
		}

		++i;
	}

	printf("lines: %d, words: %d, characters: %d\n", nl, nw, nc);
	return 0;
}