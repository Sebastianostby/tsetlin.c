


#ifndef CORE_H
#define CORE_H

void dot(int *clause_outputs, int **W, int *vote_values, int num_classes, int num_clauses);
void clip(int *vote_values, int *vote_values_clamped, int num_classes, int threshold);

#endif 
