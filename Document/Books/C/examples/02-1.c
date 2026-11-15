/* Original teaching example; not compiled or executed here. */
#include <stdio.h>

int main(void)
{
    int first = 80;
    int second = 95;
    int total = first + second;
    double wrong = total / 2;
    double mean = total / 2.0;
    printf("total=%d wrong=%.1f mean=%.1f\n", total, wrong, mean);
    printf("int uses %zu C bytes\n", sizeof(int));
    return 0;
}
