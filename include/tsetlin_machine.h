#ifndef TSETLIN_MACHINE_H
#define TSETLIN_MACHINE_H

typedef struct {
    int num_clauses;
    int num_classes;
    int num_literals;
    int threshold;
    float s;

    int seed;

    float s_min_inv;
    float s_min;

    int **C;
    int **W;


} TsetlinMachine;

TsetlinMachine* allocate_memory(int num_clauses, int num_classes, int num_literals, int threshold, float s, int seed);



void train(TsetlinMachine* tm, int **X, int *y, int epochs);

#endif