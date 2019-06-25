#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <pthread.h>

#define LOG2_10 3.32192809489
#define N 100000
#define PREC 25000
#define PRECBITS (int)PREC*LOG2_10
#define BUFSIZE (int)(3*N/10)
#define THREADNUM 2

typedef struct {
	pthread_mutex_t buf_lock;
  int fullSp;
  int empSp;
  int start;
	pthread_cond_t full;
	pthread_cond_t empty;
	mpf_t buffer[BUFSIZE];
} buf_t;

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

int itCounter;
buf_t *buf;

buf_t *new_cb(){
	buf_t *cbp;
	cbp = (buf_t *) malloc(sizeof(buf_t));
	if (cbp == NULL){
		return (NULL);
  }
	pthread_mutex_init(&cbp->buf_lock, NULL);
	pthread_cond_init(&cbp->full, NULL);
	pthread_cond_init(&cbp->empty, NULL);
	cbp->fullSp = 0;
	cbp->empSp = BUFSIZE;
  cbp->start = 0;
  for(int i = 0; i < BUFSIZE; ++i){
    mpf_init(cbp->buffer[i]);
  }
	return cbp;
}

void *thread_AB(){
  itCounter = 0;
  while(1){
    pthread_mutex_lock(&((buf_t *)buf)->buf_lock);
    if(itCounter == N){
      pthread_cond_signal(&((buf_t *)buf)->empty);
      pthread_mutex_unlock(&((buf_t *)buf)->buf_lock);
      pthread_exit(0);
    }
    if(((buf_t *)buf)->fullSp == BUFSIZE){
      pthread_cond_wait(&((buf_t *)buf)->full, &((buf_t *)buf)->buf_lock);
    }

    //a = (a + b)/2
    mpf_add(a, temp_a, temp_b);
    mpf_div_ui(a, a, 2.0);

    //b = sqrt(ab)
    mpf_mul(b, temp_a, temp_b);
    mpf_sqrt(b, b);

    mpf_set(((buf_t *)buf)->buffer[(((buf_t *)buf)->fullSp + ((buf_t *)buf)->start) % BUFSIZE], a);
    mpf_set(((buf_t *)buf)->buffer[((((buf_t *)buf)->fullSp + ((buf_t *)buf)->start) % BUFSIZE) + 1], temp_a);
    mpf_set(temp_a, a);
    mpf_set(temp_b, b);

    ++itCounter;
    (((buf_t *)buf)->fullSp) += 2;
    (((buf_t *)buf)->empSp) -= 2;

    pthread_cond_signal(&((buf_t *)buf)->empty);
    pthread_mutex_unlock(&((buf_t *)buf)->buf_lock);
  }
  pthread_exit(0);
}

void *thread_T(){
  while(1){
    pthread_mutex_lock(&((buf_t *)buf)->buf_lock);
    if(((buf_t *)buf)->fullSp <= 0){
      if(itCounter == N){
        pthread_cond_signal(&((buf_t *)buf)->full);
        pthread_mutex_unlock(&((buf_t *)buf)->buf_lock);
        pthread_exit(0);
      }
    }
    if(((buf_t *)buf)->fullSp == 0){
      pthread_cond_wait(&((buf_t *)buf)->empty, &((buf_t *)buf)->buf_lock);
    }

    mpf_sub(temp, ((buf_t *)buf)->buffer[(((buf_t *)buf)->start) + 1], ((buf_t *)buf)->buffer[((buf_t *)buf)->start]);
    
    //t = t - p(a - a)^2
    mpf_mul(temp, temp, temp);
    mpf_mul(temp, temp, temp_p);
    mpf_sub(t, temp_tt, temp);
    
    //p = 2p
    mpf_mul_ui(p, temp_p, 2.0);

    mpf_set(temp_tt, t);
    mpf_set(temp_p, p);
    
    ((buf_t *)buf)->fullSp -= 2;
    ((buf_t *)buf)->empSp += 2;
    ((buf_t *)buf)->start += 2;
    if(((buf_t *)buf)->start == BUFSIZE || ((buf_t *)buf)->empSp == BUFSIZE){
      ((buf_t *)buf)->start = 0;
    }
    
    pthread_cond_signal(&((buf_t *)buf)->full);
    pthread_mutex_unlock(&((buf_t *)buf)->buf_lock);
  }
  pthread_exit(0);
}

int main(void){  
  pthread_t threads[THREADNUM];
  buf = new_cb();

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

  mpf_set(temp_a, a);
  mpf_set(temp_b, b);
  mpf_set(temp_tt, t);
  mpf_set(temp_p, p);

  pthread_create(&threads[0], NULL, thread_AB, NULL);
  pthread_create(&threads[1], NULL, thread_T, NULL);  
 
  pthread_join(threads[0], NULL);
  pthread_join(threads[1], NULL);

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
