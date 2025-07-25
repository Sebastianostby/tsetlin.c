#include <stdio.h>
#include <stdbool.h>
#include <omp.h>
#include "../include/rng.h"
#include "../include/core.h"

typedef struct {
    FastRNG rng;
} ThreadRNG;

void evaluate_clauses_training(int *C, int *clause_outputs, const int *literals, int num_literals, int num_clauses) 
{
    #pragma omp parallel for schedule(static) num_threads(omp_get_num_procs() - 1)
    for (int clause_k = 0; clause_k < num_clauses; clause_k++)
    {
        const int clause_offset = clause_k * (2 * num_literals);
        
        for (int literal_k = 0; literal_k < num_literals; literal_k++)
        {
            if (C[clause_offset + literal_k] > 0)
            {
                if (literals[literal_k] == 0)
                {
                    clause_outputs[clause_k] = 0;
                    break;
                }
            }

            if (C[clause_offset + literal_k + num_literals] > 0) 
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


void evaluate_clauses(int *C, int *clause_outputs, int *literals, int num_literals, int num_clauses) 
{
    #pragma omp parallel for schedule(static) num_threads(omp_get_num_procs() - 1)
    for (int clause_k = 0; clause_k < num_clauses; clause_k++)
    {
        bool is_empty_clause = true;

        for (int literal_k = 0; literal_k < num_literals; literal_k++)
        {
            if (C[clause_k * (2 * num_literals) + literal_k] > 0)
            {
                is_empty_clause = false;

                if (literals[literal_k] == 0)
                {
                    clause_outputs[clause_k] = 0;

                    break;
                }
                
            }

            if (C[clause_k * (2 * num_literals) + literal_k + num_literals] > 0) 
            {
                is_empty_clause = false;

                if (literals[literal_k] == 1)
                {
                    clause_outputs[clause_k] = 0;

                    break;
                }
            }

        }

        if (is_empty_clause == true)
        {
            clause_outputs[clause_k] = 0;
        }
        

    }


}

void update_clauses(int *C, int *W, int *clause_outputs, int *literals, int num_literals, int target, int not_target, int num_clauses, float pos_update_p, float neg_update_p, float s_min_inv, float s_inv, FastRNG* rng)
{
    
    
    #pragma omp parallel num_threads(omp_get_num_procs() - 1)
    {

        FastRNG thread_rng;
        fast_rng_init(&thread_rng, rng->state + omp_get_thread_num() * 1000000);

        #pragma omp for schedule(static)
        for (int clause_k = 0; clause_k < num_clauses; clause_k++)
        {   
            if (fast_random_prob(&thread_rng) <= pos_update_p)
            {
                update_clause(C, W, 1, literals, num_literals, clause_outputs, num_clauses, clause_k, target, s_min_inv, s_inv, &thread_rng);
            }
    
            if (fast_random_prob(&thread_rng) <= neg_update_p)
            {
                update_clause(C, W, -1, literals, num_literals, clause_outputs, num_clauses, clause_k, not_target, s_min_inv, s_inv, &thread_rng);
            }
                
        }
        
    }
    
    
}


void update_clause(int *C, int *W, int target, const int *literals, int num_literals, const int *clause_output, int num_clauses, int clause_k, int class_k, float s_min_inv, float s_inv, FastRNG* rng)
{

    const int clause_offset = class_k * num_clauses;

    int sign;
    if (W[clause_offset + clause_k] >= 0) sign = 1; else sign = -1;


    if (target * sign > 0)
    {
        if (clause_output[clause_k] == 1)
        {
            W[clause_offset + clause_k] += sign;
            T1aFeedback(C, clause_k, literals, num_literals, s_min_inv, s_inv, rng);
        }
        else
        {
            T1bFeedback(C, clause_k, num_literals, s_inv, rng);
        }
    }
    
    else if (target * sign < 0)
    {
        if (clause_output[clause_k] == 1)
        {
            W[clause_offset + clause_k] -= sign;
            T2Feedback(C, clause_k, literals, num_literals);
        }
        
    }
    
    
}

void T1aFeedback(int *C, int clause_k, const int *literals, int num_literals, float s_min_inv, float s_inv, FastRNG* rng) 
{
    const int upper_state = 127;
    const int lower_state = -127;
    const int clause_offset = clause_k * (2 * num_literals);

    for (int literal_k = 0; literal_k < num_literals; literal_k++)
    {
        if (literals[literal_k] == 1)
        {
            if(fast_random_prob(rng) <= s_min_inv)
            {
                if (C[clause_offset + literal_k] < upper_state)
                {
                    C[clause_offset + literal_k] ++;
                }
            }

            if (fast_random_prob(rng) <= s_inv)
            {
                if (C[clause_offset + literal_k + num_literals] > lower_state)
                {
                    C[clause_offset + literal_k + num_literals] --;
                }
                
            }
            
        }
        else
        {
            if (fast_random_prob(rng) <= s_inv)
            {
                if (C[clause_offset + literal_k] > lower_state)
                {
                    C[clause_offset + literal_k] --;
                }
            }

            if(fast_random_prob(rng) <= s_min_inv)
            {
                if (C[clause_offset + literal_k + num_literals] < upper_state)
                {
                    C[clause_offset + literal_k + num_literals] ++;
                }
            }

        }
        
    }
    


}

void T1bFeedback(int *C, int clause_k, int num_literals, float s_inv, FastRNG* rng) 
{
    const int lower_state = -127;
    const int clause_offset = clause_k * (2 * num_literals);

    for (int literal_k = 0; literal_k < num_literals; literal_k++)
    {
        if (fast_random_prob(rng) <= s_inv)
        {
            if (C[clause_offset + literal_k] > lower_state)
            {
                C[clause_offset + literal_k] --;
            }
            
        }

        if (fast_random_prob(rng) <= s_inv)
        {
            if (C[clause_offset + literal_k + num_literals] > lower_state)
            {
                C[clause_offset + literal_k + num_literals] --;
            }
        }
        
    }
    

}

void T2Feedback(int *C, int clause_k, const int *literals, int num_literals)
{

    const int clause_offset = clause_k * (2 * num_literals);

    for (int literal_k = 0; literal_k < num_literals; literal_k++)
    {
        if (literals[literal_k] == 0)
        {
            if (C[clause_offset + literal_k] <= 0)
            {
                C[clause_offset + literal_k] ++;
            }
            
        }

        else
        {
            if (C[clause_offset + literal_k + num_literals] <= 0)
            {
                C[clause_offset + literal_k + num_literals] ++;
            }
        }
        
    }
    
}


void dot(int *clause_outputs, int *W, int *vote_values, int num_classes, int num_clauses) 
{
    for (int i = 0; i < num_classes; i++) {
        int sum = 0;
        int *W_row = &W[i * num_clauses];
        
        for (int j = 0; j < num_clauses; j++) {
            sum += clause_outputs[j] * W_row[j];
        }
        
        vote_values[i] += sum;
    }
}

void clip(int *vote_values, int *vote_values_clamped, int num_classes, int threshold)
{
    for (int class_k = 0; class_k < num_classes; class_k++) {
       if (vote_values[class_k] < -threshold) {
           vote_values_clamped[class_k] = -threshold;
       } else if (vote_values[class_k] > threshold) {
           vote_values_clamped[class_k] = threshold;
       }
       else {
           vote_values_clamped[class_k] = vote_values[class_k];
       }
   }
}


int argmax(int *arr, int size) {
    if (size <= 0) return -1;
    
    int max_index = 0;
    int max_value = arr[0]; 
    
    for (int i = 1; i < size; i++) {
        if (arr[i] > max_value) {
            max_value = arr[i];
            max_index = i;
        }
    }
    return max_index;
}