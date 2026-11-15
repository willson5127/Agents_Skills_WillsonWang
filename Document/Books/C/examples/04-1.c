/* Original teaching example; not compiled or executed here. */
#include <stdio.h>

int is_pass(int score);

int main(void)
{
    int score = 75;
    printf("%d %d\n", is_pass(score), score);
    return 0;
}

int is_pass(int score)
{
    return score >= 60 && score <= 100;
}
