#include <stdio.h>
#include <math.h>
#include <gmp.h>

#define SERIES_CONSTANT 0.00000002341993215302714155128015541262875842676294

void calc_pi(int max_sum) {
    mpf_t mpf_t_series_const;
    mpf_init_set_d(mpf_t_series_const, SERIES_CONSTANT);



    mpf_clear(mpf_t_series_const);
    printf("bruh\n");
}

static void pi_summation(int max_sum) {
    mpf_t sum;
    mpf_init(sum);
    for (long int k=0;k<=max_sum;k++) {
        //turing k into mpz
        mpz_t mpz_k;
        mpz_init_set_ui(mpz_k, k);

        //(-1)^k
        mpz_t alt;
        mpz_init(alt);
        mpz_pow_ui(alt, alt, k);

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
        mpz_mul(numerator, denominator, pow1);

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

        mpf_add(sum, sum, quotient);
    }
}