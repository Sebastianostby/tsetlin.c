#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/tsetlin_machine.h"
#include "../include/rng.h"
#include "../include/perf_counter.h"
#include "../include/metrics.h"
#include "../include/core.h"


TsetlinMachine* allocate_memory(int num_clauses, int num_classes, int num_literals, int threshold, float s, int seed) 
{

    srand(seed);

    TsetlinMachine* tm = malloc(sizeof(TsetlinMachine));

    tm->num_clauses = num_clauses;
    tm->num_classes = num_classes;
    tm->num_literals = num_literals;
    tm->threshold = threshold;
    tm->s = s;
    
    tm->s_min_inv = (float)(s-1)/(float)s;
    tm->s_inv =  1.0f/(float)s;
    

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
    int n_instances = 0;
    while (X[n_instances] != NULL) n_instances++;

    printf("Training TM for %d epochs...\n", epochs);

    float train_acc = 0.0;
    float eval_acc = 0.0;
    float best_eval_acc = 0.0;

    int y_hat[n_instances];

    int clause_outputs[tm->num_clauses];
    int vote_values[tm->num_classes];
    int vote_values_clamped[tm->num_classes];


    for (int epoch = 0; epoch < epochs; epoch++)
    {
        double st = perf_counter();
        
        memset(y_hat, 0, n_instances * sizeof(int));

        for (int i = 0; i < n_instances; i++)
        {

            int *instance = X[i];  
            int target = y[i];

            for (int j = 0; j < tm->num_clauses; j++) clause_outputs[j] = 1; 
            memset(vote_values, 0, tm->num_classes * sizeof(int));
            memset(vote_values_clamped, 0, tm->num_classes * sizeof(int));

            evaluate_clauses_training(tm->C, clause_outputs, instance, tm->num_literals, tm->num_clauses);
        
            dot(clause_outputs, tm->W, vote_values, tm->num_classes, tm->num_clauses); // When i printed the vote values, they were WERT LARGE!! Is this error?
            clip(vote_values, vote_values_clamped, tm->num_classes, tm->threshold); // Clamp worked!

            int not_target = (target + 1 + randint(tm->num_classes - 1)) % tm->num_classes;
            
            int v_clamped_pos = vote_values_clamped[target];

            float pos_update_p = (tm->threshold - v_clamped_pos) / (2.0f *tm->threshold); // make sure that the probs are floats
            
            int v_clamped_neg = vote_values_clamped[not_target];

            float neg_update_p =  (tm->threshold + v_clamped_neg) / (2.0f *tm->threshold);
            
            update_clauses(tm->C, tm->W, clause_outputs, instance, tm->num_literals, target, not_target, tm->num_clauses, pos_update_p, neg_update_p, tm->s_min_inv, tm->s_inv);
            
            y_hat[i] = argmax(vote_values, tm->num_classes);

        }
        
        double et = perf_counter();
        
        train_acc = accuracy_score(y_hat, y, n_instances);

        printf("[%d/%d] Train Time: %fs, Train Acc: %f%%, Eval Acc: %f%%, Best Eval Acc: %f%%\n", epoch+1, epochs, et-st, train_acc, eval_acc, best_eval_acc);
    }
    

}