
#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "MyNetworking.h"
#include "priorityQueue.h"
#include "networkGraph.h"

//Typedefs
typedef struct{
	//the base node
	char homeNode;
	//number of in the network
	int numNodes;
	//Node labels
	char *labels;
	//known shortest path
	int *lengths;
	//label of the predecessor
	char *predecessors;
	//the addresses of neighboring nodes
	struct sockaddr_in **addresses;
	//the addresses of neighboring ports
	short *ports;
	//graph representation of the network
	NetworkGraph *graph;
	//number of loaded nodes
	int loadedNodes;
	long *seqNums;
} RoutingTable;

//prototypes

//Expects a opened file pointer and an initilied RoutingTable
void loadTableFromFile(RoutingTable *table, FILE *fp);
//mallocs a RoutingTable struct and initlizes homeNode and NumNodes
RoutingTable* newRoutingTable(int numNodes, char currentNodeLabel);
//frees a RoutingTable struct
void freeRoutingTable(RoutingTable *table);
//creates a RoutingTable and calls loadTableFromFile
RoutingTable* createAndLoadRoutingTable(int numNodes, char currentNodeLabel, char *pathToNetworkFile);
//places a string representation of the RoutingTable into buffer
void getTablePrintString(RoutingTable *table, char *buffer);
void getTablePrintStringWithAddresses(RoutingTable *table, char *buffer);
//Updates table with a linkstate packet from fromNode using Dijkstra's Algorithm
void updateRoutingTable(RoutingTable *table, char fromNode, Pair *updateLists, int numPairs);

void calcMinPaths(RoutingTable *table);

int getIndexOfLabel(RoutingTable *table, char label);

#endif //endif DIJKSTRA_H
