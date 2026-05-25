#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gmp.h>
#include <mpfr.h>
#include <mpf2mpfr.h> //converts all mpf into mpfr
#include "real_numbers.h"
#include <stdint.h>

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

static void calculate_set_constant(mpfr_t constant_var) {
    //making series constant
    mpfr_t f_series_const;
    mpfr_init(f_series_const);

    mpfr_t num1;
    mpfr_init_set_ui(num1, 426880, MPFR_RNDN);

    mpfr_t num2;
    mpfr_init(num2);
    mpfr_sqrt_ui(num2, 10005, MPFR_RNDN);
    
    mpfr_t const_denom;
    mpfr_init(const_denom);
    mpfr_mul(const_denom, num1, num2, MPFR_RNDN);

    mpfr_ui_div(f_series_const, 1, const_denom, MPFR_RNDN);

    mpfr_clear(num1);
    mpfr_clear(num2);
    mpfr_clear(const_denom);

    mpfr_set(constant_var, f_series_const, MPFR_RNDN);
    mpfr_clear(f_series_const);
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

void calc_pi_binary_split(uint64_t digits, int write_to_file) {
    mpf_set_default_prec((digits * PRECISION_PER_DIGIT) + 128);

    mpfr_t f_series_const;
    mpfr_init(f_series_const);
    calculate_set_constant(f_series_const);

    //getting (C^3)/24
    mpz_t C3;
    mpz_init(C3);
    mpz_ui_pow_ui(C3, 640320, 3);

    mpz_t C3_OVER_24;
    mpz_init(C3_OVER_24);
    mpz_div_ui(C3_OVER_24, C3, 24);

    uint64_t terms_to_calc =  (digits / DIGITS_PER_TERM) + 10;

    mpz_t P_z;
    mpz_init(P_z);

    mpz_t Q_z;
    mpz_init(Q_z);

    mpz_t T_z;
    mpz_init(T_z);

    binary_split(P_z, Q_z, T_z, 0, terms_to_calc, C3_OVER_24, digits);

    //gmp_printf("t: %Zd", T_z);

    mpz_clears(C3, C3_OVER_24, P_z, NULL);

    mpfr_t Q;
    mpfr_init_set_z(Q, Q_z, MPFR_RNDN);
    mpz_clear(Q_z);

    mpfr_t T;
    mpfr_init_set_z(T, T_z, MPFR_RNDN);
    mpz_clear(T_z);

    mpfr_t sum;
    mpfr_init_set_ui(sum, 0, MPFR_RNDN);
    mpfr_div(sum, T, Q, MPFR_RNDN);

    mpfr_clears(T, Q, NULL);

    mpfr_t calculated_pi;
    mpfr_init_set_ui(calculated_pi, 0, MPFR_RNDN);
    mpfr_mul(calculated_pi, f_series_const, sum, MPFR_RNDN);
    mpfr_ui_div(calculated_pi, 1, calculated_pi, MPFR_RNDN);

    if (write_to_file) {
        FILE *pi_file = fopen("pi.txt","w");
        mpf_out_str(pi_file, 10, digits, sum);
        fclose(pi_file);
    }

    mpfr_clears(sum, f_series_const, NULL);

    mpf_clear(calculated_pi);
}

static void binary_split(mpz_t P, mpz_t Q, mpz_t T, uint64_t a, uint64_t b, mpz_t C3_OVER_24, uint64_t digits) {
    static int digits_done = 0;

    uint64_t diff = b - a;
    if (diff==1) { //directly compute P, Q T
        if (a==0) {
            mpz_set_ui(P, 1);
            mpz_set_ui(Q, 1);
        }else {
            calc_Pab(a, P);
            calc_Qab(a, C3_OVER_24, Q);
        }
        calc_Tab(a, P, T);
        if (a % 2 == 1) { //checking if odd
            mpz_neg(T, T);    
        }

        digits_done += DIGITS_PER_TERM; //add DIGITS_PER_TERM instead of one so the digits done scales correctly
        float percent_complete = ((double) digits_done/digits)*100;
        if (fmod(percent_complete, 1) == 0)
            printf("Percent Calculated: %d%%\n", (int) percent_complete);
    }else {
        uint64_t midpoint = (a + b)/2;

        mpz_t Pam, Qam, Tam;
        mpz_inits(Pam, Qam, Tam, NULL);

        //split from a to m
        binary_split(Pam, Qam, Tam, a, midpoint, C3_OVER_24, digits);

        mpz_t Pmb, Qmb, Tmb;
        mpz_inits(Pmb, Qmb, Tmb, NULL);

        //split from m to b
        binary_split(Pmb, Qmb, Tmb, midpoint, b, C3_OVER_24, digits);

        //combining P, Q
        mpz_mul(P, Pam, Pmb);
        mpz_mul(Q, Qam, Qmb);

        mpz_clears(Qam, Pmb, NULL);

        //combining T
        mpz_t Tab1;
        mpz_init(Tab1);
        mpz_mul(Tab1, Qmb, Tam);

        mpz_clears(Qmb, Tam, NULL);

        mpz_t Tab2;
        mpz_init(Tab2);
        mpz_mul(Tab2, Pam, Tmb);

        mpz_clears(Pam, Tmb, NULL);

        mpz_add(T, Tab1, Tab2);

        mpz_clears(Tab1, Tab2, NULL);
    }
}

static void calc_Pab(uint64_t a, mpz_t Pab) {
    //term1
    mpz_t term1;
    mpz_init_set_ui(term1, a);
    mpz_mul_ui(term1, term1, 6);
    mpz_sub_ui(term1, term1, 5);

    //term2
    mpz_t term2;
    mpz_init_set_ui(term2, a);
    mpz_mul_ui(term2, term2, 2);
    mpz_sub_ui(term2, term2, 1);

    //term3
    mpz_t term3;
    mpz_init_set_ui(term3, a);
    mpz_mul_ui(term3, term3, 6);
    mpz_sub_ui(term3, term3, 1);

    //final multiplied terms
    mpz_t Pab_mult;
    mpz_init(Pab_mult);
    mpz_mul(Pab_mult, term1, term2);
    mpz_mul(Pab_mult, Pab_mult, term3);

    mpz_set(Pab, Pab_mult);

    mpz_clear(term1);
    mpz_clear(term2);
    mpz_clear(term3);
    mpz_clear(Pab_mult);
}

static void calc_Qab(uint64_t a, mpz_t C3_OVER_24, mpz_t Qab) {
    mpz_t Qab_calc;
    mpz_init_set_ui(Qab_calc, a);
    mpz_pow_ui(Qab_calc, Qab_calc, 3);
    mpz_mul(Qab_calc, Qab_calc, C3_OVER_24);

    mpz_set(Qab, Qab_calc);

    mpz_clear(Qab_calc);
}

static void calc_Tab(uint64_t a, mpz_t Pab, mpz_t Tab) {
    mpz_t Tab_calc;
    mpz_init_set_ui(Tab_calc, a);
    mpz_mul_ui(Tab_calc, Tab_calc, 545140134);
    mpz_add_ui(Tab_calc, Tab_calc, 13591409);
    mpz_mul(Tab_calc, Tab_calc, Pab);

    mpz_set(Tab, Tab_calc);

    mpz_clear(Tab_calc);
}

void calc_pi_chunking_binary_split(uint64_t digits, int num_chunks, int write_to_file) {
    mpf_set_default_prec((digits * PRECISION_PER_DIGIT) + 128);

    mpfr_t f_series_const;
    mpfr_init(f_series_const);
    calculate_set_constant(f_series_const);

    //mpfr_printf("const: %.50RNf\n", f_series_const);

    //getting (C^3)/24
    mpz_t C3;
    mpz_init(C3);
    mpz_ui_pow_ui(C3, 640320, 3);

    mpz_t C3_OVER_24;
    mpz_init(C3_OVER_24);
    mpz_div_ui(C3_OVER_24, C3, 24);

    uint64_t terms_to_calc =  (digits / DIGITS_PER_TERM) + 10;

    mpz_t P_z;
    mpz_init_set_ui(P_z, 1);

    mpz_t Q_z;
    mpz_init_set_ui(Q_z, 1);

    mpz_t T_z;
    mpz_init(T_z);

    mpz_t split_P_z;
    mpz_init_set_ui(split_P_z, 1);

    mpz_t split_Q_z;
    mpz_init_set_ui(split_Q_z, 1);

    mpz_t split_T_z;
    mpz_init(split_T_z);

    uint64_t terms_per_chunk = terms_to_calc / num_chunks;

    for (int i=0;i<num_chunks;i++) {
        uint64_t starting_term = terms_per_chunk * i;
        uint64_t ending_term = starting_term + terms_per_chunk;

        binary_split(split_P_z, split_Q_z, split_T_z, starting_term, ending_term, C3_OVER_24, digits);
        
        //combining T
        mpz_t term1;
        mpz_init(term1);
        mpz_mul(term1, T_z, split_Q_z);

        mpz_t term2;
        mpz_init(term2);
        mpz_mul(term2, P_z, split_T_z);

        mpz_add(T_z, term1, term2);

        mpz_clears(term1, term2, NULL);

        mpz_mul(P_z, P_z, split_P_z);
        mpz_mul(Q_z, Q_z, split_Q_z);
    }

    //gmp_printf("t: %Zd", T_z);

    mpz_clears(C3, C3_OVER_24, split_P_z, split_Q_z, split_T_z, P_z, NULL);

    mpfr_t Q;
    mpfr_init_set_z(Q, Q_z, MPFR_RNDN);

    mpfr_t T;
    mpfr_init_set_z(T, T_z, MPFR_RNDN);

    // gmp_printf("Qz: %Zd\n", Q_z);
    // gmp_printf("Tz: %Zd\n", T_z);

    //mpfr_printf("Q: %.50Rf\n", Q);
    //mpfr_printf("T: %.50Rf\n", T);

    mpz_clears(Q_z, T_z, NULL);

    mpfr_t sum;
    mpfr_init_set_ui(sum, 0, MPFR_RNDN);
    mpfr_div(sum, T, Q, MPFR_RNDN);

    mpfr_clears(T, Q, NULL);

    mpfr_t calculated_pi;
    mpfr_init_set_ui(calculated_pi, 0, MPFR_RNDN);
    mpfr_mul(calculated_pi, f_series_const, sum, MPFR_RNDN);
    mpfr_ui_div(calculated_pi, 1, calculated_pi, MPFR_RNDN);

    if (write_to_file) {
        FILE *pi_file = fopen("pi.txt","w");
        mpf_out_str(pi_file, 10, digits, calculated_pi);
        fclose(pi_file);
    }

    mpfr_clears(sum, f_series_const, NULL);

    mpf_clear(calculated_pi);
}