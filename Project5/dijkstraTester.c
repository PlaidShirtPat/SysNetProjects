#include "dijkstra.h"

int main(int argc, char **argv){
	int numNodes = 9;

	RoutingTable*table = createAndLoadRoutingTable (numNodes, 'A', "networkGraph.txt");

	char  buffer[5000];
	getTablePrintString(table, buffer);
	printf("\nTable printout:\n%s", buffer);

	return 0;
}


