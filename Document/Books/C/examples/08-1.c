/* Original teaching example; not compiled or executed here. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int grow(int **owner, size_t count)
{
    if (count == 0 || count > SIZE_MAX / sizeof **owner) {
        return 0;
    }
    int *next = realloc(*owner, count * sizeof **owner);
    if (next == NULL) {
        return 0;
    }
    *owner = next;
    return 1;
}

int main(void)
{
    int *scores = NULL;
    if (!grow(&scores, 3)) {
        fputs("allocation failed\n", stderr);
        return 1;
    }
    scores[0] = 80;
    scores[1] = 95;
    scores[2] = 70;
    if (!grow(&scores, 6)) {
        free(scores);
        return 1;
    }
    scores[3] = 100;
    printf("%d %d\n", scores[0], scores[3]);
    free(scores);
    scores = NULL;
    return 0;
}
