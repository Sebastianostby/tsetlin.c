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

    int *x_train;
    int *y_train;
    int *x_eval;
    int *y_eval;
    int n_instances;
    int n_instances_eval;

} TsetlinMachine;

TsetlinMachine* allocate_memory(int num_clauses, int num_classes, int num_literals, int threshold, float s, int seed);
void set_train_data(TsetlinMachine* tm, int *x_train, int *y_train, int n_instances);
void set_eval_data(TsetlinMachine* tm, int *x_eval, int *y_eval, int n_instances);
void train(TsetlinMachine* tm, int epochs);

#endif


                                                                                        