all: tester

tester: dijTester

dijTester: dijkstra.o dijkstraTester.o MyNetworking.o priorityQueue.o networkGraph.o
	gcc -Wall dijkstraTester.o dijkstra.o priorityQueue.o MyNetworking.o networkGraph.o -o dijTester -pthread

dijkstraTester.o: dijkstraTester.c dijkstra.h priorityQueue.h
	gcc -O0 -Wall -g dijkstraTester.c -c -pthread

dijkstra.o: dijkstra.c dijkstra.h priorityQueue.h
	gcc -O0 -Wall -g dijkstra.c -c -pthread

networkGraph.o: networkGraph.c networkGraph.h 
	gcc -O0 -Wall -g networkGraph.c -c -pthread

priorityQueue.o: priorityQueue.c priorityQueue.h
	gcc -O0 -Wall -g priorityQueue.c -c -pthread

MyNetworking.o: MyNetworking.c MyNetworking.h
	gcc -O0 -Wall -g MyNetworking.c -c -pthread

clean : 
	rm -f dijTester *.o
