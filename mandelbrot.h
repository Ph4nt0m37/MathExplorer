#ifndef MANDELBROT_HEADER
#define MANDELBROT_HEADER

#include "complex_number.h"
#include <stdbool.h>

#define EXPANSION_RANGE 2

unsigned int get_num_iterations(complex_number c, unsigned int max_iterations);
bool in_set(complex_number c, unsigned int max_iterations);

#endif