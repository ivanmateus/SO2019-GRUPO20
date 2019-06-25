# SO2019-GRUPO20

## Compilação

Os códigos devem, naturalmente, serem compilados uma primeira vez antes de serem executados. Algumas bibliotecas utilizadas, como PThreads e GMP já fazem parte das bibliotecas do compilador gcc. Por esse motivo e pelo fato dele ser o compilador de C mais amplamente usado, optamos por usá-lo na compilação dos nossos códigos.

Foi criado um Makefile para o auxílio da compilação, portanto, para compilar todos os códigos, basta executar o comando abaixo

```
make all
```
Caso o usuário queira, por exemplo, compilar apenas o arquivo borwein_paralelo, ele precisa rodar:
```
make borwein_paralelo
```
Para compilar apenas usando o gcc, sem usar o Makefile, o usuário pode seguir a seguinte referência:

Borwein: `gcc -o borwein borwein.c -lgmp`

Borwein Paralelo: `gcc -o borwein_paralelo borwein_paralelo.c -lgmp -lpthread`

Gauss-Legendre: `gcc -o gauss-legendre gauss-legendre.c -lgmp`

Gauss-Legendre Paralelo: `gcc -o gauss-legendre_paralelo gauss-legendre_paralelo.c -lgmp -lpthread`

Black-Scholes: `gcc -o black-scholes black-scholes.c rand_bm.c -lm -lgmp -lmpfr`

Black-Scholes Paralelo: `gcc -o black-scholes_paralelo black-scholes_paralelo.c rand_bm.c -lm -lgmp -lpthread -lmpfr`

Monte-Carlo: `gcc -o monte-carlo monte-carlo.c rand_bm.c -lm -lgmp`

Monte-Carlo Paralelo: `gcc -o monte-carlo_paralelo monte-carlo_paralelo.c rand_bm.c -lm -lgmp -lpthread `


## Execução

Após os arquivos serem compilados, eles podem ser executados. Para executar, por exemplo, o algoritmo de Gauss-Legendre paralelo, basta executar:
```
./gauss-legendre_paralelo
```
Porém, como estaremos também fazendo a contagem do tempo, é importante utilizar o comando:
```
/usr/bin/time -f “%e” ./gauss-legendre_paralelo
```
Observação: O algoritmo de Black-Scholes, tanto sequencial como paralelo, precisa de um arquivo de entrada, com os valores necessários. Para executá-lo:
 ```
/usr/bin/time -f “%e” ./gauss-legendre_paralelo < entrada_blackscholes.txt
```