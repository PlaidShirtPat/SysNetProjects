
/*
 * File         : MyNetworking.h
 * Author       : Patrick Brown, Michael Hood
 * Title        : Networking headers
 * Description  : Header info for our networking code
 */



//constant #defines
#define MAX_LEN 100
#define MAX_CONNECTIONS 20
#define MAX_MESSAGE_LENGTH 5000
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
int CreateServerSocket(int listenPort);
void runServer(int serverSocket,void *(*requestHandleFunction)(void*), int maxRequests ) ;
int SendUDPPacket(char *hostname, int port, char *data);
void printIPAddress(char *message, struct sockaddr_in *clientAddress);
int sendUDPPacket(int socket, struct sockaddr_in *address, char *data);
char *getIPAddressString(struct sockaddr_in *address);
int getPortFromAddress(struct sockaddr_in *address);
struct sockaddr_in *getSocketAddress(int socket);
