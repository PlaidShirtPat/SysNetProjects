#include "MyNetworking.h"

//max message size + header length (4)
#define MAX_SEGMENT_SIZE (MAX_MESSAGE_LENGTH + 4)

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
