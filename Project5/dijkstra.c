#include "dijkstra.h"

/*
 * Expects a file containing table->numNodes lines
 * line format is expected:
 *  <nodeLabel>,<IP_address/hostname>,<portNumber>,<cost>
 */
void loadTableFromFile(RoutingTable *table, FILE *fp){

	char buffer[500];
	int i;
	for(i=0; i< table->numNodes; i++){
		
		//get the line 
		unsigned char result = fgets(buffer, 500, fp);
		if(result == EOF)
			exitError("There were not enough nodes in the network graph file");

		//get the node label of this line
		char *tok = strtok(buffer, ",");
		table->labels[i] = tok[0];
		char *hostname = strtok(buffer, ",");
		char *port = strtok(buffer, ",");



		
		table->[i] = tok[0];
		
	}


}

RoutingTable* newRoutingTable(int numNodes, char homeNode){
	RoutingTable *table = malloc(sizeof(RoutingTable));
	table->homeNode = homeNode;
	table->numNodes = numNodes;
	table->labels = malloc(sizeof(char)*numNodes);
	table->lengths = malloc(sizeof(int)*numNodes);
	table->predecessors = malloc(sizeof(char)*numNodes);
	table->addresses = malloc(sizeof(struct sockaddr_in)*numNodes);
	table->ports = malloc(sizeof(short)*numNodes);
}

void freeRoutingTable(RoutingTable *table){
	free(table->labels);
	free(table->lengths);
	free(table->predecessors);
	free(table->addresses);
	free(table->ports);
	free(table);
}

RoutingTable* createAndLoadRoutingTable(int numNodes, char currentNodeLabel, char *pathToNetworkFile){

	RoutingTable *table = newRoutingTable(numNodes, currentNodeLabel);

	FILE *fp = fopen(pathToNetworkFile, "r");
	loadTableFromFile(table, fp);
	fclose(fp);

	return table;

}

void getTablePrintString(RoutingTable *table, char *buffer){
	//init thestart of buffer to null char
	buffer[0] = '\0';

	int i;

	sprintf(buffer, "Routing Table for node %c\n[Node]\t[PathLength]\t[PredecessorNode]\n", table->homeNode);
	for(i=0; i< table->numNodes;i++){
		sprintf(buffer, "%c\t%d\t%c\n", table->labels[i], table->lengths[i], table->predecessors[i]);

	}

}
