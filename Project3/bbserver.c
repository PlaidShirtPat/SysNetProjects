#include "MyNetworking.h"

struct sockaddr_in *hostList[255];
int hostIndex = 0;

void *handleClientRequest( void *serverSocketArg){

  int serverSocket = *((int *)serverSocketArg);
  int messageSize;
  char buffer[MAX_MESSAGE_LENGTH+1];
  struct sockaddr_in *remoteaddress = malloc(sizeof(struct sockaddr_in));
  socklen_t addrlen = sizeof(*remoteaddress);
  
  if(
      (messageSize = 
       recvfrom(serverSocket, buffer, MAX_MESSAGE_LENGTH, 0, (struct sockaddr *)remoteaddress, &addrlen)
      ) < 0
  )
    exitError("\nrecvfrom() call failed");

  printIPAddress("\nclient address: %s", remoteaddress);
  
  //add it to the hosts list
  hostList[hostIndex] = remoteaddress; 
  hostIndex++;
  
  
  return 0;
}

void sendReplies(int serverSocket, int maxPeers)
{
  int i;
  for(i=0; i < maxPeers;i++)
  { 

    struct sockaddr_in *prevPeer, *nextPeer;

    //edge cases
    //peer is the first in the list


    if(maxPeers == 1){
      prevPeer = hostList[0];
      nextPeer = hostList[0];
    }
    else if(i==0){
      prevPeer = hostList[maxPeers-1];
      nextPeer = hostList[i+1];
    }
    //last peer
    else if(i==(maxPeers-1)){
      prevPeer = hostList[i-1];
      nextPeer = hostList[0];
    }
    //all others
    else{
      prevPeer = hostList[i-1];
      nextPeer = hostList[i+1];
    }

    char peerInfo[500] ; 
    //
    //format our data
    //<prevIP>;<prevPort>,<nextIP>;<nextPort>,<peerID>
    sprintf(peerInfo, "%s;%d,%s;%d,%d",
        getIPAddressString(prevPeer),
        getPortFromAddress(prevPeer),
        getIPAddressString(nextPeer),
        getPortFromAddress(nextPeer),
        i);

  if(sendto(serverSocket, peerInfo, strlen(peerInfo), 0, (struct sockaddr *)hostList[i], sizeof(*hostList[i])) < 0)
    perror("\nsend() failed");
  }
}

void checkInputs(int argc, char **argv){

  //Checking for proper number of arguments
  if (argc != 2){
      printf("\nError: There must be one argument\n<peerCount>\n");
      exit(EXIT_FAILURE);
  }

}

int main(int argc, char** argv) {    

  checkInputs(argc, argv);
  
  int numPeers = atoi(argv[1]);

  //Call all our functions to create a server socket
  int serverSocket = createServerSocket(getServerAddressStruct(getAddress(getServerHostName()), 51337));
  printf("server port:%d\n", getSocketPort(serverSocket));

  runServer(serverSocket, handleClientRequest, numPeers);

  sendReplies(serverSocket, numPeers);


  printf("\nI'll be back\n");
}


