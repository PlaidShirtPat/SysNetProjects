all: TCPserver.out testclient.out

TCPserver.out: TCPserver.o
	gcc  TCPserver.o -o TCPserver.out

TCPserver.o:
	gcc -O0 -g TCPserver.c -c 

testclient.out:
	gcc -g testclient.c -o testclient.out

clean:
	rm *.o *.out
