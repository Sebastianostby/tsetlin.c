


#ifndef CORE_H
#define CORE_H



void evaluate_clauses_training(int **C, int *clause_outputs, int *literals, int num_literals, int num_clauses);
void update_clauses(int **C, int **W, int *clause_outputs, int *literals, int num_literals, int target, int not_target, int num_clauses, float pos_update_p, float neg_update_p, float s_min_inv, float s_inv);
void update_clause(int **C, int **W, int target, const int *literals, int num_literals, const int *clause_output, int clause_k, int class_k, float s_min_inv, float s_inv);
void T1aFeedback(int **C, int clause_k, const int *literals, int num_literals, float s_min_inv, float s_inv);
void T1bFeedback(int **C, int clause_k, int num_literals, float s_inv);
void T2Feedback(int **C, int clause_k, const int *literals, int num_literals);
void dot(int *clause_outputs, int **W, int *vote_values, int num_classes, int num_clauses);
void clip(int *vote_values, int *vote_values_clamped, int num_classes, int threshold);
int argmax(int *arr, int size);

#endif 
