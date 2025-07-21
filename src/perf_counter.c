#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <time.h>
#include "../include/perf_counter.h"



double perf_counter() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1000000000.0;
}