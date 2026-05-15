#include <stdio.h>
#include <math.h>
#include "calc_pi.h"
#include "calc_accuracy.h"

int main() {
    char *pi_str = calc_pi(1024*10, 200);

    printf("Correct Digits: %d\n",get_num_correct_digits(pi_str, PI_10000));
    return 0;
}