#include <stdio.h>
#include <math.h>
#include <string.h>
#include "calc_pi.h"
#include "calc_accuracy.h"
#include <time.h>

#define NUM_DIGITS 1000000
#define ACCURACY 10000000
#define DIGITS_PER_LINE 50

int main() {
    printf("Calculating PI...\n");

    clock_t begin = clock();
    char *pi_str = calc_pi_binary_split(NUM_DIGITS); //for some reason parameters are swapped
    //char *pi_str = calc_pi(NUM_DIGITS, ACCURACY);
    clock_t end = clock();

    double calculation_time = (double) (end-begin)/CLOCKS_PER_SEC;

    //writing pi to file
    FILE *pi_file = fopen("pi.txt","w");

    long int i = 0;
    while (pi_str[i]!='\0') {
        fprintf(pi_file, "%c", pi_str[i]);
        if (i!=0 && i%DIGITS_PER_LINE==0) {
            fprintf(pi_file, "\n");
        }
        i++;
    }

    fclose(pi_file);

    printf("Digits Calculated: %ld. Time taken: %f. Running Accuracy Check...\n", i, calculation_time);

    printf("Correct Digits: %d\n",get_num_correct_digits(pi_str, PI_100000));
    free(pi_str);
    return 0;
}