#ifndef COMPLEX_NUMBER_HEADER
#define COMPLEX_NUMBER_HEADER

#include <stdio.h>
#include <stdint.h>

typedef long double ldouble;

typedef struct {
    ldouble re;
    ldouble im;
} complex_number;

void complex_add(complex_number* dest, complex_number num1, complex_number num2);

void complex_mul(complex_number* dest, complex_number num1, complex_number num2);

uint64_t complex_abs(complex_number num);

uint64_t complex_abs_sqrd(complex_number num);

#endif