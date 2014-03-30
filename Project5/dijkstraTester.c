#include "dijkstra.h"

int main(int argc, char **argv)
{
	int numNodes = 5;

	RoutingTable*table = createAndLoadRoutingTable (numNodes, 'A', "networkGraph.txt");

	char  buffer[5000];
	getTablePrintString(table, buffer);
	printf("\nTable printout:\n%s", buffer);

	getTablePrintStringWithAddresses(table, buffer);
	printf("\nTable printout:\n%s", buffer);


	return 0;
}


