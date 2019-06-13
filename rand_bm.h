#ifndef RAND_BM_H_
#define RAND_BM_H_

#include <stdlib.h>

struct BoxMullerState {
    double x1, x2, w, y1, y2;
    int useLast;
    struct drand48_data random;
};

void initBoxMullerState(struct BoxMullerState* state);
double boxMullerRandom(struct BoxMullerState* state);

#endif