/* Original teaching example; not compiled or executed here. */
#include <stdio.h>
#include <stdlib.h>

static int compare_int(const void *left, const void *right)
{
    int a = *(const int *)left;
    int b = *(const int *)right;
    return (a > b) - (a < b);
}

int main(void)
{
    int a[] = {95, 70, 80, 70};
    size_t n = sizeof a / sizeof a[0];
    qsort(a, n, sizeof a[0], compare_int);
    for (size_t i = 0; i < n; ++i) {
        printf("%d%s", a[i], i + 1 == n ? "\n" : " ");
    }
    return 0;
}
