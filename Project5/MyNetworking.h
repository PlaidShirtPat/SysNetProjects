
/*
 * File         : MyNetworking.h
 * Author       : Patrick Brown, Michael Hood
 * Title        : Networking headers
 * Description  : Header info for our networking code
 */

#ifndef MYNETWORKING_H
#define MYNETWORKING_H
//constant #defines
#define MAX_LEN 100
#define MAX_CONNECTIONS 20
#define MAX_MESSAGE_LENGTH 5000
#define TIMEOUT_VAL 2
//max message size + header length (4)
#define MAX_SEGMENT_SIZE (MAX_MESSAGE_LENGTH + 4)
#define bool char
#define true 1
#define false 0

//#includes
#include <stdio.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <stdlib.h>   
#include <string.h>  
#include <unistd.h>  
#include <netdb.h>
#include <pthread.h>
#include <math.h>
#include <limits.h>

//typedefs
typedef struct {
	int delay;
	int socket;
	struct sockaddr_in *toAddress;
	char *data;
}SendParams;


//Function Prototypes
void exitError(char *error);
in_addr_t getAddress(char *hostname);
char *getServerHostName();
int createServerSocket(struct sockaddr_in*  serverAddress);
struct sockaddr_in *getServerAddressStruct(in_addr_t serverIP, int port);
int getSocketPort(int socket);
char *getEchoReply(char *message);
char *getLoadAvgReply();
char *getErrorReply();
char *handleMessage(char *message);
int CreateServerSocket(int listenPort);
//void runServer(int serverSocket, void *(*requestHandleFunction)(void*), int maxRequests );
//will print a IP address. Message eg: "IP address %s\n"
void printIPAddress(char *message, struct sockaddr_in *clientAddress);
int sendUDPPacket(int socket, struct sockaddr_in *address, char *data);
//gets IP address string, expects a buffer of at least length 16
char *getIPAddressString(struct sockaddr_in *address, char *buffer);
int getPortFromAddress(struct sockaddr_in *address);
struct sockaddr_in *getSocketAddress(int socket);
int setUpSocket();
void printSocketStats(int socket);
void printAddressStats(struct sockaddr_in *address);
int recvPacket(int socket, struct sockaddr_in *senderAddress, char *buffer);
bool compareAddresses(struct sockaddr_in *a, struct sockaddr_in *b);


#endif //END MYNETWORKING_H 
