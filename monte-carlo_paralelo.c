#include <gmp.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INTERVAL 300000000
#define NUMTHREADS 4

const unsigned int A = 1664525, C = 1013904223;

unsigned int next(unsigned int x) { return x * A + C; }
unsigned int x;

typedef struct {
    int fromidx, length;
    mpf_t square, circle;
} thread_arg, *ptr_thread_arg;

pthread_t threads[NUMTHREADS];
thread_arg arguments[NUMTHREADS];

double rand_coord() {
    x = next(x);
    int interval = sqrt(INTERVAL);
    double r = (double)(x % (interval + 1)) / interval;
    return r;
}

void proccess(ptr_thread_arg arg, mpf_t circle, mpf_t square, mpf_t rand_x, mpf_t rand_y,
              mpf_t distance, mpf_t tmp_a, mpf_t tmp_b) {
    double x = rand_coord();
    double y = rand_coord();
    mpf_set_d(rand_x, x);
    mpf_set_d(rand_y, y);

    mpf_mul(tmp_a, rand_x, rand_x);
    mpf_mul(tmp_b, rand_y, rand_y);
    mpf_add(distance, tmp_a, tmp_b);

    if (mpf_cmp_ui(distance, 1) <= 0) {
        mpf_add_ui(arg->circle, arg->circle, 1);
    }
    mpf_add_ui(arg->square, arg->square, 1);
}

void *thread_func(void *arg) {
    ptr_thread_arg argument = (ptr_thread_arg)arg;
    int endix = argument->fromidx + argument->length;

    mpf_t rand_x, rand_y, distance, tmp_a, tmp_b;
    mpf_inits(argument->circle, argument->square, rand_x, rand_y, distance, tmp_a, tmp_b, NULL);

    for (int i = argument->fromidx; i < endix; i++) {
        proccess(argument, argument->circle, argument->square, rand_x, rand_y, distance, tmp_a,
                 tmp_b);
    }

    mpf_clears(rand_x, rand_y, distance, tmp_a, tmp_b, NULL);
};

int main(void) {
    x = time(NULL);

    int length = INTERVAL / NUMTHREADS;
    int rem = INTERVAL % NUMTHREADS;

    mpf_t pi, circle, square;
    mpf_inits(pi, circle, square, NULL);

    for (int i = 0; i < NUMTHREADS; i++) {
        arguments[i].fromidx = i * length;
        arguments[i].length = length;
        if (i == (NUMTHREADS - 1)) arguments[i].length += rem;
        pthread_create(&(threads[i]), NULL, thread_func, &(arguments[i]));
    }
    for (int i = 0; i < NUMTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    for (int i = 0; i < NUMTHREADS; i++) {
        mpf_add(square, square, arguments[i].square);
        mpf_add(circle, circle, arguments[i].circle);
        mpf_clears(arguments[i].square, arguments[i].circle, NULL);
    }

    mpf_div(circle, circle, square);
    mpf_mul_ui(pi, circle, 4);

    gmp_printf("%.6Ff\n", pi);

    mpf_clears(pi, circle, square, NULL);

    return 0;
}