#include "../include/mnist.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int load_mnist_data(const char *filename, int **x_train, int **y_train, int n_samples, int n_features) {

    FILE *file = fopen(filename, "rb");
    if (!file) return -1;

    *x_train = malloc(n_samples * n_features * sizeof(int));
    *y_train = malloc(n_samples * sizeof(int));

    if (!*x_train || !*y_train) return -2;

    for (int i = 0; i < n_samples; i++) {
        for (int j = 0; j < n_features; j++) {
            uint8_t temp;
            if (fread(&temp, 1, 1, file) != 1) return -3;
            (*x_train)[i * n_features + j] = (int)temp;
        }

        uint32_t label;
        if (fread(&label, sizeof(uint32_t), 1, file) != 1) return -4;
        (*y_train)[i] = (int)label;
    }

    fclose(file);
    return 0;
}
