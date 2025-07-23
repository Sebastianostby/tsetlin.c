#ifndef MNIST_H
#define MNIST_H

#include <stdint.h>
#include <stdio.h>

int load_mnist_data(const char *filename, int **x_train, int **y_train, int n_samples, int n_features);

#endif // MNIST_H