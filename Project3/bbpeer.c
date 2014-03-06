
#include "MyNetworking.h"
#include "fileio.h"


//const
#define MAX_WRITE_SIZE 601

//command types
#define bbWRITE 0
#define bbREAD 1
#define bbEXIT 2

struct bbCommand{
  int commandType;
  char *message; 
  int readNumber;
};

struct sockaddr_in *myAddress;
struct sockaddr_in *prevPeer;
struct sockaddr_in *nextPeer;
int myPeerID;
FILE *bbFP;


void *handleClientRequest( void *serverSocketArg){

  int serverSocket = *((int *)serverSocketArg);
  int messageSize;
  char buffer[MAX_MESSAGE_LENGTH+1];
  struct sockaddr_in remoteaddress;
  socklen_t addrlen = sizeof(remoteaddress);
  
  if(
      (messageSize = 
       recvfrom(serverSocket, buffer, MAX_MESSAGE_LENGTH, 0, (struct sockaddr *)&remoteaddress, &addrlen)
      ) < 0
  )
    exitError("\nrecvfrom() call failed");
  
  //create null terminator
  buffer[messageSize] = 0;
  printf("\nmessage:%s", buffer);    
  
  char *remoteAddress = "142.45.6.5";

  
  char* reply = "hitherepopo";

  if(reply == NULL)
    exitError("\ncould not handle request");

  printf("\nReply:%s", reply);    

  //UDP sendto using remoteaddress filled-in from recvfrom()
  if(sendto(serverSocket, reply, strlen(reply), 0, (struct sockaddr *)&remoteaddress, addrlen) < 0)
    perror("\nsend() to client failed");
  
  free(reply);
  return 0;
}

void checkInputs(int argc, char **argv){

  //Checking for proper number of arguments
  if (argc != 3){
      printf("\nError: There must be two arguments\n<bbserverName>, <bbseverPort>\n");
      exit(EXIT_FAILURE);
  }
  //Check port number for validity
  if (atoi(argv[2]) < 0 || atoi(argv[2]) > 65535){
      printf("\nError: The port # must be between 0 and 65535");
      exit(EXIT_FAILURE);
  }

}

void sendToNext(int socket, char *token){
  sendUDPPacket(socket, nextPeer, token);
}

struct bbCommand *getInput()
{
  char *commandBuffer = malloc(sizeof(char) *MAX_WRITE_SIZE);
  struct bbCommand *command = malloc(sizeof(struct bbCommand));

  bool invalid = true;
  while (invalid){
    printf("\nPlease enter a command (write|read|exit): ");
    fgets(commandBuffer, MAX_WRITE_SIZE, stdin);

    if(strcmp(commandBuffer, "write\n") == 0)
    {
      command->commandType = bbWRITE;

      printf("\nPlease enter your message (max message size: %d characters): ", MAX_WRITE_SIZE-1);
      fgets(commandBuffer, MAX_WRITE_SIZE, stdin);
      command->message = commandBuffer;

      invalid = false;
    }
    else if(strcmp(commandBuffer, "read\n") == 0){
      command->commandType = bbREAD;

      //get a valid number
      bool numInvalid = true;
      while(numInvalid){
        printf("\nWhat message would you like to read? (valid message range is:1-%d): ", findLastMessage(bbFP));
        fgets(commandBuffer, MAX_WRITE_SIZE, stdin);
        command->readNumber = atoi(commandBuffer);
        if(command->readNumber <= findLastMessage(bbFP) || command->readNumber > 0)
          numInvalid = false;
        else 
          printf("\nThat is not a valid message number.");
      }
      invalid = false;

    }
    else if(strcmp(commandBuffer, "exit\n") == 0){
      command->commandType = bbEXIT;
      invalid = false;

    }
    else
      printf("\nInvalid command");
  }
  return command;
}

void exitFromRing(int receiveSocket, char *token){
  //append "<prevIP;port>,<myIP;port>,<nextIP,port>:" to the token string
  sprintf(token, "%s%s;%d,%s;%d,%s;%d:", 
    token, 
    getIPAddressString(prevPeer),
    getPortFromAddress(prevPeer),
    getIPAddressString(myAddress),
    getPortFromAddress(myAddress),
    getIPAddressString(nextPeer),
    getPortFromAddress(nextPeer)
  );

  sendToNext(receiveSocket, token);
}

