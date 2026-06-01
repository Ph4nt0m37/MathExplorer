#include <stdio.h>
#include <math.h>
#include "complex_number.h"

void complex_add(complex_number* dest, complex_number num1, complex_number num2) {
    dest->re = num1.re + num2.re;
    dest->im = num1.im + num2.im;
}

void complex_mul(complex_number* dest, complex_number num1, complex_number num2) {
    dest->re = num1.re * num2.re - num1.im * num2.im;
    dest->im = num1.re * num2.im + num1.im * num2.re;
}

uint64_t complex_abs(complex_number num) {
    return sqrt((num.re*num.re) + (num.im*num.im));
}

uint64_t complex_abs_sqrd(complex_number num) {
    return (num.re*num.re) + (num.im*num.im);
}