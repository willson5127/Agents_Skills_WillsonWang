/* Original teaching example; not compiled or executed here. */
#include <limits.h>
#include <stdckdint.h>
#include <stdio.h>

int main(void)
{
    int result;
    if (ckd_add(&result, INT_MAX, 1)) {
        puts("overflow");
    } else {
        printf("%d\n", result);
    }
    return 0;
}
