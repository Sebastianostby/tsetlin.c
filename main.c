#include <stdio.h>
#include <stdlib.h>
#include "include/utils.h"  // Include from include directory
#include "include/xor.h"  // data


void xor_experiment() {

    int N = 10;
    int M = 6;
    int **X;
    int *Y;

    get_xor(N, M, 0.2, &X, &Y, true);

}


int main() {    

    xor_experiment();

    return 0;
}