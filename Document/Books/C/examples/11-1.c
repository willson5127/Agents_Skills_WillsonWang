/* Original teaching example; not compiled or executed here. */
#include <assert.h>
#include <limits.h>
#include <stdio.h>

static int add_int(int a, int b, int *out)
{
    if ((b > 0 && a > INT_MAX - b) ||
        (b < 0 && a < INT_MIN - b)) {
        return 0;
    }
    *out = a + b;
    return 1;
}

int main(void)
{
    int result = 0;
    int ok = add_int(70, 30, &result);
    assert(ok && result == 100);
    ok = add_int(INT_MAX, 1, &result);
    assert(!ok && result == 100);
    printf("%d\n", result);
    return 0;
}
