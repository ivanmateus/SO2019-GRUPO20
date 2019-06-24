CC=gcc
LIBS=-lm -lgmp -lpthread -lmpfr

all:
	$(CC) -o borwein borwein.c $(LIBS)
	$(CC) -o borwein_paralelo borwein_paralelo.c $(LIBS)
	$(CC) -o gauss-legendre gauss-legendre.c $(LIBS)
	$(CC) -o gauss-legendre_paralelo gauss-legendre_paralelo.c $(LIBS)
	$(CC) -o black-scholes black-scholes.c rand_bm.c $(LIBS)
	$(CC) -o black-scholes_paralelo black-scholes_paralelo.c rand_bm.c $(LIBS)
	$(CC) -o monte-carlo monte-carlo.c rand_bm.c $(LIBS)
	# $(CC) -o monte-carlo_paralelo monte-carlo_paralelo.c rand_bm.c $(LIBS)

borwein: borwein.c
	$(CC) -o borwein borwein.c $(LIBS)

borwein_paralelo: borwein_paralelo.c
	$(CC) -o borwein_paralelo borwein_paralelo.c $(LIBS)

gauss-legendre: gauss-legendre.c
	$(CC) -o gauss-legendre gauss-legendre.c $(LIBS)

gauss-legendre_paralelo: gauss-legendre_paralelo.c
	$(CC) -o gauss-legendre_paralelo gauss-legendre_paralelo.c $(LIBS)

black-scholes: black-scholes.c rand_bm.c
	$(CC) -o black-scholes black-scholes.c rand_bm.c $(LIBS)

black-scholes_paralelo: black-scholes_paralelo.c rand_bm.c
	$(CC) -o black-scholes_paralelo black-scholes_paralelo.c rand_bm.c $(LIBS)

monte-carlo: monte-carlo.c rand_bm.c
	$(CC) -o monte-carlo monte-carlo.c rand_bm.c $(LIBS)

monte-carlo_paralelo: monte-carlo_paralelo.c rand_bm.c
	$(CC) -o monte-carlo_paralelo monte-carlo_paralelo.c rand_bm.c $(LIBS)