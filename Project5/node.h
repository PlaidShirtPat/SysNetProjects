#ifndef NODE_H
#define NODE_H

#include "dijkstra.h"
#include "MyNetworking.h"
#include "networkGraph.h"


//parses a string and puts the values into seqNum, fromNode, and updateList. 
//updateList should be of length numNodes
// "<nodeID>;<seqNum>;<nodeID>,<cost>;<nodeID>,<cost>....."
void createPairList(char *input, Pair *updateList,int *seqNum, char *fromNode, int *count);
void createLSP(char *input, RoutingTable *table, char homeNode);
#endif //NODE_H
