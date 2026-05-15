#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gmp.h>
#include <mpfr.h>
#include <mpf2mpfr.h> //converts all mpf into mpfr
#include "real_numbers.h"

char* calc_pi(int precision, int max_sum);
static void pi_summation(mpf_t *sum_ptr, int max_sum);

char* calc_pi(int precision, int max_sum) {
    mpf_set_default_prec(precision);

    //making series constant
    mpf_t f_series_const;
    mpf_init(f_series_const);

    mpf_t const_numer;
    mpf_init_set_ui(const_numer, 12);

    mpf_t three_twos;
    mpf_init_set_d(three_twos, 3/2.0);
    
    mpf_t const_denom;
    mpf_init_set_ui(const_denom, 640320);
    mpfr_pow(const_denom, const_denom, three_twos, MPFR_RNDN);

    mpf_div(f_series_const, const_numer, const_denom);
    //mpf_set_d(f_series_const, 2.3419932855e-8);
    //mpfr_printf("f_series_const: %0.50Rf\n", f_series_const);

    mpf_t sum;
    mpf_init(sum);
    //pi_summation(&sum, max_sum);

    char *sum_str = malloc(precision+5); //chars are 1 byte so I don't have to use sizeof
    mp_exp_t exp;
    mpf_get_str(sum_str, &exp, 10, precision, sum);

    //mpfr_printf("sum: %0.50Rf\n", sum);

    //printf("sum: %s\n", sum_str);

    mpf_t calculated_pi;
    mpf_init(calculated_pi);
    mpf_mul(calculated_pi, f_series_const, sum);
    mpf_ui_div(calculated_pi, 1, calculated_pi);

    //mpfr_printf("pi: %.200Rf\n", calculated_pi);
    //gmp_printf("pi: %.200Ff\n", calculated_pi);

    char *pi_str = malloc(precision+5);
    mpf_get_str(pi_str, &exp, 10, precision, calculated_pi);

    //printf("pi str: %s\n", pi_str);
    mpf_clear(sum);
    mpf_clear(f_series_const);
    mpf_clear(const_numer);
    mpf_clear(const_denom);

    return pi_str;
}

static void pi_summation(mpf_t *sum_ptr, int max_sum) {
    mpf_t sum;
    mpf_init(sum);
    for (long int k=0;k<=max_sum;k++) {
        //turing k into mpz
        mpz_t mpz_k;
        mpz_init_set_ui(mpz_k, k);

        //(-1)^k
        mpz_t alt;
        mpz_init_set_si(alt, pow(-1,k));

        //(6k)!
        mpz_t fact1;
        mpz_init(fact1);
        mpz_fac_ui(fact1, (6*k));

        //545140134k
        mpz_t add1;
        mpz_init(add1);
        mpz_mul_ui(add1, mpz_k, 545140134);

        //545140134k + 13591409
        mpz_add_ui(add1, add1, 13591409);

        //calculating numerator
        mpz_t numerator;
        mpz_init(numerator);
        mpz_mul(numerator, alt, fact1);
        mpz_mul(numerator, numerator, add1);

        //(3k)!
        mpz_t fact2;
        mpz_init(fact2);
        mpz_fac_ui(fact2, (3*k));
        
        //(k!)^3
        mpz_t fact3;
        mpz_init(fact3);
        mpz_fac_ui(fact3, k);
        mpz_pow_ui(fact3, fact3, 3);

        //(640320)^(3k)
        mpz_t pow1;
        mpz_init_set_ui(pow1, 640320);
        mpz_pow_ui(pow1, pow1, (3*k));

        //calculating denominator
        mpz_t denominator;
        mpz_init(denominator);
        mpz_mul(denominator, fact2, fact3);
        mpz_mul(denominator, denominator, pow1);

        //converting numerator and denominator to float
        mpf_t numerator_f;
        mpf_init(numerator_f);
        mpf_set_z(numerator_f, numerator);

        mpf_t denominator_f;
        mpf_init(denominator_f);
        mpf_set_z(denominator_f, denominator);

        //calculating division
        mpf_t quotient;
        mpf_init(quotient);
        mpf_div(quotient, numerator_f, denominator_f);

        //adding the quotient to the sum
        mpf_add(sum, sum, quotient);

        //clearing variables
        mpz_clear(mpz_k);
        mpz_clear(alt);
        mpz_clear(fact1);
        mpz_clear(add1);
        mpz_clear(numerator);
        mpz_clear(fact2);
        mpz_clear(fact3);
        mpz_clear(pow1);
        mpz_clear(denominator);
        mpf_clear(numerator_f);
        mpf_clear(denominator_f);
        mpf_clear(quotient);
    }

    mpf_set(*sum_ptr, sum);
}