#include "dijkstra.h"

/*
 * Expects a file containing table->numNodes lines
 * line format is expected:
 *  <nodeLabel>,<IP_address/hostname>,<portNumber>,<cost>
 */
void loadTableFromFile(RoutingTable *table, FILE *fp)
{

	char buffer[500];
	int i;
	for(i=0; i< table->numNodes; i++)
	{

		//get the line from the file and put it into the buffer
		char *result = fgets(buffer, 500, fp);
		if(result == NULL)
			exitError("There were not enough nodes in the network graph file");

		//get the node label of this line, this will also be the initial predecessor
		char *tok = strtok(buffer, ",");
		table->labels[i] = tok[0];
		
		//all connected nodes have the homeNode as a predecessor initially.
		table->predecessors[i] = table->homeNode;

		//get the address information
		char *hostname = strtok(NULL, ",");
		char *port = strtok(NULL, ",");
		table->addresses[i] = getServerAddressStruct(getAddress(hostname), atoi(port));
		
		//get the edge length info
		table->lengths[i] = atoi(strtok(NULL, ","));
	}
}

RoutingTable* newRoutingTable(int numNodes, char homeNode)
{
	RoutingTable *table = malloc(sizeof(RoutingTable));
	table->homeNode = homeNode;
	table->numNodes = numNodes;
	table->labels = malloc(sizeof(char)*numNodes);
	table->lengths = malloc(sizeof(int)*numNodes);
	table->predecessors = malloc(sizeof(char)*numNodes);
	table->addresses = malloc(sizeof(struct sockaddr_in*)*numNodes);
	table->ports = malloc(sizeof(short)*numNodes);
	return table;
}

void freeRoutingTable(RoutingTable *table)
{
	free(table->labels);
	free(table->lengths);
	free(table->predecessors);

	int i;
	for(i=0;i < table->numNodes;i++)
	{
		free(table->addresses[i]);

		free(table->addresses);
		free(table->ports);
		free(table);

	}
}

RoutingTable* createAndLoadRoutingTable(int numNodes, char currentNodeLabel, char *pathToNetworkFile)
{
	RoutingTable *table = newRoutingTable(numNodes, currentNodeLabel);

	FILE *fp = fopen(pathToNetworkFile, "r");
	loadTableFromFile(table, fp);
	fclose(fp);

	return table;

}

void getTablePrintString(RoutingTable *table, char *buffer)
{
	//init thestart of buffer to null char
	buffer[0] = '\0';

	int i;

	sprintf(buffer, "Routing Table for node %c\n[Node]\t[PathLength]\t[PredecessorNode]\n", table->homeNode);
	for(i=0; i< table->numNodes;i++)
	{
		char buffer2[500];
		sprintf(buffer2, "%c\t%d\t\t%c\n", table->labels[i], table->lengths[i], table->predecessors[i]);
		strcat(buffer, buffer2);
	}

}

void getTablePrintStringWithAddresses(RoutingTable *table, char *buffer)
{
	//init thestart of buffer to null char
	buffer[0] = '\0';

	int i;

	sprintf(buffer, 
		"Routing Table for node %c\n[Node]\t[PathLength]\t[PredecessorNode]\t[Address:Port]\n", table->homeNode);
	for(i=0; i< table->numNodes;i++)
	{
		char buffer2[500], ipBuffer[16];

		getIPAddressString(table->addresses[i], ipBuffer);


		sprintf(buffer2, "%c\t%d\t\t%c\t\t\t%s:%d\n", 
			table->labels[i], table->lengths[i], 
			table->predecessors[i], ipBuffer, getPortFromAddress(table->addresses[i]));

		strcat(buffer, buffer2);
	}

}

void updateRoutingTable(RoutingTable *table, char fromNode, int *lengths)
{
	int i,j,k;	
	char considerationList[table->numNodes];

	char currentNode = table->homeNode;
	for(i=0;i<table->numNodes; i++)
	{
		for(j=0;i<table->numNodes; i++)
		{
			if(table->predecessors[i] == currentNode && table->lengths[i] > 0)
			{
				
			}
		}
	}
}