void checkForDropNotices(){

}


void startTokenRing(int receiveSocket){

  char token[5000] = "tok:";


  int messageSize;
  char buffer[MAX_MESSAGE_LENGTH+1];
  struct sockaddr_in remoteaddress;
  socklen_t addrlen = sizeof(remoteaddress);

  bool iCantStop = true;
  bool firstTime = true;
  while(iCantStop == true){

    struct bbCommand *command;
    if(myPeerID == 0 && firstTime ) 
    {
      firstTime = false;
      //initiate the board
      bbFP = createFile();
      writeMsg(bbFP, "Welcome to the message board!");

      command = getInput();

      if(command->commandType == bbWRITE){
        writeMsg(bbFP, command->message);
        printf("\nMessage written\nmessage:%s",command->message);
      }
      else if(command->commandType == bbREAD) {
        char readBuffer[MAX_WRITE_SIZE];
        readMsg(bbFP, command->readNumber, readBuffer);
        printf("\nMessage: %s\n", readBuffer);
      }
      else if(command->commandType == bbEXIT){
        exitFromRing(receiveSocket,token);
        printf("\nShutting down...\n");
        exit(0);
      }

      if(iCantStop)
        sendToNext(receiveSocket, token);
    }

    printf("\nwaiting on token...");
    fflush(stdout);
    if( 
        (messageSize = recvfrom(receiveSocket, token, 5000, 0, (struct sockaddr *)&remoteaddress, &addrlen)) 
    < 0)
      exitError("\nrecvfrom() call failed for token");

    checkForDropNotices();
  
    
    command = getInput();
    if(command->commandType == bbWRITE){
      writeMsg(bbFP, command->message);
      printf("\nMessage written\nmessage:%s",command->message);
    }
    else if(command->commandType == bbREAD) {
      char readBuffer[MAX_WRITE_SIZE];
      readMsg(bbFP, command->readNumber, readBuffer);
      printf("\nMessage: %s\n", readBuffer);
    }
    else if(command->commandType == bbEXIT){
      exitFromRing(receiveSocket,token);
      printf("\nShutting down...\n");
      exit(0);
    }
    
    if(iCantStop)
      sendToNext(receiveSocket, token);
     
    free(command->message);
    free(command);
  }
}

int main(int argc, char** argv) {    
  checkInputs(argc, argv);

  close(atoi(argv[2]));

  int serverPort = atoi(argv[2]);
  struct sockaddr_in *serverAddress =  getServerAddressStruct(getAddress(argv[1] ),serverPort);

  printIPAddress("Server IP Address:%s\n",serverAddress);

  //Call all our functions to create a server socket
  myAddress = getServerAddressStruct(getAddress(getServerHostName()), 0);
  int receiveSocket = createServerSocket(myAddress);
  
  sendUDPPacket(receiveSocket, serverAddress, "Sup?");

  int messageSize;
  char buffer[MAX_MESSAGE_LENGTH+1];
  struct sockaddr_in remoteaddress;
  socklen_t addrlen = sizeof(remoteaddress);

  if(
      (messageSize = 
       recvfrom(receiveSocket, buffer, MAX_MESSAGE_LENGTH, 0, (struct sockaddr *)&remoteaddress, &addrlen)
      ) < 0
  )
    exitError("\nrecvfrom() call failed");

  char *prevPeerString = strtok(buffer, ",");
  char *nextPeerString = strtok(NULL, ",");
  char *myPeerIdString = strtok(NULL, ",");

  printf("Peer ID: %d\nprevious peer: %s\nnext peer: %s\n", myPeerID, prevPeerString, nextPeerString);
  
  //get previous and next host
  char *ipString = strtok(prevPeerString, ";");
  int portNum = atoi(strtok(NULL, ";"));
  prevPeer = getServerAddressStruct(getAddress(ipString), portNum);

  ipString = strtok(nextPeerString, ";");
  portNum = atoi(strtok(NULL, ";"));
  nextPeer = getServerAddressStruct(getAddress(ipString), portNum);

  myPeerID = atoi(myPeerIdString);
  
  startTokenRing(receiveSocket);

  printf("I'll be back\n");
}
