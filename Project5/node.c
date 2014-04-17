#include "node.h"

void checkParams(int argc, char **argv)
{
	if(!(argc == 4 || argc == 5))
		exitError(
				"\nNeed correct # of arguments. " 
				"Correct format is: node <nodeLabel> <totalNumberOfNodes> <discoveryTextFile> [-dynamic]");
				
}

void createLSP(char *input, RoutingTable *table, char homeNode)
{
	int index = getIndexOfLabel(table, homeNode);
	char *writePT = input;
	
	//write homeNode and SeqNum
	sprintf(input, "%c;%ld;", homeNode, table->seqNums[index]);

	writePT = &(writePT[strlen(writePT)]);

	int connected[table->numNodes+1];
	getConnectedNodes(table->graph, index, connected);

	//process all connected nodes
	int i=0;
	while(connected[i] != -1)
	{
		sprintf(writePT, "%c,%d;", table->labels[connected[i]], getValue(table->graph, index, connected[i]));
		writePT = &(writePT[strlen(writePT)]);
		i++;
	}
	table->seqNums[index]++;
}

void createPairList(char *input, Pair *updateList,int *seqNum, char *fromNode, int *count)
{


	char scratch[9999];
	strcpy(scratch, input);


	char *tok = strtok(scratch, ";");
	*fromNode = tok[0];
	tok = strtok(NULL, ";");
	*seqNum = atol(tok);
	//main proccessing
	int i=0;
	while(( tok = strtok(NULL, ";")) != NULL)
	{
		updateList[i].label = tok[0];
		updateList[i].value = atoi(&tok[2]);
		i++;
	}

	*count = i;
}


void forwardLSP(int socket, RoutingTable *table, char *LSP)
{
	 
	int index = getIndexOfLabel(table, table->homeNode);
	int connected[table->numNodes+1];	
	getConnectedNodes(table->graph, index, connected);
	int i;
	for(i=0; connected[i] != -1; i++)
		sendUDPPacket(socket, table->addresses[connected[i]], LSP);

}

void *waitForNetworkGraph(void *args)
{
	
	int socket = *((int *)(((void **)args)[0]));
	RoutingTable *table = ((void **)args)[1];
	bool *keepGoing = ((void **)args)[2];



	char buffer[9999];
	char printBuffer[9999];
	int lspCount=0;

	graphToString(table->graph, printBuffer);
	printf("\nlsp#: %d, graph:\n%s", lspCount, printBuffer);
	fflush(stdout);

	while(lspCount < table->numNodes-1 )
	{

		recvPacket(socket, NULL, buffer);

		Pair pairs[table->numNodes];
		int seqNum, count;
		char fromNode;
		printf("\nLSP: %s", buffer);
		createPairList(buffer, pairs, &seqNum, &fromNode, &count);

		int index = getIndexOfLabel(table, fromNode);

		//if the seq # is newer, update to this seq
		if(table->seqNums[index] < seqNum || (seqNum == 0 && table->seqNums[index] == 0))
		{
			updateRoutingTable(table, fromNode, pairs, count);
			table->seqNums[index] = seqNum+1;
			printf("\nupdating w/ LSP from %c", fromNode);

			graphToString(table->graph, printBuffer);
			printf("\nlsp#: %d, graph:\n%s", lspCount, printBuffer);

			fflush(stdout);
			forwardLSP(socket, table, buffer);
		}

		//see if we got an LSP from everyone
		int i;
		lspCount = 0;
		for( i=0;i<table->numNodes;i++)
		{
			if(table->seqNums[i] > 0)
				lspCount++;
		}

	}
	*keepGoing = false;

	return NULL;
}

int main(int argc, char **argv)
{
	checkParams(argc, argv);
	
	bool isDynamic = (argc == 5);
	char printBuffer[9999];
	
	//init routing table
	RoutingTable *table = createAndLoadRoutingTable(atoi(argv[2]), argv[1][0], argv[3]);
	getTablePrintStringWithAddresses(table, printBuffer);
	printf("%s", printBuffer);

		
	int serverSocket = createServerSocket(table->addresses[getIndexOfLabel(table, table->homeNode)]);
	bool keepGoing = true;
	void *args[3];
	args[0] = &serverSocket;
	args[1] = table;
	args[2] = &keepGoing;
		
	//start wait
	pthread_t thread ;
	pthread_create( &thread, NULL, waitForNetworkGraph, (void *)args);

	char LSP[9999];
	createLSP(LSP, table, table->homeNode);


	printf("\nPress any key to start flood");
	getc(stdin);
	
	forwardLSP(serverSocket, table, LSP);

	while(keepGoing);
	
	calcMinPaths(table);
	getTablePrintString(table, printBuffer);
	printf("\n%s", printBuffer);
	fflush(stdout);
	
	//cleanup
	freeRoutingTable(table);
	
	//ensure newline before exit
	printf("\n");
	return 0;
}

