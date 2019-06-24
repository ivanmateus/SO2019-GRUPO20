#include <gmp.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INTERVAL 300000000

const unsigned int A = 1664525, C = 1013904223;

unsigned int next(unsigned int x) { return x * A + C; }
unsigned int x;

double rand_coord() {
    x = next(x);
    int interval = sqrt(INTERVAL);
    double r = (double)(x % (interval + 1)) / interval;
    return r;
}

int main(void) {
    mpf_t pi, circle, square, rand_x, rand_y, distance, tmp_a, tmp_b;

    mpf_inits(pi, circle, square, rand_x, rand_y, distance, tmp_a, tmp_b, NULL);

    x = time(NULL);

    for (int i = 0; i < INTERVAL; i++) {
        double x = rand_coord();
        double y = rand_coord();
        mpf_set_d(rand_x, x);
        mpf_set_d(rand_y, y);

        mpf_mul(tmp_a, rand_x, rand_x);
        mpf_mul(tmp_b, rand_y, rand_y);
        mpf_add(distance, tmp_a, tmp_b);

        if (mpf_cmp_ui(distance, 1) <= 0) {
            mpf_add_ui(circle, circle, 1);
        }
        mpf_add_ui(square, square, 1);
    };

    mpf_div(tmp_a, circle, square);
    mpf_mul_ui(pi, tmp_a, 4);

    gmp_printf("%.6Ff\n", pi);

    mpf_clears(pi, circle, square, rand_x, rand_y, distance, tmp_a, tmp_b, NULL);

    return 0;
}