all: TCPserver

TCPserver.out: TCPserver.o
	gcc  TCPserver.o -o TCPserver.out

TCPserver.o:
	gcc -O0 -g TCPserver.c -c 

clean:
	rm *.o *.out
