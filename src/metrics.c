

double accuracy_score(int *y_hat, int *y, int n_instances) {
    if (n_instances <= 0) return 0.0;
    
    int correct = 0;
    for (int i = 0; i < n_instances; i++) {
        if (y_hat[i] == y[i]) {
            correct++;
        }
    }
    
    return (double)correct / n_instances;
}

