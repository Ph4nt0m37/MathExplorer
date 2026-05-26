#ifndef CALC_PI_HEADER
#define CALC_PI_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gmp.h>
#include <mpfr.h>
#include <mpf2mpfr.h> //converts all mpf into mpfr
#include "real_numbers.h"
#include <stdint.h>
#include "GUI/gui.h"

#define PRECISION_PER_DIGIT 3.32192809489
#define DIGITS_PER_TERM 14.181647462

static void calculate_set_constant(mpfr_t constant_var);
char* calc_pi(int precision, int max_sum);
static void pi_summation(mpf_t *sum_ptr, int max_sum);
void calc_pi_binary_split(uint64_t digits, int write_to_file);
static void binary_split(mpz_t P, mpz_t Q, mpz_t T, uint64_t a, uint64_t b, mpz_t C3_OVER_24, uint64_t digits);
static void calc_Pab(uint64_t a, mpz_t Pab);
static void calc_Qab(uint64_t a, mpz_t C3_OVER_24, mpz_t Qab);
static void calc_Tab(uint64_t a, mpz_t Pab, mpz_t Tab);
void calc_pi_chunking_binary_split(uint64_t digits, int num_chunks, int write_to_file);



#endif