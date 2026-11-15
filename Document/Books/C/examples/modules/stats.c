#include "stats.h"

double stats_mean(const int *a, size_t n)
{
    double total = 0.0;
    for (size_t i = 0; i < n; ++i) {
        total += a[i];
    }
    return total / n;
}
