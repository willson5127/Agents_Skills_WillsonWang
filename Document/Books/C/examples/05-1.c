/* Original teaching example; not compiled or executed here. */
#include <stdio.h>

int main(void)
{
    int scores[] = {80, 95, 70};
    size_t n = sizeof scores / sizeof scores[0];
    int total = 0;
    for (size_t i = 0; i < n; ++i) {
        total += scores[i];
    }
    printf("n=%zu mean=%.2f\n", n, (double)total / n);
    return 0;
}
