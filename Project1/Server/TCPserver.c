#include <stdio.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <stdlib.h>   
#include <string.h>  
#include <unistd.h>  
#include <netdb.h>
#include <pthread.h>

#define MAX_LEN 100
#define MAX_CONNECTIONS 20
#define MAX_MESSAGE_LENGTH 256
#define bool short
#define true 1
#define false 0




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

int getSocketPort(int socket)
{
  struct sockaddr_in mySockAddr;
  socklen_t length = sizeof(mySockAddr);
  getsockname(socket, (struct sockaddr *) &mySockAddr, &length);
  return ntohs(mySockAddr.sin_port);
}




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

char *getLoadAvgReply()
{
  char *reply = malloc(sizeof(char) * MAX_MESSAGE_LENGTH+1);

  double loads[3];
  getloadavg(loads, 3);



  //copy the message starting at the 7th spot in reply, leaving room for header <reply> and footer 
  strcpy(&(reply[0]), "<replyLoadAvg>");
  sprintf(reply, "%f:%f:%f", loads[0], loads[1], loads[2]);
  int messageLen = strlen(reply);
  strcpy(&(reply[messageLen]), "</replyLoadAvg>");

  return reply;
}

char *getErrorReply()
{
  char *reply = malloc(sizeof(char)*30);
  strcpy(reply, "<error>unknown format</error>");
  return reply;
}
char *handleMessage(char *message)
{
  char reply[MAX_MESSAGE_LENGTH+1];
  
  int messageLength = strlen(message);
  //check to see if the first 6 characters are <echo>
  if(strncmp(message, "<echo>", 6)==0)
  {
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
  if(strcmp(message, "<loadavg/>")==0)
  {  
      return getLoadAvgReply();
  }
  else
    return getErrorReply();


  return NULL;

}

void *handleClientRequest(void *clientSocketArg)
{
  int clientSocket = *((int *)clientSocketArg);
  int messageSize;
  char buffer[MAX_MESSAGE_LENGTH+1];
  if((messageSize = recv(clientSocket, buffer, MAX_MESSAGE_LENGTH, 0))<0)
      exitError("recv() call failed");
  
  //create null terminator
  buffer[messageSize] = 0;



  printf("message:%s\n", buffer);    
  

  char* reply = handleMessage((char *)&buffer);

  if(reply == NULL)
    exitError("could not handle request");

  printf("reply:%s\n", reply);    

  if( send(clientSocket, reply, strlen(reply), 0) < 0)
    perror("send() to client failed");


  close(clientSocket);
  pthread_exit(NULL);
}


int main (int argc, char *argv[])
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


  //our client structures
  int clientSocket;
  struct sockaddr_in *clientAddress;
  socklen_t clientAddressLength = sizeof(struct sockaddr_in);

  if(listen(serverSocket, MAX_CONNECTIONS) < 0)
    exitError("listen() on server socket failed");

  bool iCantStop = true;
  while(iCantStop){
    
    //init things
    clientAddress = malloc(sizeof(struct sockaddr_in));
    
    //a-a-a-a-a-accept the connection
    if((clientSocket = accept(
      serverSocket,
      (struct sockaddr *) clientAddress,
      &clientAddressLength
    ))<0)
      exitError("accept() call failed");
    

    char clientIPString[200];
    inet_ntop(AF_INET, &(clientAddress->sin_addr.s_addr), clientIPString, 200);
    printf("client address: %s\n", clientIPString);

    //start thread

    pthread_t thread;
    if(pthread_create(&thread, NULL, handleClientRequest, (void *)&clientSocket))
      exitError("thread creation failed");
  }


  return 0;
}
