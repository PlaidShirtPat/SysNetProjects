
#ifndef COMMON_H
#define COMMON_H
#include "Common.h"

//returns 0 on failure, 1 on success
int contactHost(int socket, struct sockaddr_in *hostAddress)
{
	sendUDPPacket(socket, hostAddress, "hi");
	return 0;
}

//decodes <message> and populates <ack>
void decodeAckMessage(char *message, AckMessage *ack)
{
	//convert to integer with ascii arithmetic
	ack->seqNum = message[0] - '0'; 
	ack->isCorrupt = message[2] - '0';
}

//decodes <message> and populates <segment>
void decodeSegmentMessage(char *message, SegmentMessage *segment)
{
	//convert to integer with ascii arithmetic
	segment->seqNum = message[0] - '0'; 
	segment->isCorrupt = message[2] - '0';
	strcpy(segment->data, &(message[4]));
}

#endif //END MYNETWORKING_H 
