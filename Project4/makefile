all: proxy sender receiver

proxy: proxy.o MyNetworking.o Common.o
	gcc -Wall proxy.o MyNetworking.o Common.o -o proxy -pthread

proxy.o: proxy.c MyNetworking.h 
	gcc -O0 -Wall -g proxy.c -c -pthread

sender: sender.o MyNetworking.o Common.o
	gcc -Wall sender.o MyNetworking.o Common.o -o sender -pthread

sender.o: sender.c MyNetworking.h Common.h
	gcc -O0 -Wall -g sender.c -c -pthread

receiver: receiver.o MyNetworking.o Common.o
	gcc -Wall receiver.o MyNetworking.o Common.o -o receiver -pthread

receiver.o: sender.c MyNetworking.h Common.h
	gcc -O0 -Wall -g receiver.c -c -pthread

MyNetworking.o: MyNetworking.c MyNetworking.h
	gcc -O0 -Wall -g MyNetworking.c -c -pthread

Common.o: Common.c Common.h
	gcc -O0 -Wall -g Common.c -c -pthread

clean : 
	rm receiver proxy sender *.o
