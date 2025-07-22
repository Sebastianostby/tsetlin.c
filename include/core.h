


#ifndef CORE_H
#define CORE_H



void evaluate_clauses_training(int **C, int *clause_outputs, int *literals, int num_literals, int num_clauses);
void update_clauses(int **C, int **W, int *clause_outputs, int *literals, int num_literals, int target, int not_target, int num_clauses, float pos_update_p, float neg_update_p);
void dot(int *clause_outputs, int **W, int *vote_values, int num_classes, int num_clauses);
void clip(int *vote_values, int *vote_values_clamped, int num_classes, int threshold);
int argmax(int *arr, int size);

#endif 
