/* 
 * File         : UDPmain.c
 * Author       : Michael Hood, Patrick Brown
 * Title        : Systems and Networks II - Project 2
 * Description  : Client
 *
 * Created on January 26, 2014, 2:04 PM
 */

///
///Includes
///

#include "headers.h"
#include "UDPclient.h"

///
///Functions
///

/*
 * Name : main()
 * Description : Main program logic control source file
 */

int main(int argc, char** argv) {    
    //Checking for proper number of arguments
    if (argc != 3){
        printf("\nError: There must be three arguments\n<cmd> <hostname> <port>");
        exit(EXIT_FAILURE);
    }
    
    int s;              //Socket file descriptor
    char buffer[256 + 1];       //Buffer
    struct sockaddr_in *dest = malloc(sizeof(struct sockaddr_in));    //Server socket address
    
    //Create a socket and connect to the server at a specified location
    s = createSocket(argv[1], atoi(argv[2]), dest);
    if (s < 0){
        exit(EXIT_FAILURE);
    }
    
    printf("\n>");                      //Print message prompt
    scanf("%s", buffer);                //Get message from user
    
    //Send a request to the server
    if (sendRequest(s, buffer, dest) < 0){
        exit(EXIT_FAILURE);
    }
    
    //Receive a reply from the server and display it in its entire length
    if (receiveResponse(s, buffer) < 0){
      exit(EXIT_FAILURE);  
    }
    
    //Close the socket and terminate the program
    if(closeSocket(s) < 0){
        exit(EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}


/*
 * Name        : 
 * Description : 
 */