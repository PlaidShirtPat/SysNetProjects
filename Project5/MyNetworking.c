/*
 * File         : MyNetworking.c
 * Author       : Patrick Brown, Michael Hood
 * Title        : Networking code
 * Description  : Contains all the code needed to make UDP connections and such
 */

#include "MyNetworking.h"

///
///Functions
///

/*
 * Name         : exitError()
 * Description  :
 */
void exitError(char *error)
{
	printf("\nError occured: %s", error);
	perror(error);
	printf("\n");
	exit(1);
}

/*
 * Name         : getAddress()
 * Description  :
 * Notes: As per the MANual documentation, the hostent struct is static data and does not need to be free()d
 */
in_addr_t getAddress(char *hostname)
{
	//Get host address by hostname
	struct hostent *hostInfo = gethostbyname(hostname);           
	//Get host address from address list
	struct in_addr **addrList = (struct in_addr**)(hostInfo->h_addr_list);        
	//return the first address
	in_addr_t addr = (addrList[0])->s_addr;
	return addr;
}

/*
 * Name         : getServerHostName()
 * Description  :
 */
char *getServerHostName()
{
	char *hostname;
	hostname = malloc(sizeof(char)*MAX_LEN);
	gethostname(hostname, MAX_LEN);
	return hostname;
}

/*
 * Name         : createServerSocket()
 * Description  :
 */
