#include "../include/xor.h"
#include "../include/rng.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


void get_xor(int N, int M, float noise_level, int ***X_ptr, int **y_ptr, bool verbose) {

    
    // srand((unsigned int)time(NULL));

    int **X = malloc(N * sizeof(int*));   // Allocate array of pointers for X
    int *Y = malloc(N * sizeof(int));

    
    
    for (int i = 0; i < N; i++)
    {
        X[i] = malloc(2 * sizeof(int));
        
        for (int j = 0; j < M; j++)
        {
            X[i][j] = random_int(2); // creates a number from 0 to 1, 2 excluded.
        }
        
        Y[i] = X[i][0] ^ X[i][1];
        
        if (random_prob() < noise_level) Y[i] = !Y[i]; // flip the label to add noise based on fraction.
        

    }
    
    *X_ptr = X;
    *y_ptr = Y;

    if (verbose) {
        printf("Generated %i instances of XOR ...\n", N);

        printf("X = [\n");
        for (int i = 0; i < N; i++) {
            printf("[");
            for (int j = 0; j < M; j++) {
                printf("%d", X[i][j]);
            }
            printf("]\n");


        }
        printf("]\n");

        printf("Y = [");
        for (int i = 0; i < N; i++) {
            printf("%d ", Y[i]);
        }
        printf("]\n");

    }

}




