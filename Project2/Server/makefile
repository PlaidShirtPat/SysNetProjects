all: UDPserver.out

UDPserver.out: UDPserver.o
	gcc UDPserver.o -o UDPserver -pthread

UDPserver.o:
	gcc -O0 -g UDPserver.c -c -pthread

clean:
	rm *.o *.out
