#ifndef RNG_H
#define RNG_H

#include <stdint.h>

typedef struct {
    uint64_t state;
} FastRNG;

void fast_rng_init(FastRNG* rng, uint64_t seed);

float fast_random_prob(FastRNG* rng);
int fast_random_int(FastRNG* rng, int max_value);

float random_prob(void);
int random_int(int max_value);

uint64_t xorshift64(uint64_t* state);

#endif