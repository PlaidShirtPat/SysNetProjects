/*
 * File         : UDPserver.c
 * Author       : Patrick Brown, Michael Hood
 * Title        : UDP Server Example
 * Description  : 
 */

///
///Includes
///

#include <stdio.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <stdlib.h>   
#include <string.h>  
#include <unistd.h>  
#include <netdb.h>
#include <pthread.h>

///
///Macros
///

#define MAX_LEN 100
#define MAX_CONNECTIONS 20
#define MAX_MESSAGE_LENGTH 256
#define bool short
#define true 1
#define false 0

///
///Functions
///

/*
 * Name         : exitError()
 * Description  :
 */

void exitError(char *error)
{

    //printf("%s\n", error);
    perror(error);
    printf("\nShutting down...");
    exit(1);
}

/*
 * Name         : getAddress()
 * Description  :
 */

struct in_addr *getAddress(char *hostname){
  struct hostent *hostInfo = gethostbyname(hostname);           //Get host address by hostname
  struct in_addr **addrList = (struct in_addr**)(hostInfo->h_addr_list);        //Get host address from address list
  //return the first address
  return addrList[0];
}

/*
 * Name         : getServerHostName()
 * Description  :
 */

char *getServerHostName(){
  char *hostname;
  hostname = malloc(sizeof(char)*MAX_LEN);
  gethostname(hostname, MAX_LEN);
  return hostname;
}

/*
 * Name         : createServerSocket()
 * Description  :
 */

int createServerSocket(struct sockaddr_in*  serverAddress){
  int serverSocket;
  
  if((serverSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
    exitError("\nsocket() call failed");
  }
  if(bind(serverSocket, (struct sockaddr *) serverAddress, sizeof(*serverAddress)) < 0){
    exitError("\nbind() call failed");
  }
  
  return serverSocket;
}

/*
 * Name         : getServerAddressStruct()
 * Description  :
 */

struct sockaddr_in *getServerAddressStruct(struct in_addr *serverIP, int port){
  //create the socket address structure
  struct sockaddr_in *serverAddress;
  serverAddress = malloc(sizeof(struct sockaddr_in));
  //memset(serverAddress, 0, sizeof(serverAddress));
  serverAddress->sin_family = AF_INET;
  /*
  int i = port;
  printf("\nport test, original : %d,", i);
  i = htons(i);
  printf(", network byte order: %d", i);
  i = ntohs(i);
  printf(", back again : %d", i);
  */
  serverAddress->sin_port = htons(port);
  serverAddress->sin_addr.s_addr = *(in_addr_t *)serverIP;
  return serverAddress;
}

/*
 * Name         : getSocketPort()
 * Description  :
 */

int getSocketPort(int socket){
  struct sockaddr_in mySockAddr;
  socklen_t length = sizeof(mySockAddr);
  getsockname(socket, (struct sockaddr *) &mySockAddr, &length);
  return ntohs(mySockAddr.sin_port);
}

/*
 * Name         : getEchoReply()
 * Description  :
 */

char *getEchoReply(char *message){
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

char *getLoadAvgReply(){
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
 * Name         : getErrorReply()
 * Description  :
 */

char *getErrorReply(){
  char *reply = malloc(sizeof(char)*30);
  strcpy(reply, "<error>unknown format</error>");
  return reply;
}

/*
 * Name         : handleMessage()
 * Description  :
 */

char *handleMessage(char *message){
  char reply[MAX_MESSAGE_LENGTH+1];
  
  int messageLength = strlen(message);
  //check to see if the first 6 characters are <echo>
  if(strncmp(message, "<echo>", 6)==0){
  //check to see if the last 7 characters are </echo>
    if(strncmp(&(message[messageLength-7]), "</echo>", 7)==0)
      //we can't reply to a 256 size message because the reply will be 258 bytes 
      //due to difference in header/footer size
      if(messageLength > MAX_MESSAGE_LENGTH-2)
        return getErrorReply();
      else
        return getEchoReply(message);
  }
  
  //check to see if the message is <loadavg/>
  if(strncmp(message, "<loadavg/>", 10) == 0){  
      return getLoadAvgReply();
  }
  else if(strncmp(message, "<shutdown/>", 11) == 0){  //check to see if the message is <shutdown/>
      printf("\nShutting down...");
      exit(EXIT_SUCCESS);
  }
  else{
      return getErrorReply();
  }
  
  return NULL;
}

/*
 * Name         : handleClientRequest()
 * Description  :
 */

void *handleClientRequest(void *serverSocketArg){
  int serverSocket = *((int *)serverSocketArg);
  int messageSize;
  char buffer[MAX_MESSAGE_LENGTH+1];
  struct sockaddr_in remoteaddress;
  socklen_t addrlen = sizeof(remoteaddress);
  
  if((messageSize = recvfrom(serverSocket, buffer, MAX_MESSAGE_LENGTH, 0, (struct sockaddr *)&remoteaddress,
                &addrlen)) < 0){
      exitError("\nrecvfrom() call failed");
  }
  //create null terminator
  buffer[messageSize] = 0;
  printf("\nmessage:%s", buffer);    
  char* reply = handleMessage((char *)&buffer);

  if(reply == NULL)
    exitError("\ncould not handle request");

  printf("\nReply:%s", reply);    

  //UDP sendto using remoteaddress filled-in from recvfrom()
  if(sendto(serverSocket, reply, strlen(reply), 0, (struct sockaddr *)&remoteaddress, addrlen) < 0){
      perror("\nsend() to client failed");
  }

  free(reply);
  //free(serverSocketArg);
}

/*
 * Name         :
 * Description  :
 */

int main (int argc, char *argv[]){
    //Checking for proper number of arguments
    if (argc != 2){
        printf("\nError: There must be two arguments\n<cmd> <porttorecvfrom>\n");
        exit(EXIT_FAILURE);
    }
    //Check port number for validity
    if (atoi(argv[1]) < 0 || atoi(argv[1]) > 65535){
        printf("\nError: The port # must be between 0 and 65535");
        exit(EXIT_FAILURE);
    }
    close(atoi(argv[1]));
  printf("\nHere we go");
  
  //get and print hostname
  char *hostname  = getServerHostName();
  printf("\nHostname: %s", hostname);
  printf("\nPortnum: %d", atoi(argv[1]));
  fflush(stdout);               //Buffer flush
  //get the server address for the network interface we want
  struct in_addr *serverIP = getAddress(hostname);

  //get the socketAddress struct
  struct sockaddr_in *serverAddress = getServerAddressStruct(serverIP, atoi(argv[1]));

  //create our socket to receive requests on
  int serverSocket = createServerSocket(serverAddress);
 
  //our client structures
  int *clientSocket;
  struct sockaddr_in *clientAddress;
  socklen_t clientAddressLength = sizeof(struct sockaddr_in);

  bool iCantStop = true;
  while(iCantStop){
    handleClientRequest((void *)&serverSocket);
    //init things
    clientAddress = malloc(sizeof(struct sockaddr_in));
    clientSocket = malloc(sizeof(int));    

    char clientIPString[200];
    inet_ntop(AF_INET, &(clientAddress->sin_addr.s_addr), clientIPString, 200);
    printf("\nclient address: %s", clientIPString);
  }
  return 0;
}

/*
 * Name         :
 * Description  :
 */