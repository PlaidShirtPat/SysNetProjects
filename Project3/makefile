all: UDPserver UDPclient

UDPclient : UDPmain.o UDPclient.o UDPclient.class
	gcc -o UDPclient UDPmain.o UDPclient.o -lm

UDPclient.class :
	javac UDPclient.java

UDPmain.o : UDPmain.c headers.h UDPclient.h
		gcc -g -Wall -c UDPmain.c

UDPclient.o : UDPclient.c headers.h UDPclient.h
		gcc -g -Wall -c UDPclient.c

UDPserver: UDPserver.o
	gcc UDPserver.o -o UDPserver -pthread

UDPserver.o:
	gcc -O0 -g UDPserver.c -c -pthread

clean : 
	rm UDPserver UDPclient *.o
	rm UDPclient.class
