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
			break;

		table->loadedNodes++;
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

	updateGraph(table->graph, table->lengths, 0);

}

RoutingTable* newRoutingTable(int numNodes, char homeNode)
{
	RoutingTable *table = malloc(sizeof(RoutingTable));
	table->homeNode = homeNode;
	table->numNodes = numNodes;
	table->labels = malloc(sizeof(char)*numNodes);
	table->lengths = malloc(sizeof(int)*numNodes);
	//
	//set all lengths to max value
	int i; for(i=0; i<numNodes;i++) table->lengths[i] = INT_MAX;

	table->predecessors = malloc(sizeof(char)*numNodes);
	table->addresses = malloc(sizeof(struct sockaddr_in*)*numNodes);
	table->ports = malloc(sizeof(short)*numNodes);
	table->graph = newGraph(numNodes);
	table->loadedNodes = 0;
	return table;
}

void freeRoutingTable(RoutingTable *table)
{
	free(table->labels);
	free(table->lengths);
	free(table->predecessors);
	freeGraph(table->graph);

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

int getIndexOfLabel(RoutingTable *table, char label)
{

	int result = -1, i;

	for(i=0; i<table->numNodes; i++)
	{
		if(table->labels[i] == label)
			result = i;
	}
	return result;
}


void calcMinPaths(RoutingTable *table)
{

	PriorityQueue *queue = newQueue(50);
	Pair *tmp = malloc(sizeof(Pair));
	tmp->label = table->homeNode;
	push(queue, tmp);
	NetworkGraph *graph = table->graph;

	bool visited[table->numNodes];
	//set all to unvisited
	memset(visited, false, table->numNodes);

	while(queue->size != 0)
	{
		Pair *currNode = pop(queue);
		
		int currIndex = getIndexOfLabel(table, currNode->label);
		visited[currIndex] = true;
		
		int connected[table->numNodes];
		getConnectedNodes(graph, currIndex, connected);

		int i = 0;
		while(connected[i] != -1)
		{

			//calc lengths, add to routing table if shorter. 
			//Then push nodes onto queue if we have not already visisted them

			char considered = table->labels[connected[i]];
			

			//length is length between nodes + pev length.
			int dist = getValue(table->graph, currIndex, connected[i]);
			dist += table->lengths[currIndex];

			//update the table
			if(dist < table->lengths[connected[i]])
			{
				table->lengths[connected[i]] = dist;
				table->predecessors[connected[i]] = currNode->label;
			}

			//if we've not visited the node, create a pair and push it
			if(!visited[connected[i]])
			{
				tmp = malloc(sizeof(Pair));
				tmp->label = table->labels[connected[i]];
				push(queue, tmp);
			}
			
			i++;
		}

		//this will free all malloced Pairs
		free(currNode);
	}
}

void updateRoutingTable(RoutingTable *table, char fromNode, Pair **updateLists, int numPairs)
{
	
}