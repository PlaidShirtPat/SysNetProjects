#include "Common.h"

void checkInputs(int argc, char **argv){

	//Checking for proper number of arguments
	if (argc != 3){
			printf("\nError: There must be two arguments\n<proxyName>, <proxyPort>\n");
			exit(EXIT_FAILURE);
	}
	//Check port number for validity
	if (atoi(argv[2]) < 0 || atoi(argv[2]) > 65535){
			printf("\nError: The port # must be between 0 and 65535");
			exit(EXIT_FAILURE);
	}

}

void sendAck(int socket, struct sockaddr_in *proxyAddress, int seqNum){
			char segment[3];

			segment[0] = (seqNum == 0) ? '0' : '1';
			segment[1] = ';';
			//placeholder for corrupt segment
			segment[2] = '0';

			sendUDPPacket(socket, proxyAddress, segment);
}



void receiveMessage(int socket, struct sockaddr_in *proxyAddress){

	char *buffer = malloc(sizeof(char)*(MAX_SEGMENT_SIZE+1));
	int seqNum = 0;
	bool gettingMessage = true;

	char message[ MAX_MESSAGE_LENGTH + 1];
	//init to empty string
	message[0] = '\0';
		
	while(gettingMessage) {

		SegmentMessage segment;
		char segMessage[MAX_MESSAGE_LENGTH];
		segment.data = segMessage;

		bool gettingPacket = true;
		while(gettingPacket){

			recvPacket(socket, NULL, buffer);
			printf("\nsegment: %s", buffer);

			//decode message
			decodeSegmentMessage(buffer, &segment);

			//drop packet if it is corrupt or wrong seq #
			if(!(segment.isCorrupt) || segment.seqNum == seqNum)
				gettingPacket = false;

		}
		sendAck(socket, proxyAddress, seqNum);
		
		//append data to end of message
		strcat(message, segment.data);
		
		//toggle expected seq#
		seqNum = (seqNum == 0) ? 1 : 0;

	}

	printf("\nFinal message\n%s", message);
}

void waitForProxy(int socket, struct sockaddr_in *proxyAddress){
	
  int messageSize;
  char buffer[MAX_MESSAGE_LENGTH+1];
	//get both of our connecttions
	
	socklen_t addrlen = sizeof(*proxyAddress);
	
	if(
			(messageSize = 
			 recvfrom(socket, buffer, MAX_MESSAGE_LENGTH, 0, (struct sockaddr *)proxyAddress, &addrlen)
			) < 0
	)
		exitError("\nrecvfrom() call failed");

  printIPAddress("\nclient address: %s", proxyAddress);
}

int main(int argc, char** argv) {    

  struct sockaddr_in *proxyAddress = malloc(sizeof(struct sockaddr_in));
	int socket = setUpSocket();

	//print where we're sending stuff
	printSocketStats(socket);

	//get the proxy address
	waitForProxy(socket, proxyAddress);

	//begin accept input
	receiveMessage(socket, proxyAddress);

	return 0;

}

