#include <stdio.h>
#include <stdlib.h>
#include "include/xor.h"  // data
#include "include/tsetlin_machine.h" // tm


void xor_experiment() {

    int N = 100000;
    int M = 10;
    int *X;
    int *Y;

    get_xor(N, M, 0.2, &X, &Y, false);

}


void tm_experiment() {

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

    // printf("C Matrix (%d clauses x %d literals):\n", tm->num_clauses, 2*tm->num_literals);
    // for (int i = 0; i < tm->num_clauses; i++) {
    //     printf("Clause %3d: ", i);
    //     for (int j = 0; j < 2*tm->num_literals; j++) {
    //         printf("%2d ", tm->C[i * (2*tm->num_literals) + j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    // // Print W matrix (contiguous)
    // printf("W Matrix (%d classes x %d clauses):\n", tm->num_classes, tm->num_clauses);
    // for (int i = 0; i < tm->num_classes; i++) {
    //     printf("Class %2d: ", i);
    //     for (int j = 0; j < tm->num_clauses; j++) {
    //         printf("%2d ", tm->W[i * tm->num_clauses + j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

}



int main() {    

    // xor_experiment();
    tm_experiment();

    return 0;
}
