
/*
 * File         : MyNetworking.h
 * Author       : Patrick Brown, Michael Hood
 * Title        : Networking headers
 * Description  : Header info for our networking code
 */



//constant #defines
#define MAX_LEN 100
#define MAX_CONNECTIONS 20
#define MAX_MESSAGE_LENGTH 256
#define bool short
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


//Function Prototypes
void exitError(char *error);
struct in_addr *getAddress(char *hostname);
char *getServerHostName();
int createServerSocket(struct sockaddr_in*  serverAddress);
struct sockaddr_in *getServerAddressStruct(struct in_addr *serverIP, int port);
int getSocketPort(int socket);
char *getEchoReply(char *message);
char *getLoadAvgReply();
char *getErrorReply();
char *handleMessage(char *message);
void *handleClientRequest(void *serverSocketArg);
int CreateServerSocket(int listenPort);
void runServer(int serverSocket,void *(*requestHandleFunction)(void*) ) ;
int SendUDPPacket(char *hostname, int port, byte *data);













