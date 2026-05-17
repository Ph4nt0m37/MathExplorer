#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gmp.h>
#include <mpfr.h>
#include <mpf2mpfr.h> //converts all mpf into mpfr
#include "real_numbers.h"

#define PRECISION_PER_DIGIT 3.32192809489
#define DIGITS_PER_TERM 14.181647462

static void calculate_set_constant(mpf_t constant_var);
char* calc_pi(int precision, int max_sum);
static void pi_summation(mpf_t *sum_ptr, int max_sum);
static char* calc_pi_binary_split(int digits);
static mpz_t* binary_split(mpz_t a, mpz_t b, mpz_t C3_OVER_24, int digits);
static void calc_Pab(mpz_t a, mpz_t *Pab);
static void calc_Qab(mpz_t a, mpz_t C3_OVER_24, mpz_t *Qab);
static void calc_Tab(mpz_t a, mpz_t Pab, mpz_t *Tab);

static void calculate_set_constant(mpf_t constant_var) {
    //making series constant
    mpf_t f_series_const;
    mpf_init(f_series_const);

    mpf_t num1;
    mpf_init_set_ui(num1, 426880);

    mpf_t num2;
    mpf_init(num2);
    mpf_sqrt_ui(num2, 10005);
    
    mpf_t const_denom;
    mpf_init(const_denom);
    mpf_mul(const_denom, num1, num2);

    mpf_ui_div(f_series_const, 1, const_denom);

    mpf_clear(num1);
    mpf_clear(num2);
    mpf_clear(const_denom);

    mpf_set(constant_var, f_series_const);
    mpf_clear(f_series_const);
}

char* calc_pi(int precision, int max_sum) {
    mpf_set_default_prec(precision);
    //mpf_set_d(f_series_const, 2.3419932855e-8);
    //mpfr_printf("f_series_const: %0.50Rf\n", f_series_const);

    mpf_t f_series_const;
    mpf_init(f_series_const);
    calculate_set_constant(f_series_const);

    mpf_t sum;
    mpf_init(sum);
    pi_summation(&sum, max_sum);

    //mpfr_printf("sum: %0.50Rf\n", sum);

    mpf_t calculated_pi;
    mpf_init(calculated_pi);
    mpf_mul(calculated_pi, f_series_const, sum);
    mpf_ui_div(calculated_pi, 1, calculated_pi);

    //mpfr_printf("pi: %.200Rf\n", calculated_pi);
    //gmp_printf("pi: %.200Ff\n", calculated_pi);

    char *pi_str = malloc(precision+5);
    mp_exp_t exp;
    mpf_get_str(pi_str, &exp, 10, precision, calculated_pi);

    //printf("pi str: %s\n", pi_str);
    mpf_clear(sum);
    mpf_clear(f_series_const);

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

        float percent_complete = ((double) k/max_sum)*100;
        if (fmod(percent_complete, 1) == 0)
            printf("Percent Calculated: %d%%\n", (int) percent_complete);
    }

    mpf_set(*sum_ptr, sum);
}

char* calc_pi_binary_split(int digits) {
    mpf_set_default_prec((digits * PRECISION_PER_DIGIT) + 128);

    mpf_t f_series_const;
    mpf_init(f_series_const);
    calculate_set_constant(f_series_const);

    //getting (C^3)/24
    mpz_t C3;
    mpz_init(C3);
    mpz_ui_pow_ui(C3, 640320, 3);

    mpz_t C3_OVER_24;
    mpz_init(C3_OVER_24);
    mpz_div_ui(C3_OVER_24, C3, 24);

    mpz_t mpz_zero;
    mpz_init(mpz_zero); //default is 0

    mpz_t mpz_digits;
    mpz_init_set_ui(mpz_digits, (digits / DIGITS_PER_TERM) + 10);

    mpf_t *P = malloc(sizeof(mpf_t));
    mpf_init(*P);

    mpf_t *Q = malloc(sizeof(mpf_t));
    mpf_init(*Q);

    mpf_t *T = malloc(sizeof(mpf_t));
    mpf_init(*T);

    mpz_t *PQT_ab = binary_split(mpz_zero, mpz_digits, C3_OVER_24, digits);
    mpf_set_z(*P, PQT_ab[0]);
    mpf_set_z(*Q, PQT_ab[1]);
    mpf_set_z(*T, PQT_ab[2]);

    mpf_t sum;
    mpf_init(sum);
    mpf_div(sum, *T, *Q);

    mpf_t calculated_pi;
    mpf_init(calculated_pi);
    mpf_mul(calculated_pi, f_series_const, sum);
    mpf_ui_div(calculated_pi, 1, calculated_pi);

    char *pi_str = malloc(digits+5);
    mp_exp_t exp;
    mpf_get_str(pi_str, &exp, 10, digits, calculated_pi);

    mpz_clear(C3);
    mpz_clear(C3_OVER_24);
    mpz_clear(mpz_digits);
    mpf_clear(*P);
    free(P);
    mpf_clear(*Q);
    free(Q);
    mpf_clear(*T);
    free(T);
    mpf_clear(sum);
    mpf_clear(calculated_pi);
    mpf_clear(f_series_const);
    mpz_clears(PQT_ab[0], PQT_ab[1], PQT_ab[2], NULL);
    free(PQT_ab);

    return pi_str;
}

