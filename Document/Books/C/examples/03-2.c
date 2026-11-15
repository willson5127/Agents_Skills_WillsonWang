/* Original teaching example; not compiled or executed here. */
#include <stdio.h>

int main(void)
{
    int total = 0;
    int passes = 0;
    for (int i = 0; i < 5; ++i) {
        int score = 50 + i * 10;
        total += score;
        if (score >= 60) {
            ++passes;
        }
    }
    printf("total=%d passes=%d mean=%.1f\n", total, passes, total / 5.0);
    return 0;
}
