all: bbserver bbpeer


bbserver: bbserver.o MyNetworking.o
	gcc bbserver.o MyNetworking.o -o bbserver -pthread

bbpeer: bbpeer.o MyNetworking.o fileio.o
	gcc bbpeer.o MyNetworking.o fileio.o -o bbpeer -pthread

bbpeer.o: bbpeer.c MyNetworking.h fileio.h
	gcc -O0 -g bbpeer.c -c -pthread

bbserver.o: bbserver.c MyNetworking.h
	gcc -O0 -g bbserver.c -c -pthread

MyNetworking.o: MyNetworking.c MyNetworking.h
	gcc -O0 -g MyNetworking.c -c -pthread

fileio.o: fileio.c fileio.h
	gcc -O0 -g fileio.c -c -pthread

clean : 
	rm bbserver bbpeer *.o