static mpz_t* binary_split(mpz_t a, mpz_t b, mpz_t C3_OVER_24, int digits) {
    static int digits_done = 0;

    mpz_t Pab;
    mpz_init(Pab);

    mpz_t Qab;
    mpz_init(Qab);

    mpz_t Tab;
    mpz_init(Tab);

    mpz_t diff;
    mpz_init(diff);
    mpz_sub(diff, b, a);
    if (mpz_cmp_ui(diff, 1)==0) { //directly compute P, Q T
        if (mpz_cmp_ui(a, 0)==0) {
            mpz_set_ui(Pab, 1);
            mpz_set_ui(Qab, 1);
        }else {
            calc_Pab(a, &Pab);
            calc_Qab(a, C3_OVER_24, &Qab);
        }
        calc_Tab(a, Pab, &Tab);
        if (mpz_odd_p(a)) {
            mpz_neg(Tab, Tab);    
        }

        digits_done += DIGITS_PER_TERM; //add DIGITS_PER_TERM instead of one so the digits done scales correctly
        float percent_complete = ((double) digits_done/digits)*100;
        if (fmod(percent_complete, 1) == 0)
            printf("Percent Calculated: %d%%\n", (int) percent_complete);
    }else {
        mpz_t midpoint;
        mpz_init(midpoint);
        mpz_add(midpoint, a, b);
        mpz_div_ui(midpoint, midpoint, 2);

        //split from a to m
        mpz_t *PQT_am = binary_split(a, midpoint, C3_OVER_24, digits);

        mpz_t Pam;
        mpz_init_set(Pam, PQT_am[0]);

        mpz_t Qam;
        mpz_init_set(Qam, PQT_am[1]);

        mpz_t Tam;
        mpz_init_set(Tam, PQT_am[2]);

        mpz_clears(PQT_am[0], PQT_am[1], PQT_am[2], NULL);
        free(PQT_am);

        //split from m to b
        mpz_t *PQT_mb = binary_split(midpoint, b, C3_OVER_24, digits);

        mpz_t Pmb;
        mpz_init_set(Pmb, PQT_mb[0]);

        mpz_t Qmb;
        mpz_init_set(Qmb, PQT_mb[1]);

        mpz_t Tmb;
        mpz_init_set(Tmb, PQT_mb[2]);

        mpz_clears(PQT_mb[0], PQT_mb[1], PQT_mb[2], NULL);
        free(PQT_mb);

        //combining P, Q
        mpz_mul(Pab, Pam, Pmb);
        mpz_mul(Qab, Qam, Qmb);

        //combining T
        mpz_t Tab1;
        mpz_init(Tab1);
        mpz_mul(Tab1, Qmb, Tam);

        mpz_t Tab2;
        mpz_init(Tab2);
        mpz_mul(Tab2, Pam, Tmb);

        mpz_add(Tab, Tab1, Tab2);

        mpz_clear(Pam);
        mpz_clear(Qam);
        mpz_clear(Tam);
        mpz_clear(Pmb);
        mpz_clear(Qmb);
        mpz_clear(Tmb);
        mpz_clear(Tab1);
        mpz_clear(Tab2);
        mpz_clear(midpoint);
    }
    mpz_clear(diff);
    mpz_t *PQT = malloc(3*sizeof(mpz_t));
    mpz_init_set(PQT[0], Pab);
    mpz_init_set(PQT[1], Qab);
    mpz_init_set(PQT[2], Tab);
    mpz_clears(Pab, Qab, Tab, NULL);
    return PQT;
}

static void calc_Pab(mpz_t a, mpz_t *Pab) {
    //term1
    mpz_t term1;
    mpz_init(term1);
    mpz_mul_ui(term1, a, 6);
    mpz_sub_ui(term1, term1, 5);

    //term2
    mpz_t term2;
    mpz_init(term2);
    mpz_mul_ui(term2, a, 2);
    mpz_sub_ui(term2, term2, 1);

    //term3
    mpz_t term3;
    mpz_init(term3);
    mpz_mul_ui(term3, a, 6);
    mpz_sub_ui(term3, term3, 1);

    //final multiplied terms
    mpz_t Pab_mult;
    mpz_init(Pab_mult);
    mpz_mul(Pab_mult, term1, term2);
    mpz_mul(Pab_mult, Pab_mult, term3);

    mpz_set(*Pab, Pab_mult);

    mpz_clear(term1);
    mpz_clear(term2);
    mpz_clear(term3);
    mpz_clear(Pab_mult);
}

static void calc_Qab(mpz_t a, mpz_t C3_OVER_24, mpz_t *Qab) {
    mpz_t Qab_calc;
    mpz_init(Qab_calc);
    mpz_pow_ui(Qab_calc, a, 3);
    mpz_mul(Qab_calc, Qab_calc, C3_OVER_24);

    mpz_set(*Qab, Qab_calc);

    mpz_clear(Qab_calc);
}

static void calc_Tab(mpz_t a, mpz_t Pab, mpz_t *Tab) {
    mpz_t Tab_calc;
    mpz_init(Tab_calc);
    mpz_mul_ui(Tab_calc, a, 545140134);
    mpz_add_ui(Tab_calc, Tab_calc, 13591409);
    mpz_mul(Tab_calc, Tab_calc, Pab);

    mpz_set(*Tab, Tab_calc);

    mpz_clear(Tab_calc);
}