#include <stdio.h>

/* count digits, white space, others in hardcoded string */
/* array-style pointers */

int main()
{
    int i, nwhite, nother;
    char x1[100] = "The 25 quick brown foxes jumped over the 27 lazy dogs 17 times.";
    int ndigit[10];

    char *x1ptr = x1;
    int *ndigitptr = ndigit;

    nwhite = nother = 0;

    for (i = 0; i < 10; ++i) {
        *(ndigitptr + i) = 0;
    }

    i = 0;

    while (*(x1ptr + i) != '\0') {
        if (*(x1ptr + i) >= '0' && *(x1ptr + i) <= '9') {
            ++*(ndigitptr + (*(x1ptr + i)-'0'));
        } else if (*(x1ptr + i) == ' ' || *(x1ptr + i) == '\n' || *(x1ptr + i) == '\t') {
           ++nwhite;
        } else {
            ++nother;
        }

        ++i;
    }

    printf("digits:");
    for (i = 0; i < 10; ++i) {
        printf(" %d", *(ndigitptr + i));
    }

    printf(", white space: %d, other: %d\n", nwhite, nother);
    return 0;
}
