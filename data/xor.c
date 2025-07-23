#include "../include/xor.h"
#include "../include/rng.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


void get_xor(int N, int M, float noise_level, int **X_ptr, int **y_ptr, bool verbose) {

    
    // srand((unsigned int)time(NULL));

    int *X = malloc(N * M * sizeof(int));
    int *Y = malloc(N * sizeof(int));
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            X[i * M + j] = random_int(2);
        }
        // Access first two elements of row i
        Y[i] = X[i * M + 0] ^ X[i * M + 1];
        
        if (random_prob() < noise_level) Y[i] = !Y[i];
    }
    
    *X_ptr = X;
    *y_ptr = Y;

    // if (verbose) {
    //     printf("Generated %i instances of XOR ...\n", N);

    //     printf("X = [\n");
    //     for (int i = 0; i < N; i++) {
    //         printf("[");
    //         for (int j = 0; j < M; j++) {
    //             printf("%d", X[i][j]);
    //         }
    //         printf("]\n");


    //     }
    //     printf("]\n");

    //     printf("Y = [");
    //     for (int i = 0; i < N; i++) {
    //         printf("%d ", Y[i]);
    //     }
    //     printf("]\n");

    // }

}




