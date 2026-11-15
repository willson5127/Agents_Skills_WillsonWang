#include <stdio.h>
#include "stats.h"

int main(void)
{
    int scores[] = {70, 80, 90};
    printf("%.1f\n", stats_mean(scores, 3));
    return 0;
}
