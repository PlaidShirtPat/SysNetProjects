#ifndef NETWORKGRAPH_H
#define NETWORKGRAPH_H

#include "MyNetworking.h"
#include "priorityQueue.h"

typedef struct{
	int size;
	//graph array, will manually calculate offsets
	int *graph;


} NetworkGraph;



NetworkGraph *newGraph(int size);
void freeGraph(NetworkGraph *graph);
int getValue(NetworkGraph *graph, int x, int y);
void setValue(NetworkGraph *graph, int x, int y, int value);
void updateGraph(NetworkGraph *graph,  int *newVals, int homeIndex);
void graphToString(NetworkGraph *graph, char *buffer);

//buffer is expected to be graph->size + 1
//returns the indexes of connected nodes, -1 indicates the end of the list
int getConnectedNodes(NetworkGraph *graph, int nodeIndex, int *buffer);


#endif //NETWORKGRAPH_H
