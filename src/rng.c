#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/rng.h"

void fast_rng_init(FastRNG* rng, uint64_t seed) {
    rng->state = seed ? seed : 1;
}

float fast_random_prob(FastRNG* rng) {
    return (xorshift64(&rng->state) >> 40) * 0x1.0p-24f;
}

int fast_random_int(FastRNG* rng, int max_value) {
    return xorshift64(&rng->state) % max_value;
}

float random_prob(void) {
    return (float)rand() / (float)RAND_MAX;
}

int random_int(int max_value) {
    return rand() % max_value;
}

uint64_t xorshift64(uint64_t* state) {
    uint64_t x = *state;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    *state = x;
    return x;
}