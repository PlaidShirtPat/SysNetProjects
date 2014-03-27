#include "Common.h"

void checkInputs(int argc, char **argv){

  //Checking for proper number of arguments
  if (argc != 6){
      printf("\nError: There must be five arguments\n<receiverIP>, <receiverPort>, <dropPercentage>, <delayPercent>, <corruptPercent>\n");
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

void *delayedSend(void *sendParams){
	SendParams *params = sendParams;
	sleep(params->delay);
	sendUDPPacket(params->socket, params->toAddress, params->data);

	
	printf("\nPacket: (%s)\ndelayed packet sent to:", params->data);
	printAddressStats(params->toAddress);
	return NULL;

}

void startForwarding(int socket, struct sockaddr_in *receiverAddress, struct sockaddr_in *senderAddress, 
		int dropPercent, int delayPercent, int corruptPercent){

	char scratch[MAX_MESSAGE_LENGTH+1];
	char *buffer = scratch;
	struct sockaddr_in compareAddress;
	while(true){
		recvPacket(socket, &compareAddress, 	buffer);

		struct sockaddr_in *sendToAddress;
		if(compareAddresses(&compareAddress, receiverAddress))
			sendToAddress = senderAddress;
		else
			sendToAddress = receiverAddress;

		//check to see if drop happens
		if(happens(dropPercent)){

			printf("\n\nPacket: (%s)\nDropped packet from:", buffer);
			printAddressStats(&compareAddress);
			printf("\nto:");
			printAddressStats(sendToAddress);
			
		}
		else if(happens(delayPercent)) {

			printf("\n\nPacket: (%s)\nDelayed packet from:", buffer);
			printAddressStats(&compareAddress);
			printf("\nto:");
			printAddressStats(sendToAddress);

			SendParams sendParams;
			sendParams.delay = (TIMEOUT_VAL *2);
			sendParams.socket = socket;
			sendParams.toAddress = sendToAddress;
			sendParams.data = buffer;


			pthread_t thread;
			pthread_create(&thread, NULL, delayedSend, (void *)&sendParams);
		}
		else if(happens(corruptPercent)){
			printf("\n\nPacket: (%s)\nForwarding corrupted packet from:", buffer);
			printAddressStats(&compareAddress);
			printf("\nto:");
			printAddressStats(sendToAddress);
			//corrupt packet
			buffer[2] = '1';
			sendUDPPacket(socket, sendToAddress, buffer);
		}
		else{
			printf("\n\nPacket: (%s)\nForwarding packet from:", buffer);
			printAddressStats(&compareAddress);
			printf("\nto:");
			printAddressStats(sendToAddress);
			sendUDPPacket(socket, sendToAddress, buffer);
		}
	}

}


int main(int argc, char** argv) {    

  checkInputs(argc, argv);

	int dropPercent = atoi(argv[3]);
	int delayPercent = atoi(argv[4]);
	int corruptPercent = atoi(argv[5]);

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
	

	startForwarding(socket, receiverAddress, senderAddress, dropPercent, delayPercent, corruptPercent);


	/*runServer(serverSocket, handleClientRequest, numPeers);*/

	/*sendReplies(serverSocket, numPeers);*/

  printf("\nI'll be back\n");

	return 0;
}