int createServerSocket(struct sockaddr_in*  serverAddress)
{
	int serverSocket;

	if((serverSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		exitError("\nsocket() call failed");

	if(bind(serverSocket, (struct sockaddr *) serverAddress, sizeof(*serverAddress)) < 0)
		exitError("\nbind() call failed");

	return serverSocket;
}

/*
 * Name         : setUpSocket()
 * Description  : Creates a basic UDP socket to send and recive over
 */
int setUpSocket()
{

	int socket = createServerSocket(getServerAddressStruct(getAddress(getServerHostName()),0));


	return socket;
}

/*
 * Name         : getServerAddressStruct()
 * Description  :
 */
struct sockaddr_in *getServerAddressStruct(in_addr_t serverIP, int port)
{

	//create the socket address structure
	struct sockaddr_in *serverAddress;
	serverAddress = malloc(sizeof(struct sockaddr_in));

	memset(serverAddress, 0, sizeof(serverAddress));
	serverAddress->sin_family = AF_INET;
	serverAddress->sin_port = htons(port);
	serverAddress->sin_addr.s_addr = serverIP;
	return serverAddress;
}

/*
 * Name         : getSocketPort()
 * Description  :
 */
int getSocketPort(int socket)
{

	//Grab the address structure from the socket and return the port
	struct sockaddr_in mySockAddr;
	socklen_t length = sizeof(mySockAddr);
	getsockname(socket, (struct sockaddr *) &mySockAddr, &length);
	return ntohs(mySockAddr.sin_port);
}

struct sockaddr_in *getSocketAddress(int socket)
{

	//Grab the address structure from the socket and return the port
	struct sockaddr_in *mySockAddr = malloc(sizeof(struct sockaddr_in));
	socklen_t length = sizeof(*mySockAddr);
	getsockname(socket, (struct sockaddr *) mySockAddr, &length);
	return mySockAddr;
}

/*
 * Name         : getEchoReply()
 * Description  :
 */
char *getEchoReply(char *message)
{
	char *reply = malloc(sizeof(char) * MAX_MESSAGE_LENGTH+1);
	int messageLen = strlen(message) - 13 ;
	//copy the message starting at the 7th spot in reply, leaving room for header <reply> and footer 
	strcpy(&(reply[0]), "<reply>");
	strncpy(&(reply[6]), &(message[5]),  MAX_MESSAGE_LENGTH-15);
	strcpy(&(reply[7+messageLen]), "</reply>");
	return reply;
}

/*
 * Name         : getLoadAvgReply()
 * Description  :
 */
char *getLoadAvgReply()
{
	char *reply = malloc(sizeof(char) * MAX_MESSAGE_LENGTH+1);

	double loads[3];
	getloadavg(loads, 3);
	//copy the message starting at the 7th spot in reply, leaving room for header <reply> and footer 
	strcpy(&(reply[0]), "<replyLoadAvg>");
	sprintf(&(reply[14]), "%f:%f:%f", loads[0], loads[1], loads[2]);
	int messageLen = strlen(reply);
	strcpy(&(reply[messageLen]), "</replyLoadAvg>");

	return reply;
}

/*
	 get* Name         : getErrorReply()
 * Description  :
 */
char *getErrorReply()
{
	char *reply = malloc(sizeof(char)*30);
	strcpy(reply, "<error>unknown format</error>");
	return reply;
}

/*
 * Name         : Create Server Socket
 * Description  : Creates and returns a server socket
 */
int CreateServerSocket(int listenPort)
{
	//get and print hostname
	char *hostname  = getServerHostName();
	printf("\nHostname: %s", hostname);
	printf("\nPortnum: %d", listenPort);
	fflush(stdout);             

	//get the server address for the network interface we want
	in_addr_t serverIP = getAddress(hostname);

	//get the socketAddress struct
	struct sockaddr_in *serverAddress = getServerAddressStruct(serverIP, listenPort);

	//create our socket to receive requests on
	int serverSocket = createServerSocket(serverAddress);

	return serverSocket;
}

char *getIPAddressString(struct sockaddr_in *address, char *buffer)
{
	bzero(buffer, 16*sizeof(char));
	inet_ntop(AF_INET, &(address->sin_addr.s_addr), buffer, 16);
	return buffer;

}

void printIPAddress(char* message, struct sockaddr_in *clientAddress)
{
	char clientIPString[200];
	if(inet_ntop(AF_INET, &(clientAddress->sin_addr.s_addr), clientIPString, 200) == NULL)
		exitError("Error printing IP Address");

	printf(message, clientIPString);
}

/*
 * Name         : Run Server
 * Description  : creates a server socket 
 */
/*void runServer(int serverSocket, void *(*requestHandleFunction), int maxRequests)*/
/*{*/

	/*//our client structures*/
	/*int *clientSocket;*/
	/*struct sockaddr_in *clientAddress;*/

	/*//main processing loop*/
	/*bool iCantStop = true;*/
	/*int requestCount = 0;*/
	/*while(iCantStop)*/
	/*{*/

		/*requestHandleFunction((void *)&serverSocket);*/

		/*//init things*/
		/*clientAddress = malloc(sizeof(struct sockaddr_in));*/
		/*clientSocket = malloc(sizeof(int));    */


		/*//should we stop?*/
		/*//If max is 0, we don't bother counting*/
		/*if(maxRequests != 0)*/
		/*{*/
			/*requestCount++;*/
			/*if(!(requestCount < maxRequests))*/
				/*iCantStop = false;*/
		/*}*/
	/*}*/
/*}*/

/*
 * Name         : Send UDP Packet
 * Description  : Sends a UDP packet
 */
int sendUDPPacket(int socket, struct sockaddr_in *address, char *data)
{

	printIPAddress("\nsending to: %s", address);
	fflush(stdout);

	if(sendto(socket, data, strlen(data)+1, 0, (struct sockaddr *)address, sizeof(struct sockaddr)) < 0)
		perror("\nsend() failed");
	return 0;
}

int getPortFromAddress(struct sockaddr_in *address)
{
	return ntohs(address->sin_port);
}

void printSocketStats(int socket)
{
	printIPAddress("\nIP Address: %s", getSocketAddress(socket)); 
	printf("\nPort Number: %d", getSocketPort(socket));
	fflush(stdout);

	return;
}

void printAddressStats(struct sockaddr_in *address)
{
	char ipBuffer[16];
	printf("\nIP Address: %s", getIPAddressString(address, ipBuffer)); 
	printf("\nPort Number: %d", getPortFromAddress(address));
	fflush(stdout);

	return;
}

//returns the length of the received message
int recvPacket(int socket, struct sockaddr_in *senderAddress, char *buffer)
{

	int messageSize;
	socklen_t addrlen = sizeof(*senderAddress);

	//if NULL, give dummy
	struct sockaddr_in foo;
	if(senderAddress == NULL)
		senderAddress = &foo;

	if(
			(messageSize = 
			 recvfrom(socket, buffer, MAX_SEGMENT_SIZE, 0, (struct sockaddr *)senderAddress, &addrlen)
			) < 0
		)
		exitError("\nrecvfrom() call failed");

	return messageSize;
}

bool compareAddresses(struct sockaddr_in *a, struct sockaddr_in *b)
{

	return (a->sin_addr.s_addr == b->sin_addr.s_addr
			&& a->sin_port == b->sin_port);

}
