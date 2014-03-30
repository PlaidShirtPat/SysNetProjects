all: tester

tester: dijTester

dijTester: dijkstra.o dijkstraTester.o MyNetworking.o Common.o
	gcc -Wall dijkstraTester.o dijkstra.o MyNetworking.o Common.o -o dijTester -pthread

dijkstraTester.o: dijkstraTester.c dijkstra.h
	gcc -O0 -Wall -g dijkstraTester.c -c -pthread

dijkstra.o: dijkstra.c dijkstra.h
	gcc -O0 -Wall -g dijkstra.c -c -pthread

MyNetworking.o: MyNetworking.c MyNetworking.h
	gcc -O0 -Wall -g MyNetworking.c -c -pthread

Common.o: Common.c Common.h
	gcc -O0 -Wall -g Common.c -c -pthread

clean : 
	rm -f dijTester *.o
