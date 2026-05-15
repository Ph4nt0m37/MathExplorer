#include <stdio.h>
#include <math.h>
#include <string.h>
#include "calc_pi.h"
#include "calc_accuracy.h"

#define NUM_DIGITS 1000000
#define ACCURACY 1000000
#define DIGITS_PER_LINE 50

int main() {
    printf("Calculating PI...\n");
    char *pi_str = calc_pi_binary_split(ACCURACY, NUM_DIGITS);
    //char *pi_str = calc_pi(NUM_DIGITS, ACCURACY);

    //writing pi to file
    FILE *pi_file = fopen("pi.txt","w");

    int lines_written = 0;
    char pi_line[DIGITS_PER_LINE+1];
    while (lines_written < (NUM_DIGITS/DIGITS_PER_LINE)) {
        strncpy(pi_line, pi_str+(lines_written*DIGITS_PER_LINE), DIGITS_PER_LINE);
        pi_line[DIGITS_PER_LINE] = '\0';
        fprintf(pi_file, "%s\n", pi_line);
        lines_written++;
    }

    printf("Done Calculating. Running Accuracy Check...\n");

    printf("Correct Digits: %d\n",get_num_correct_digits(pi_str, PI_100000));
    return 0;
}