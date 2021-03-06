#include <math.h>
#include "MyNetworking.h"

int getRandRange(int range)
{ return ((int)(( floor(((double)rand())/((double)RAND_MAX)) * (double)range))) ; }

int main(int argc, char **argv)
{
	int i;
	for(i=0;i<20;i++)
		printf("%d\n", getRandRange(10));
/*

	Pair list[5];
	int count, seqNum;
	char fromNode;
	char input[] ="A;6;B,32;C,312;";

	createPairList(input, list, &seqNum, &fromNode, &count);

	printf("\ncount:%d\nseq#:%d\n", count, seqNum);
	int i;
	for(i=0; i<count; i++)
		printf("%c:%d,", list[i].label, list[i].value);


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

	createLSP(buffer, table, 'A');
	printf("\nLSP: <%s>\n", buffer);

	freeRoutingTable(table);
	return 0;
	*/

}
