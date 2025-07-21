#include <stdio.h>
#include <stdlib.h>
#include "include/xor.h"  // data
#include "include/tsetlin_machine.h" // tm


void xor_experiment() {

    int N = 10;
    int M = 6;
    int **X;
    int *Y;

    get_xor(N, M, 0.2, &X, &Y, true);

}


void tm_experiment() {

    int **X;
    int *y;


    TsetlinMachine* tm = allocate_memory(10, 2, 100, 50, 10.0, 42);
    printf("Clauses: %d\n", tm->num_clauses);
    printf("Classes: %d\n", tm->num_classes);  
    printf("Literals: %d\n", tm->num_literals);
    printf("Threshold: %d\n", tm->threshold);
    printf("s: %.1f\n", tm->s);

    printf("s_min_inv: %.1f\n", tm->s_min_inv);
    printf("s_min: %.1f\n", tm->s_min);

    printf("C Matrix (%d clauses x %d literals):\n", tm->num_clauses, 2*tm->num_literals);
    for (int i = 0; i < tm->num_clauses; i++) {
        printf("Clause %3d: ", i);
        for (int j = 0; j < 2*tm->num_literals; j++) {
            printf("%2d ", tm->C[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    printf("W Matrix (%d classes x %d clauses):\n", tm->num_classes, tm->num_clauses);
    for (int i = 0; i < tm->num_classes; i++) {
        printf("Class %2d: ", i);
        for (int j = 0; j < tm->num_clauses; j++) {
            printf("%2d ", tm->W[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    train(tm, X, y, 1000);

}



int main() {    

    xor_experiment();
    tm_experiment();

    return 0;
}