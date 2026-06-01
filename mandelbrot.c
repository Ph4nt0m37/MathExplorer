#include <stdio.h>
#include "mandelbrot.h"

unsigned int get_num_iterations(complex_number c, unsigned int max_iterations) {
    complex_number z = {0,0};
    unsigned int i = 0;
    while (complex_abs_sqrd(z) < EXPANSION_RANGE*EXPANSION_RANGE && i < max_iterations) {
        complex_mul(&z, z, z); //squaring z
        complex_add(&z, z, c); //adding c
        //printf("z_re: %Lf | z_im: %Lf\n",z.re, z.im);
        i++;
    }
    return i;
}

bool in_set(complex_number c, unsigned int max_iterations) {
    return get_num_iterations(c, max_iterations) == max_iterations; //if i is equal to max_iterations, that means it this c value did not explode past EXPANSION_RANGE (default: 2)
}