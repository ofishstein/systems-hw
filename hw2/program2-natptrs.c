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
    int *ndigitorignptr = ndigitptr;

    nwhite = nother = 0;

    for (i = 0; i < 10; ++i) {
        *ndigitptr = 0;
        *ndigitptr++;
    }


    while (*x1ptr  != '\0') {
        if (*x1ptr >= '0' && *x1ptr <= '9') {

            ndigitptr = ndigitorignptr;

            for (i = 0; i < (*x1ptr - '0'); i++){
                *ndigitptr++;
            }
            ++*ndigitptr;

        } else if (*x1ptr == ' ' || *x1ptr == '\n' || *x1ptr == '\t') {
            ++nwhite;
        } else {
            ++nother;
        }

        *x1ptr++;
    }

    ndigitptr = ndigitorignptr;

    printf("digits:");
    for (i = 0; i < 10; ++i) {
        printf(" %d", *ndigitptr++);
    }

    printf(", white space: %d, other: %d\n", nwhite, nother);
    return 0;
}
