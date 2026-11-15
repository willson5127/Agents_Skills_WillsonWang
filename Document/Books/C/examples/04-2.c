/* Original teaching example; not compiled or executed here. */
#include <stdio.h>

static unsigned int sum_to(unsigned int n)
{
    if (n == 0) {
        return 0;
    }
    return n + sum_to(n - 1);
}

int main(void)
{
    printf("%u\n", sum_to(10));
    return 0;
}
