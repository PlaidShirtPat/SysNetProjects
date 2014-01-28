/* 
 * File         : main.c
 * Author       : Michael Hood, Patrick Brown
 * Title        : Systems and Networks II - Project 1
 * Description  : Client
 *
 * Created on January 26, 2014, 2:04 PM
 */

///
///Includes
///

#include "headers.h"
#include "TCPclient.h"

///
///Prototypes
///

/*
 * Name : main()
 * Description : 
 */

int main(int argc, char** argv) {    
    //Checking for proper number of arguments
    if (argc != 3){
        printf("Error: There must be three arguments\n<cmd> <hostname> <port>\n");
        exit(EXIT_FAILURE);
    }
    //Check port number for validity
    if (atoi(argv[2]) < 0 || atoi(argv[2]) > 65535){
        printf("Error: The port # must be between 0 and 65535\n");
        exit(EXIT_FAILURE);
    }
    //Check hostname for validity
    int charnum = 0;    //Holds the array offset for the hostname validity check
    do{
        if ( ( *(argv[1]+charnum) >= 97 && *(argv[1]+charnum) <= 122) ||        //Chars a-z
             ( *(argv[1]+charnum) >= 65 && *(argv[1]+charnum) <= 90) ||     //Chars A-Z
             ( *(argv[1]+charnum) >=48 && *(argv[1]+charnum) <=57) ||       //Chars 0-9
             ( *(argv[1]+charnum) == 45)                                 //The '-' char
                ){
            ;
        }
        else{
            printf("\nError: Invalid hostname\nMust include only characters between",
                    "a-z, A-Z, 0-9, and -\n");
            exit(EXIT_FAILURE);
        }
        charnum++;
    }while(*(argv[1]+charnum) != '\0');
    
    int s;         //Socket descriptor
    int n;              //Number of bytes in each recv call
    char* servName;     //Server name
    int servPort;       //Server port number
    char* string;       //String to be echoed
    int len;            //Length of string to be echoed
    char buffer[256 + 1];       //Buffer
    char* ptr = buffer;         //Pointer to move along the buffer
    struct sockaddr_in serverSocketAddr;      //Server socket address
    
    //TCPclient.c functions
    
    // int createSocket(char * serverName, int port, struct sockaddr_in * dest);
    //createSocket(); //Create a socket and connect to the server at a specified location
    
    // int sendRequest(int sock, char * request, struct sockaddr_in * dest);
    //sendRequest(); //Send a request to the server (ser entered or a fixed message)
    
    // int receiveResponse(int sock, char * response);
    //receiveResponse(); //Receive a reply from the server and display it in its entire length
    
    // int closeSocket(int sock);
    //closeSocket();      //Close the socket and terminate the program

    
    //Create a TCP socket
    s = socket(AF_INET, SOCK_STREAM, 0); //Creates the TCP socket (Stream), gets the socket file descriptor
    if (s < 0){
        perror("Error: Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    //Get info about destination host (server/recipient/other host/etc.)
    struct hostent *hostptr;
    hostptr = gethostbyname(argv[1]); //Gets the hostent struct from the cmd line arg
    if (hostptr == NULL){
        perror("Error: Getting host name failed");
        exit(EXIT_FAILURE);
    }
    memset((void *) &serverSocketAddr, 0, (size_t)sizeof(serverSocketAddr)); //Clears the server socket address struct with 0s
    
    //Fill in destination address structure (3 fields, other 2 not used)
    serverSocketAddr.sin_family = (short)(AF_INET);     //Fills in socket family (IPv4))
    memcpy((void *)&serverSocketAddr.sin_addr,
           (void *)hostptr->h_addr, hostptr->h_length); //Fills in the host address
    serverSocketAddr.sin_port = htons((uint16_t)atoi(argv[2]));//Fills in the port number (host to network short)
    
    
    //Connect to server
    if(connect(s, (struct sockaddr *)&serverSocketAddr, sizeof(serverSocketAddr)) < 0){
        perror("Error:Connection or binding failed");
        exit(EXIT_FAILURE);
    }
    
    //Get messages from user
    printf("\n>");
    fgets(buffer, strlen(buffer)-1, stdin)
    if(buffer == NULL){
        perror("Error: Invalid message\n");
    }
            
    //Send data to destination
    write(s, buffer, strlen(buffer));
    
    //Read data from destination
    bzero(buffer, 256); //instead of memset
    read(s, buffer, 256);
    
    //Close socket
    close(s);

    return (EXIT_SUCCESS);
}

///
///Functions
///

/*
 * Name : createSocket()
 * Description : 
 */

/*
 * Creates a streaming socket and connects to a server.
 *
 * serverName - the ip address or hostname of the server given as a string
 * port       - the port number of the server
 * dest       - the server's address information; the structure should be created with information
 *              on the server (like port, address, and family) in this function call
 *
 * return value - the socket identifier or a negative number indicating the error if a connection could not be established
 */

int createSocket(char * serverName, int port, struct sockaddr_in * dest){
    
}

/*
 * Name : sendRequest()
 * Description : 
 */

/*
 * Sends a request for service to the server. This is an asynchronous call to the server, 
 * so do not wait for a reply in this function.
 * 
 * sock    - the socket identifier
 * request - the request to be sent encoded as a string
 * dest    - the server's address information
 *
 * return   - 0, if no error; otherwise, a negative number indicating the error
 */
int sendRequest(int sock, char * request, struct sockaddr_in * dest);

/*
 * Receives the server's response formatted as an XML text string.
 *
 * sock     - the socket identifier
 * response - the server's response as an XML formatted string to be filled in by this function;
 *            memory is allocated for storing response
 *
 * return   - 0, if no error; otherwise, a negative number indicating the error
 */
int receiveResponse(int sock, char * response);

/*
 * Prints the response to the screen in a formatted way.
 *
 * response - the server's response as an XML formatted string
 *
 */
void printResponse(char* response);

/*
 * Closes the specified socket
 *
 * sock - the ID of the socket to be closed
 * 
 * return - 0, if no error; otherwise, a negative number indicating the error
 */
int closeSocket(int sock);



/*
 * Name : 
 * Description : 
 */