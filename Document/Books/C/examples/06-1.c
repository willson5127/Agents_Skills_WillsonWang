/* Original teaching example; not compiled or executed here. */
#include <stdio.h>

static void swap(int *a, int *b)
{
    int saved = *a;
    *a = *b;
    *b = saved;
}

int main(void)
{
    int low = 70;
    int high = 95;
    swap(&low, &high);
    printf("%d %d\n", low, high);
    return 0;
}
