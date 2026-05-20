#include <stdio.h>
#include <math.h>
#include <string.h>
#include "calc_pi.h"
#include "calc_accuracy.h"
#include <time.h>

#define NUM_DIGITS 10000000
#define ACCURACY 10000000
#define DIGITS_PER_LINE 50

int main() {
    printf("Calculating PI...\n");

    clock_t begin = clock();
    calc_pi_chunking_binary_split(NUM_DIGITS, 5, 1); //for some reason parameters are swapped
    //char *pi_str = calc_pi(NUM_DIGITS, ACCURACY);
    clock_t end = clock();

    double calculation_time = (double) (end-begin)/CLOCKS_PER_SEC;

    printf("done calculating!\n");

    printf("Digits Calculated: %d. Time taken: %f.\n", NUM_DIGITS, calculation_time);

    return 0;
}