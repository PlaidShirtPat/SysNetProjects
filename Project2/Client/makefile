client : UDPmain.o UDPclient.o
	gcc -o client UDPmain.o UDPclient.o -lm

UDPmain.o : UDPmain.c headers.h UDPclient.h
		gcc -g -Wall -c UDPmain.c
UDPclient.o : UDPclient.c headers.h UDPclient.h
		gcc -g -Wall -c UDPclient.c
clean : 
	rm client *.o
