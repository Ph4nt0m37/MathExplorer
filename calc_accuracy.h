#include <stdio.h>

int get_num_correct_digits(char* calculated_str, char* real_str) {
    int i=0;
    while (calculated_str[i] == real_str[i]) {
        i++;
    }
    return i;
}