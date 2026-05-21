#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gmp.h>
#include <mpfr.h>
#include <mpf2mpfr.h> //converts all mpf into mpfr
#include <stdint.h>

void calc_e(uint64_t digits, int write_to_file) {
    mpfr_set_default_prec(digits);

    mpfr_set_emax(MPFR_EMAX_MAX);
    mpfr_set_emin(MPFR_EMIN_MIN);

    mpfr_t eulers;
    mpfr_init_set_ui(eulers, 0, MPFR_RNDN);

    uint64_t digits_calculated = 0;

    for (uint64_t k=0;k<digits;k++) {
        mpfr_t k_term;
        mpfr_init(k_term);

        mpz_t k_fact;
        mpz_init(k_fact);

        mpz_fac_ui(k_fact, k);

        mpfr_set_z(k_term, k_fact, MPFR_RNDN);
        mpfr_ui_div(k_term, 1, k_term, MPFR_RNDN);

        mpfr_add(eulers, eulers, k_term, MPFR_RNDN);

        mpz_clear(k_fact);
        mpfr_clear(k_term);

        digits_calculated++;
        float percent_complete = ((double) digits_calculated/digits)*100;
        if (fmod(percent_complete, 1) == 0) {
            printf("Percent Calculated: %d%%\r", (int) percent_complete);
            fflush(stdout);
        }
    }

    if (write_to_file) {
        FILE *eulers_file = fopen("eulers.txt","w");
        mpf_out_str(eulers_file, 10, digits, eulers);
        fclose(eulers_file);
    }

    mpfr_clear(eulers);

    printf("\n");
}