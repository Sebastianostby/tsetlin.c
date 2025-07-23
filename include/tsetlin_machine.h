#ifndef TSETLIN_MACHINE_H
#define TSETLIN_MACHINE_H

#include "rng.h"

typedef struct {
    int num_clauses;
    int num_classes;
    int num_literals;
    int threshold;
    float s;

    int seed;

    float s_min_inv;
    float s_inv;

    int *C;
    int *W;

    FastRNG rng;

} TsetlinMachine;

TsetlinMachine* allocate_memory(int num_clauses, int num_classes, int num_literals, int threshold, float s, int seed);
void train(TsetlinMachine* tm, int **x_train, int *y_train, int **x_eval, int *y_eval, int epochs);

#endif


                                                                                        