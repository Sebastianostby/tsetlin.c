#include <stdio.h>
#include "../include/rng.h"


void evaluate_clauses_training(int **C, int *clause_outputs, int *literals, int num_literals, int num_clauses) 
{

    for (int clause_k = 0; clause_k < num_clauses; clause_k++)
    {

        for (int literal_k = 0; literal_k < num_literals; literal_k++)
        {
            if (C[clause_k][literal_k] > 0)
            {
                if (literals[literal_k] == 0)
                {
                    clause_outputs[clause_k] = 0;
                    break;
                }
                
            }

            if (C[clause_k][literal_k + num_literals] > 0) 
            {
                if (literals[literal_k] == 1)
                {
                    clause_outputs[clause_k] = 0;
                    break;
                }
            }

        }
        
    }
    
}


void update_clauses(int **C, int **W, int *clause_outputs, int *literals, int num_literals, int target, int not_target, int num_clauses, float pos_update_p, float neg_update_p)
{

    for (int clause_k = 0; clause_k < num_clauses; clause_k++)
    {
        if (random() <= pos_update_p)
        {
            /* code */
        }

        if (random() <= neg_update_p)
        {
            /* code */
        }
            
    }
    
}



void dot(int *clause_outputs, int **W, int *vote_values, int num_classes, int num_clauses) 
{
    for (int i = 0; i < num_classes; i++) {
        for (int j = 0; j < num_clauses; j++) {
            vote_values[i] += clause_outputs[j] * W[i][j];
        }
    }
}

void clip(int *vote_values, int *vote_values_clamped, int num_classes, int threshold)
{
    for (int i = 0; i < num_classes; i++) {
       if (vote_values[i] < -threshold) {
           vote_values_clamped[i] = -threshold;
       } else if (vote_values[i] > threshold) {
           vote_values_clamped[i] = threshold;
       }
   }
}


int argmax(int *arr, int size) {
    if (size <= 0) return -1;
    
    int max_index = 0;
    double max_value = arr[0];
    
    for (int i = 1; i < size; i++) {
        if (arr[i] > max_value) {
            max_value = arr[i];
            max_index = i;
        }
    }
    
    return max_index;
}