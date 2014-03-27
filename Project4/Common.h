#include "MyNetworking.h"


//structs
typedef struct {
	int seqNum;
	bool isCorrupt;
} AckMessage;

typedef struct {
	int seqNum;
	bool isCorrupt;
	char *data;
} SegmentMessage;


//function prototypes
int contactHost(int socket, struct sockaddr_in *proxyAddress);
void decodeAckMessage(char *message, AckMessage *ack);
void decodeSegmentMessage(char *message, SegmentMessage *segment);
