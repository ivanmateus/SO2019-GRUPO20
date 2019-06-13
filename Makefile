CC=gcc
LIBS=-lm -lgmp -lpthread -lmpfr

all:
	$(CC) -o borwein borwein.c $(LIBS)
	$(CC) -o borwein_paralelo borwein_paralelo.c $(LIBS)
	$(CC) -o gauss-legendre gauss-legendre.c $(LIBS)
	$(CC) -o gauss-legendre_paralelo gauss-legendre_paralelo.c $(LIBS)
	$(CC) -o black-scholes black-scholes.c rand_bm.c $(LIBS)

borwein:
	$(CC) -o borwein borwein.c $(LIBS)

borwein_paralelo:
	$(CC) -o borwein_paralelo borwein_paralelo.c $(LIBS)

gauss-legendre:
	$(CC) -o gauss-legendre gauss-legendre.c $(LIBS)

gauss-legendre_paralelo:
	$(CC) -o gauss-legendre_paralelo gauss-legendre_paralelo.c $(LIBS)

black-scholes:
	$(CC) -o black-scholes black-scholes.c rand_bm.c $(LIBS)