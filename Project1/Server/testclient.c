#include <stdio.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <stdlib.h>   
#include <string.h>  
#include <unistd.h>  
#include <netdb.h>

#define MAX_LEN 100
#define MAX_CONNECTIONS 20

struct in_addr *getAddress(char *hostname)
{
  struct hostent *hostInfo = gethostbyname(hostname);
  int i =0;

  struct in_addr **addrList = (struct in_addr**)(hostInfo->h_addr_list);

  //return the first address
  return addrList[0];
}

int main (int argc, char *argv[])
{

  char *hostname = argv[1];
  char *portString = argv[2];
  unsigned short serverPort = atoi(portString);
  

  int sendSock;

  sendSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  printf("server host name: %s\n", hostname);
  printf("server addr: %s\n", getAddress(hostname);


  struct sockaddr_in servAddr;
  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(serverPort);
  servAddr.sin_addr = *getAddress(hostname); 

  if( connect(sendSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) <0)
    perror("conn sucks");
  if( send(sendSock, "hello world", strlen("hello world"), 0) < 0)
    perror("send sucks");

  

  return 0;
}
