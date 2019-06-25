#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

#define LOG2_10 3.32192809489
#define N 1000000000
#define PREC 50000
#define PRECBITS (int)PREC*LOG2_10

int main(void){
  mpf_t a;
  mpf_t y;
  mpf_t a1;
  mpf_t a2;
  mpf_t y1;
  mpf_t y2;
  mpf_t dois;
  mpf_t pi;

  mpf_set_default_prec(PRECBITS);

  mpf_init(a);
  mpf_init(y);
  mpf_init(a1);
  mpf_init(a2);
  mpf_init(y1);
  mpf_init(y2);
  mpf_init(dois);
  mpf_init(pi);

  //a = 2(sqrt(2) - 1)^2
  mpf_sqrt_ui(a, 2);
  mpf_sub_ui(a, a, 1);

  //y = sqrt(2) - 1
  mpf_set(y, a);

  mpf_mul(a, a, a);
  mpf_mul_ui(a, a, 2);
  mpf_set_ui(dois, 2);

  for(int i = 0; i < N; ++i){
    mpf_set(a1, a);
    mpf_set(a2, a);
    mpf_set(y1, y);
    mpf_set(y2, y);

    //y = (1-(1-y^4)^1/4)/(1-(1+y^4)^1/4)
    mpf_pow_ui(y, y, 4);
    mpf_ui_sub(y, 1, y);
    mpf_sqrt(y, y);
    mpf_sqrt(y, y);
    mpf_ui_sub(y1, 1, y);
    mpf_add_ui(y2, y, 1);
    mpf_div(y, y1, y2);

    //a = a(1+y)^4 - (2^(2n+3))y(1+y+y^2)
    mpf_add_ui(a1, y, 1);
    mpf_pow_ui(a1, a1, 4);
    mpf_mul(a1, a1, a);
    mpf_pow_ui(dois, dois, (2*i) + 3);
    mpf_mul(a2, y, y);
    mpf_add(a2, a2, y);
    mpf_add_ui(a2, a2, 1);
    mpf_mul(a2, a2, y);
    mpf_mul(a2, a2, dois);
    mpf_sub(a, a1, a2);
    mpf_set_ui(dois, 2);
  }

  //pi = 1/a
  mpf_ui_div(pi, 1, a);

  gmp_printf("%.6Ff\n", pi);
  
  mpf_clear(a);
  mpf_clear(y);
  mpf_clear(a1);
  mpf_clear(a2);
  mpf_clear(y1);
  mpf_clear(y2);
  mpf_clear(dois);
  mpf_clear(pi);

  return 0;
}
