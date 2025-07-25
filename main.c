#include <stdio.h>
#include <stdlib.h>
#include "include/xor.h"  // data
#include "include/mnist.h"  // data

#include "include/tsetlin_machine.h" // tm


void xor_experiment() {

    int N = 100000;
    int M = 10;
    int *X;
    int *Y;

    get_xor(N, M, 0.2, &X, &Y, false);

}


void mnist_experiment() {
    int *x_train;
    int *y_train;
    int *x_test;
    int *y_test;

    const char *filename_train = "data/RAW_FILES/mnist_train.bin";
    const char *filename_test = "data/RAW_FILES/mnist_test.bin";

    if (load_mnist_data(filename_train, &x_train, &y_train, 60000, 784) != 0) {
        printf("Failed to load MNIST Train data.\n");
        return;
    }

    if (load_mnist_data(filename_test, &x_test, &y_test, 10000, 784) != 0) {
        printf("Failed to load MNIST Test data.\n");
        return;
    }

    printf("First sample from train label: %d\n", y_train[0]);

    printf("Raw binary data:\n");
    for (int i = 0; i < 28; i++) {
        for (int j = 0; j < 28; j++) {
            printf("%d", x_train[i * 28 + j]);
        }
        printf("\n");
    }

}




void tm_experiment_xor() {

    int M = 6;
    int *x_train;
    int *y_train;
    int *x_eval;
    int *y_eval;

    get_xor(1000, M, 0.3, &x_train, &y_train, false);
    get_xor(200, M, 0.0, &x_eval, &y_eval, false);

    TsetlinMachine* tm = allocate_memory(100, 2, M, 500, 10.0, 42);
    printf("Clauses: %d\n", tm->num_clauses);
    printf("Classes: %d\n", tm->num_classes);  
    printf("Literals: %d\n", tm->num_literals);
    printf("Threshold: %d\n", tm->threshold);
    printf("s: %.1f\n", tm->s);

    set_train_data(tm, x_train, y_train, 1000);
    set_eval_data(tm, x_eval, y_eval, 200);

    train(tm, 1000);

    printf("C Matrix (%d clauses x %d literals):\n", tm->num_clauses, 2*tm->num_literals);
    for (int i = 0; i < tm->num_clauses; i++) {
        printf("Clause %3d: ", i);
        for (int j = 0; j < 2*tm->num_literals; j++) {
            printf("%2d ", tm->C[i * (2*tm->num_literals) + j]);
        }
        printf("\n");
    }
    printf("\n");

    // Print W matrix (contiguous)
    printf("W Matrix (%d classes x %d clauses):\n", tm->num_classes, tm->num_clauses);
    for (int i = 0; i < tm->num_classes; i++) {
        printf("Class %2d: ", i);
        for (int j = 0; j < tm->num_clauses; j++) {
            printf("%2d ", tm->W[i * tm->num_clauses + j]);
        }
        printf("\n");
    }
    printf("\n");

}



void tm_experiment_mnist() {

    int *x_train;
    int *y_train;
    int *x_test;
    int *y_test;

    const char *filename_train = "data/RAW_FILES/mnist_train.bin";
    const char *filename_test = "data/RAW_FILES/mnist_test.bin";

    if (load_mnist_data(filename_train, &x_train, &y_train, 60000, 784) != 0) {
        printf("Failed to load MNIST Train data.\n");
        return;
    }

    if (load_mnist_data(filename_test, &x_test, &y_test, 10000, 784) != 0) {
        printf("Failed to load MNIST Test data.\n");
        return;
    }
    
    TsetlinMachine* tm = allocate_memory(500, 10, 784, 625, 10.0, 42);
    printf("Clauses: %d\n", tm->num_clauses);
    printf("Classes: %d\n", tm->num_classes);  
    printf("Literals: %d\n", tm->num_literals);
    printf("Threshold: %d\n", tm->threshold);
    printf("s: %.1f\n", tm->s);

    set_train_data(tm, x_train, y_train, 60000);
    set_eval_data(tm, x_test, y_test, 10000);

    train(tm, 10);


}


int main() {    

    // xor_experiment();
    // mnist_experiment();
    // tm_experiment_xor();
    tm_experiment_mnist();


    return 0;
}
