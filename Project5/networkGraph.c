#include "networkGraph.h"

NetworkGraph *newGraph(int size)
{
	NetworkGraph *graph = malloc(sizeof(NetworkGraph));
	graph->size = size;
	graph->graph = malloc(sizeof(int)*size*size);
	//init everything to MAX
	int i;
	for(i=0; i<size*size; i++)
		graph->graph[i] = INT_MAX;
	
	for(i=0;i<size;i++)
		setValue(graph, i, i, 0);
	return graph;
}

void freeGraph(NetworkGraph *graph)
{
	free(graph->graph);
	free(graph);
}

//newVals is expected to be of size graph->size
void updateGraph(NetworkGraph *graph,  int *newVals, int homeIndex)
{
	int i;
	for (i=0;i<graph->size;i++)
	{
		//mirroed graphs because bidirectional
		setValue(graph, homeIndex, i, newVals[i]);
		setValue(graph, i, homeIndex, newVals[i]);
	}
}

//get the val at graph[x][y]
int getValue(NetworkGraph *graph, int x, int y)
{
	return graph->graph[(x*graph->size) + y];
}

//set the val at graph[x][y]
void setValue(NetworkGraph *graph, int x, int y, int value)
{
	graph->graph[(x*graph->size) + y] = value;
}

//expects buffer to be sufficent size, roughly size*size*10ish should be alright
void graphToString(NetworkGraph *graph, char *buffer)
{
	//init to empty string
	buffer[0] = 0;
	int i,j;
	char *buffPtr = buffer;

	for(i=0; i < graph->size; i++)
	{
		sprintf(buffPtr, "\n|");
		//set the pointer to the end of the current string
		buffPtr = &buffPtr[strlen(buffPtr)];

		for(j=0; j < graph->size; j++)
		{

			int val = getValue(graph, j, i);
			if(val != INT_MAX)
				sprintf(buffPtr, "%d\t", val);
			else
				sprintf(buffPtr, "!Con\t");

			buffPtr = &buffPtr[strlen(buffPtr)];
		}
	}
}

//buffer is expected to be graph->size + 1
//returns the indexes of connected nodes, -1 indicates the end of the list
void getConnectedNodes(NetworkGraph *graph, int nodeIndex, int *buffer)
{
	int i,j=0;
	
	//set everything to 0
	for(i=0; i< graph->size + 1; i++)
		buffer[i] = -1;

	for(i=0; i<graph->size; i++)
	{
		int val = getValue(graph, nodeIndex, i) ;
		if(val != INT_MAX && val != 0)
		{	
			buffer[j] = i;
			j++;
		}
	}
}


