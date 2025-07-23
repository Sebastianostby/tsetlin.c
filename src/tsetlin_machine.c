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


    TsetlinMachine* tm = malloc(sizeof(TsetlinMachine));
    
    fast_rng_init(&tm->rng, seed);

    tm->num_clauses = num_clauses;
    tm->num_classes = num_classes;
    tm->num_literals = num_literals;
    tm->threshold = threshold;
    tm->s = s;
    
    tm->s_min_inv = (float)(s-1)/(float)s;
    tm->s_inv =  1.0f/(float)s;
    
    tm->C = malloc(num_clauses * 2 * num_literals * sizeof(int));
    memset(tm->C, 0, num_clauses * 2 * num_literals * sizeof(int));

    tm->W = malloc(num_classes * num_clauses * sizeof(int));

    for (int i = 0; i < num_classes * num_clauses; i++) {
        tm->W[i] = (fast_random_prob(&tm->rng) > 0.5) ? 1 : -1;
    }
    

    return tm;

}


void train(TsetlinMachine* tm, int **x_train, int *y_train, int **x_eval, int *y_eval, int epochs) 
{
    int n_instances = 0;
    while (x_train[n_instances] != NULL) n_instances++;

    int n_instances_eval = 0;
    while (x_eval[n_instances_eval] != NULL) n_instances_eval++;

    printf("Training TM for %d epochs...\n", epochs);

    float train_acc = 0.0;
    float eval_acc = 0.0;
    float best_eval_acc = 0.0;

    int y_hat[n_instances];
    int y_pred[n_instances_eval];


    int clause_outputs[tm->num_clauses];
    int vote_values[tm->num_classes];
    int vote_values_clamped[tm->num_classes];
    
    int clause_outputs_eval[tm->num_clauses];
    int vote_values_eval[tm->num_classes];

    double st_train = perf_counter();
    for (int epoch = 0; epoch < epochs; epoch++)
    {
        double st = perf_counter();
        
        memset(y_hat, 0, n_instances * sizeof(int));

        for (int i = 0; i < n_instances; i++)
        {

            int *instance = x_train[i];  
            int target = y_train[i];

            for (int j = 0; j < tm->num_clauses; j++) clause_outputs[j] = 1; 
            memset(vote_values, 0, tm->num_classes * sizeof(int));
            memset(vote_values_clamped, 0, tm->num_classes * sizeof(int));

            double st_eval = perf_counter();
            evaluate_clauses_training(tm->C, clause_outputs, instance, tm->num_literals, tm->num_clauses);
            double et_eval = perf_counter();

            double st_dot = perf_counter();
            dot(clause_outputs, tm->W, vote_values, tm->num_classes, tm->num_clauses); 
            clip(vote_values, vote_values_clamped, tm->num_classes, tm->threshold); 
            double et_dot = perf_counter();


            int not_target = (target + 1 + fast_random_int(&tm->rng, tm->num_classes - 1)) % tm->num_classes;
            
            int v_clamped_pos = vote_values_clamped[target];

            float pos_update_p = (tm->threshold - v_clamped_pos) / (2.0f *tm->threshold); 
            
            int v_clamped_neg = vote_values_clamped[not_target];

            float neg_update_p =  (tm->threshold + v_clamped_neg) / (2.0f *tm->threshold);
            
            double st_update = perf_counter();
            update_clauses(tm->C, tm->W, clause_outputs, instance, tm->num_literals, target, not_target, tm->num_clauses, pos_update_p, neg_update_p, tm->s_min_inv, tm->s_inv, &tm->rng);
            double et_update = perf_counter();

            // if (i % 100 == 0) {
            //     printf("Eval: %fs, Dot: %fs, Update: %fs\n", 
            //     et_eval-st_eval, et_dot-st_dot, et_update-st_update);
            //     }

            y_hat[i] = argmax(vote_values, tm->num_classes);

        }
        
        double et = perf_counter();
        
        train_acc = accuracy_score(y_hat, y_train, n_instances);

        // Eval pred
        memset(y_pred, 0, n_instances_eval * sizeof(int));
        for (int i = 0; i < n_instances_eval; i++)
        {
            for (int j = 0; j < tm->num_clauses; j++) clause_outputs_eval[j] = 1; 
            memset(vote_values_eval, 0, tm->num_classes * sizeof(int));
            
            evaluate_clauses(tm->C, clause_outputs_eval, x_eval[i], tm->num_literals, tm->num_clauses);
            dot(clause_outputs_eval, tm->W, vote_values_eval, tm->num_classes, tm->num_clauses); 

            y_pred[i] = argmax(vote_values_eval, tm->num_classes);
        }

        eval_acc = accuracy_score(y_pred, y_eval, n_instances_eval);

        if (eval_acc > best_eval_acc)
        {
            best_eval_acc = eval_acc;
        }
        

        printf("[%d/%d] Train Time: %fs, Train Acc: %f%%, Eval Acc: %f%%, Best Eval Acc: %f%%\n", epoch+1, epochs, et-st, train_acc, eval_acc, best_eval_acc);
    }
    double et_train = perf_counter();

    printf("Total train time: %f", et_train-st_train);
    
}