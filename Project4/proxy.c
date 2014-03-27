#include "Common.h"

void checkInputs(int argc, char **argv){

  //Checking for proper number of arguments
  if (argc != 4){
      printf("\nError: There must be three arguments\n<receiverIP>, <receiverPort>, <dropPercentage>\n");
      exit(EXIT_FAILURE);
  }

}

//waits for two connections
void waitForConnects(int socket, struct sockaddr_in *remoteaddress){

  int messageSize;
  char buffer[MAX_MESSAGE_LENGTH+1];
	//get both of our connecttions
	
	socklen_t addrlen = sizeof(*remoteaddress);
	
	if(
			(messageSize = 
			 recvfrom(socket, buffer, MAX_MESSAGE_LENGTH, 0, (struct sockaddr *)remoteaddress, &addrlen)
			) < 0
	)
		exitError("\nrecvfrom() call failed");

  printIPAddress("\nclient address: %s", remoteaddress);

	
}


bool happens(int randPercent){
	return ((int)((((float)rand())/((float)RAND_MAX))*100)) <= randPercent;
}


int main(int argc, char** argv) {    

  checkInputs(argc, argv);

	int dropPercent = atoi(argv[3]);

	int socket = setUpSocket();

	printSocketStats(socket);
	
	struct sockaddr_in *senderAddress = malloc(sizeof(struct sockaddr_in));
	waitForConnects(socket, senderAddress);

	int proxyPort = atoi(argv[2]);
  struct sockaddr_in *receiverAddress =  getServerAddressStruct(getAddress(argv[1] ),proxyPort);

	contactHost(socket, receiverAddress);

	printf("\n\nReceiver:");
	printAddressStats(receiverAddress);
	printf("\nSender:");
	printAddressStats(senderAddress);
	
	char *buffer = malloc(sizeof(char)*(MAX_MESSAGE_LENGTH+1));
	struct sockaddr_in *compareAddress = malloc(sizeof(struct sockaddr_in));
	while(true){
		recvPacket(socket, compareAddress, 	buffer);

		struct sockaddr_in *sendToAddress;
		if(compareAddresses(compareAddress, receiverAddress))
			sendToAddress = senderAddress;
		else
			sendToAddress = receiverAddress;

		//check to see if drop happens
		if(happens(dropPercent)){
			printf("\n\nDropped packet from:");
			printAddressStats(compareAddress);
			printf("\nto:");
			printAddressStats(sendToAddress);
			
		}
		else{
			printf("\n\nforwarding packet from:");
			printAddressStats(compareAddress);
			printf("\nto:");
			printAddressStats(sendToAddress);
			sendUDPPacket(socket, sendToAddress, buffer);
		}
	}



	/*runServer(serverSocket, handleClientRequest, numPeers);*/

	/*sendReplies(serverSocket, numPeers);*/

  printf("\nI'll be back\n");

	return 0;
}
