#include "node.h"



void checkParams(int argc, char **argv)
{
	if(!(argc == 4 || argc == 5))
		exitError(
				"\nNeed correct # of arguments. " 
				"Correct format is: node <nodeLabel> <totalNumberOfNodes> <discoveryTextFile> [-dynamic]");
				
}

int main(int argc, char **argv)
{
	checkParams(argc, argv);
	
	bool isDynamic = (argc == 5);
	char printBuffer[9999];

	
	RoutingTable *table = createAndLoadRoutingTable( argv[1][0], atoi(argv[2]), argv[3]);

	getTablePrintStringWithAddresses(table, printBuffer);
	printf("%s", printBuffer);
			
	

	




	//cleanup
	freeRoutingTable(table);
	
	//ensure newline before exit
	printf("\n");
	return 0;
}
