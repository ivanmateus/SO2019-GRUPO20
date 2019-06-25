#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <pthread.h>

#define LOG2_10 3.32192809489
#define N 100000
#define PREC 5000
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
mpf_t y;
mpf_t a_temp1;
mpf_t a_temp2;
mpf_t y_temp1;
mpf_t y_temp2;
mpf_t dois;
mpf_t pi;

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

void *thread_Y(){
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

    //y = (1-(1-y^4)^1/4)/(1-(1+y^4)^1/4)
    mpf_pow_ui(y, y, 4);
    mpf_ui_sub(y, 1, y);
    mpf_sqrt(y, y);
    mpf_sqrt(y, y);
    mpf_ui_sub(y_temp1, 1, y);
    mpf_add_ui(y_temp2, y, 1);
    mpf_div(y, y_temp1, y_temp2);

    mpf_set(((buf_t *)buf)->buffer[(((buf_t *)buf)->fullSp + ((buf_t *)buf)->start) % BUFSIZE], y);
    ++itCounter;
    (((buf_t *)buf)->fullSp) += 1;
    (((buf_t *)buf)->empSp) -= 1;
    
    pthread_cond_signal(&((buf_t *)buf)->empty);
    pthread_mutex_unlock(&((buf_t *)buf)->buf_lock);
  }
  pthread_exit(0);
}

void *thread_A(){
  int i = 0;
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
    
    mpf_t temp;
    mpf_init_set(temp, ((buf_t *)buf)->buffer[(((buf_t *)buf)->start)]);
    
    //a = a(1+y)^4 - (2^(2n+3))y(1+y+y^2)
    mpf_add_ui(a_temp1, temp, 1);
    mpf_pow_ui(a_temp1, a_temp1, 4);
    mpf_mul(a_temp1, a_temp1, a);
    mpf_pow_ui(dois, dois, (2*i) + 3);
    mpf_mul(a_temp2, temp, temp);
    mpf_add(a_temp2, a_temp2, temp);
    mpf_add_ui(a_temp2, a_temp2, 1);
    mpf_mul(a_temp2, a_temp2, temp);
    mpf_mul(a_temp2, a_temp2, dois);
    mpf_sub(a, a_temp1, a_temp2);
    mpf_set_ui(dois, 2);
    
    ++i;
    ((buf_t *)buf)->fullSp -= 1;
    ((buf_t *)buf)->empSp += 1;
    ((buf_t *)buf)->start += 1;
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
  mpf_init(y);
  mpf_init(a_temp1);
  mpf_init(a_temp2);
  mpf_init(y_temp1);
  mpf_init(y_temp2);
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

  pthread_create(&threads[0], NULL, thread_Y, NULL);
  pthread_create(&threads[1], NULL, thread_A, NULL);  
 
  pthread_join(threads[0], NULL);
  pthread_join(threads[1], NULL);

  //pi = 1/a
  mpf_ui_div(pi, 1, a);

  gmp_printf("%.6Ff\n", pi);
  
  mpf_clear(a);
  mpf_clear(y);
  mpf_clear(a_temp1);
  mpf_clear(a_temp2);
  mpf_clear(y_temp1);
  mpf_clear(y_temp2);
  mpf_clear(dois);
  mpf_clear(pi);

  return 0;
}