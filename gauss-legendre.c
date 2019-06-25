#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

#define LOG2_10 3.32192809489
#define N 100000
#define PREC 25000
#define PRECBITS (int)PREC*LOG2_10

int main(void){
  mpf_t a;
  mpf_t b;
  mpf_t t;
  mpf_t p;
  mpf_t pi;
  mpf_t temp_a;
  mpf_t temp_b;
  mpf_t temp_tt;
  mpf_t temp_p;    
  mpf_t temp;
  
  mpf_set_default_prec(PRECBITS);

  mpf_init(a);
  mpf_init(b);
  mpf_init(t);
  mpf_init(p);
  mpf_init(pi);
  mpf_init(temp_a);
  mpf_init(temp_b);
  mpf_init(temp_tt);
  mpf_init(temp_p);
  mpf_init(temp);

  //a = 1
  mpf_set_d(a, 1.0);

  //b = 1/sqrt(2)
  mpf_set_d(b, 2.0);
  mpf_sqrt(b, b);
  mpf_ui_div(b, 1, b);
  
  //t = 0.25
  mpf_set_d(t, 0.25);

  //p = 1
  mpf_set_d(p, 1.0);

  for(int i = 0; i < N + 1; ++i){
    mpf_set(temp_a, a);
    mpf_set(temp_b, b);
    mpf_set(temp_tt, t);
    mpf_set(temp_p, p);

    //a = (a + b)/2
    mpf_add(a, temp_a, temp_b);
    mpf_div_ui(a, a, 2.0);

    //b = sqrt(ab)
    mpf_mul(b, temp_a, temp_b);
    mpf_sqrt(b, b);

    //t = t - p(a - a)^2
    mpf_sub(temp, temp_a, a);
    mpf_mul(temp, temp, temp);
    mpf_mul(temp, temp, temp_p);
    mpf_sub(t, temp_tt, temp);

    //p = 2p
    mpf_mul_ui(p, temp_p, 2.0);
  }

  //pi = (a + b)^2/4t
  mpf_add(pi, a, b);
  mpf_mul(pi, pi, pi);
  mpf_mul_ui(a, t, 4.0);
  mpf_div(pi, pi, a);

  gmp_printf("%.6Ff\n", pi);
  
  mpf_clear(a);
  mpf_clear(b);
  mpf_clear(t);
  mpf_clear(pi);
  mpf_clear(temp_a);
  mpf_clear(temp_b);
  mpf_clear(temp_tt);
  mpf_clear(temp); 

  return 0;
}
