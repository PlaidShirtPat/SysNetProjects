#include <stdio.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <stdlib.h>   
#include <string.h>  
#include <unistd.h>  
#include <netdb.h>

#define MAX_LEN 100



void exitError(char *error)
{

    //printf("%s\n", error);
    perror(error);
    printf("Shutting down\n");
    exit(1);
}

struct in_addr *getAddress(char *hostname)
{
  struct hostent *hostInfo = gethostbyname(hostname);
  int i =0;

  struct in_addr **addrList = (struct in_addr**)(hostInfo->h_addr_list);

  //return the first address
  return addrList[0];
}

char *getServerHostName()
{
  char *hostname;
  hostname = malloc(sizeof(char)*MAX_LEN);
  gethostname(hostname, MAX_LEN);
  return hostname;
}


int createServerSocket(struct sockaddr_in*  serverAddress)
{


  int serverSocket;
  if((serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    exitError("socket() call failed");


  if(bind(
        serverSocket, 
        (struct sockaddr *) serverAddress, 
        sizeof(*serverAddress)
  )<0)
    exitError("bind() call failed");
  return serverSocket;

}

struct sockaddr_in *getServerAddressStruct(struct in_addr *serverIP)
{
  //create the socket address structure
  struct sockaddr_in *serverAddress;
  serverAddress = malloc(sizeof(struct sockaddr_in));
  //memset(serverAddress, 0, sizeof(serverAddress));
  serverAddress->sin_family = AF_INET;
  serverAddress->sin_port = 0;
  serverAddress->sin_addr.s_addr = *(in_addr_t *)serverIP;
  return serverAddress;
}

short getSocketPort(int socket)
{
  struct sockaddr_in mySockAddr;
  socklen_t length = sizeof(mySockAddr);
  getsockname(serverSocket, (struct sockaddr *) &mySockAddr, &length);
  return ntohs(mySockAddr.sin_port);
}

int main (int argc, char *argv)
{
  printf("Here we go\n");
  
  //get and print hostname
  char *hostname  = getServerHostName();
  printf("hostname: %s\n", hostname);

  //get the server address for the network interface we want
  struct in_addr *serverIP = getAddress(hostname);

  //get the socketAddress struct
  struct sockaddr_in *serverAddress = getServerAddressStruct(serverIP);

  //create our socket to recive requests on
  int serverSocket = createServerSocket(serverAddress);

  printf("portnum: %d\n", getSocketPort(serverSocket));


  int clientSocket;
  struct sockaddr_in clientAddress;
  int clientAddressLength = sizeof(clientAddress);

  if((clientSocket = accept(
    serverSocket,
    (struct sockaddr *) &clientAddress,
    &clientAddressLength
  )<0))
    exitError("accept() call failed");

  printf("got something!\n");


  return 0;
}
