


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