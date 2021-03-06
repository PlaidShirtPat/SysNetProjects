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

bool waitForAck(int socket, int seqNum){

	//reused holders
	char buffer[MAX_MESSAGE_LENGTH];
	AckMessage ack;

	//timeout
	struct timeval timeout;
	timeout.tv_sec = TIMEOUT_VAL;
	timeout.tv_usec = 0;
	
	//set up the watched socket
	fd_set watches;
	FD_ZERO(&watches);
	FD_SET(socket, &watches);

	//looping around, reusing the timout which is set to remaining time by the select call
	while(true){
		
		//wait
		select(socket+1,	&watches, NULL, NULL, &timeout);
		
		//timeout event
		if(timeout.tv_sec == 0 && timeout.tv_usec == 0){
			printf("\n timeout occured");
			return false;
		}

		recvPacket(socket, NULL, buffer); 
		decodeAckMessage(buffer, &ack);

		//if ack is not corrupt and has right sequence #
		if( !(ack.isCorrupt) && ack.seqNum == seqNum)
			return true;

	}
	return false;

}

void sendMessage(int socket, struct sockaddr_in *proxyAddress, char *message, int segmentSize) {

		int dataSize = segmentSize - 4,
				length = strlen(message), 
				i=0;
		
		//number of segments we'll need to send, length/size rounded up
		int numSegments = ceil((double)length / (double)dataSize);
		printf("\n\nNumber of segments to send: %d", numSegments);
		fflush(stdout);

		for(i=0;i<numSegments+1;i++) {
			char *segment = malloc(sizeof(char)* MAX_SEGMENT_SIZE);

			//header stuff
			//are we on a even or odd index, 
			//even index has a seq# of 0, odd 1
			int seqNum = ((i%2) == 0) ? 0 : 1;
			segment[0] = ((i%2) == 0) ? '0' : '1';
			segment[1] = ';';
			//placeholder for corrupt segment
			segment[2] = '0';
			segment[3] = ';';

			//the body of the segment
			//if we've sent the entire message, send the termination segment
			if(i == numSegments) {
				segment[4] = 4;
				segment[5] = '\0';
			}
			else{
				strncpy(&segment[4], &message[i*dataSize], dataSize);
				segment[4+segmentSize] = '\0';
			}
			
			printf("\nsending segment: (%s)", segment);
			fflush(stdout);
			sendUDPPacket(socket, proxyAddress, segment);
			
			//if we don't get an ACK, resend packet, do this be decrementing the index
			printf("\nWaiting for ack (seg#: %d)", seqNum);
			fflush(stdout);
			if(!waitForAck(socket, seqNum))
				i--;
		}

}

int main(int argc, char** argv) {    
  checkInputs(argc, argv);
	int proxyPort = atoi(argv[2]);
  struct sockaddr_in *proxyAddress =  getServerAddressStruct(getAddress(argv[1] ),proxyPort);
	int socket = setUpSocket();

	//print where we're sending stuff
	printSocketStats(socket);

	//Establish Connection
	contactHost(socket, proxyAddress);
	
	//begin accept input
	char *input = malloc(sizeof(char)*(MAX_MESSAGE_LENGTH+1));

	while(true) {
		bool notDone = true;
		int segSize = 0;
		while(notDone){
			printf("\n\nSegmentSize (in bytes, must be >= 5)");
			fflush(stdout);
			fgets(input, MAX_MESSAGE_LENGTH, stdin);
			segSize = atoi(input);
			if(segSize >= 5)
				notDone = false;
		}

		printf("\n\nMessage to send (max length: %d characters): ", MAX_MESSAGE_LENGTH);
		fflush(stdout);
		fgets(input, MAX_MESSAGE_LENGTH, stdin);
		input[strlen(input)-1]='\0';
		sendMessage(socket, proxyAddress, input, segSize);
	}
	
	
	return 0;
}

