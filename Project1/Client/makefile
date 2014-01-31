client : TCPmain.o TCPclient.o
	gcc -o client TCPmain.o TCPclient.o -lm

TCPmain.o : TCPmain.c headers.h TCPclient.h
		gcc -g -Wall -c TCPmain.c
TCPclient.o : TCPclient.c headers.h TCPclient.h
		gcc -g -Wall -c TCPclient.c
clean : 
	rm client TCPmain.o TCPclient.o
