/* Esse gerador de numeros aleatorios gera numeros que seguem uma distribuicao normal com media 0 e
 * desvio padrao 1: N(0,1). */

#include "rand_bm.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void initBoxMullerState(struct BoxMullerState* state) {
    state->random.__init = 0;
    state->useLast = 0;

    struct timeval now;
    gettimeofday(&now, NULL);
    state->random.__x[0] = now.tv_usec;
}

double boxMullerRandom(struct BoxMullerState* state) {
    double randomNumber;

    if (state->useLast) {
        state->y1 = state->y2;
        state->useLast = 0;
    } else {
        do {
            drand48_r(&state->random, &state->x1);
            state->x1 = 2.0 * state->x1 - 1.0;
            drand48_r(&state->random, &state->x2);
            state->x2 = 2.0 * state->x2 - 1.0;
            state->w = state->x1 * state->x1 + state->x2 * state->x2;
        } while (state->w >= 1.0);

        state->w = sqrt((-2.0 * log(state->w)) / state->w);
        state->y1 = state->x1 * state->w;
        state->y2 = state->x2 * state->w;
        state->useLast = 1;
    }

    return state->y1;
}
