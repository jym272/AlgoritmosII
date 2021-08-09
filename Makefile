CC = g++
CFLAGS = -Wall -g
 
tp1: bignum.o cmdline.o decodificar.o product.o tp1.o 
	$(CC) $(CFLAGS) -o tp1 bignum.o cmdline.o decodificar.o product.o tp1.o

tp1.o: tp1.cpp bignum.h cmdline.h decodificar.h product.h
	$(CC) $(CFLAGS) -c tp1.cpp

bignum.o: bignum.h
	$(CC) $(CFLAGS) -c bignum.cpp

product.o: product.h
	$(CC) $(CFLAGS) -c product.cpp

cmdline.o: cmdline.h
	$(CC) $(CFLAGS) -c cmdline.cpp
 
decodificar.o: decodificar.h
	$(CC) $(CFLAGS) -c decodificar.cpp

clean:
	@rm -f *.o *.out *.in tp1