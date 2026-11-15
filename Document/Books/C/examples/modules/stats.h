#ifndef CBOOK_STATS_H
#define CBOOK_STATS_H
#include <stddef.h>
/* a points to n readable ints; n > 0; result is their mean. */
double stats_mean(const int *a, size_t n);
#endif
