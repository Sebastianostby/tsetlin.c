
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


float random() {
    return (float)rand() / (float)RAND_MAX;
}


int randint(int max_value) {
    return rand()%max_value;
}


