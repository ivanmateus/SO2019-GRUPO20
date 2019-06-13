#include <gmp.h>
#include <math.h>
#include <mpfr.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "rand_bm.h"

int main(void) {
    mpf_t S, E, r, sig, T, M, tmp_a, tmp_b, t;
    mpfr_t tmp_r;
    srand(time(0));

    struct BoxMullerState state;
    initBoxMullerState(&state);

    mpf_init(S);
    mpf_init(E);
    mpf_init(r);
    mpf_init(sig);
    mpf_init(T);
    mpf_init(M);
    mpf_init(t);
    mpf_init(tmp_a);
    mpf_init(tmp_b);

    gmp_scanf("%Ff", &S);
    gmp_scanf("%Ff", &E);
    gmp_scanf("%Ff", &r);
    gmp_scanf("%Ff", &sig);
    gmp_scanf("%Ff", &T);
    gmp_scanf("%Ff", &M);

    mpf_mul(tmp_a, sig, sig);
    mpf_div_ui(tmp_a, tmp_a, 2);
    mpf_sub(tmp_a, r, tmp_a);
    mpf_mul(tmp_a, tmp_a, T);

    mpf_sqrt(tmp_b, T);
    mpf_mul(tmp_b, tmp_b, sig);

    double random = boxMullerRandom(&state);
    mpf_t rand_num;
    mpf_init_set_d(rand_num, random);
    mpf_mul(tmp_b, tmp_b, rand_num);
    mpf_add(tmp_a, tmp_a, tmp_b);

    mpfr_init_set_f(tmp_r, tmp_a, MPFR_RNDN);
    mpfr_exp(tmp_r, tmp_r, MPFR_RNDN);
    mpfr_get_f(tmp_a, tmp_r, MPFR_RNDN);

    gmp_printf("%Ff\n", tmp_a);
    return 0;
}