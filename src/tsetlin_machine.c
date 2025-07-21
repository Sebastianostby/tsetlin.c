#include <stdio.h>
#include <stdlib.h>
#include "../include/tsetlin_machine.h"
#include "../include/rng.h"
#include "../include/perf_counter.h"


TsetlinMachine* allocate_memory(int num_clauses, int num_classes, int num_literals, int threshold, float s, int seed) 
{

    // Still not defining seed for random stuff!!

    TsetlinMachine* tm = malloc(sizeof(TsetlinMachine));

    tm->num_clauses = num_clauses;
    tm->num_classes = num_classes;
    tm->num_literals = num_literals;
    tm->threshold = threshold;
    tm->s = s;
    
    tm->s_min_inv = (s-1)/s;
    tm->s_min =  1/s;
    

    // create C matrix
    tm->C = malloc(num_clauses * sizeof(int*));
    for (int i = 0; i < num_clauses; i++)
    {
        tm->C[i] = malloc((2*num_literals) * sizeof(int));

        for (int j = 0; j < (2*num_literals); j++)
        {
            tm->C[i][j] = 0;
        }
        

    }
    
    // create W matrix, are we going to have problems with this is just int8? need int32
    tm->W = malloc(num_classes * sizeof(int*));
    for (int i = 0; i < num_classes; i++)
    {
        tm->W[i] = malloc(num_clauses * sizeof(int));

        for (int j = 0; j < num_clauses; j++)
        {
            if (random() > 0.5) tm->W[i][j] = 1; else tm->W[i][j] = -1; // randomly set init weight to 1 or -1
        }
        
    }
    

    return tm;

}


void train(TsetlinMachine* tm, int **X, int *y, int epochs) 
{

    printf("Training TM for %d epochs...\n", epochs);

    float train_acc = 0.0;
    float eval_acc = 0.0;
    float best_eval_acc = 0.0;

    for (int epoch = 0; epoch < epochs; epoch++)
    {
        double st = perf_counter();
        // train and update TM
        double et = perf_counter();
        
        printf("[%d/%d] Train Time: %fs, Train Acc: %f%%, Eval Acc: %f%%, Best Eval Acc: %f%%\n", epoch, epochs, et-st, train_acc, eval_acc, best_eval_acc);
    }
    

}