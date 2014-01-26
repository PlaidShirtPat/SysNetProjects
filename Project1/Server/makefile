all: TCPserver

TCPserver: TCPserver.o
	gcc  TCPserver.o -o TCPserver

TCPserver.o:
	gcc -O0 -g TCPserver.c -c 

clean:
	rm *.o TCPserver
