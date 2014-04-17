#include "dijkstra.h"

int main(int argc, char **argv)
{



	int numNodes = 5;
	char  buffer[9999];


	RoutingTable *table = createAndLoadRoutingTable (numNodes, 'A', "networkGraph.txt");

	getTablePrintString(table, buffer);
	printf("\nTable printout:\n%s", buffer);

	graphToString(table->graph, buffer);
	printf("\n%s", buffer); 


	table->labels[3] = 'D';
	table->labels[4] = 'E';
	table->predecessors[3] = 'B';
	table->predecessors[4] = 'C';
	
	
	int b[5] = {1, 0, INT_MAX, 9, INT_MAX };
	updateGraph(table->graph, b, 1);

	int c[5] = {2, INT_MAX, 0, INT_MAX, 2 };
	updateGraph(table->graph, c, 2);

	int d[5] = {INT_MAX, 9, INT_MAX, 0, 2 };
	updateGraph(table->graph, d, 3);

	int e[5] = {INT_MAX, INT_MAX, 2, 2, 0 };
	updateGraph(table->graph, e, 4);

	graphToString(table->graph, buffer);
	printf("\n%s", buffer); 

	calcMinPaths(table);


	getTablePrintString(table, buffer);
	printf("\nTable printout:\n%s", buffer);

	freeRoutingTable(table);
	return 0;

}
