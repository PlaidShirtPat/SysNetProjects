all: client server


client : TCPmain.o TCPclient.o TCPclient.class
	gcc -o client TCPmain.o TCPclient.o -lm

TCPclient.class :
	javac TCPclient.java
TCPmain.o : TCPmain.c headers.h TCPclient.h
		gcc -g -Wall -c TCPmain.c
TCPclient.o : TCPclient.c headers.h TCPclient.h
		gcc -g -Wall -c TCPclient.c
clean : 
	rm client TCPmain.o TCPclient.o TCPclient.class
	rm *.o *.out

server: TCPserver.out testclient.out

TCPserver.out: TCPserver.o
	gcc  TCPserver.o -o TCPserver.out -pthread

TCPserver.o:
	gcc -O0 -g TCPserver.c -c -pthread

testclient.out:
	gcc -g testclient.c -o testclient.out

