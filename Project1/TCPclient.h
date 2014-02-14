/*
 * TCPclient.h
 * Systems and Networks II
 * Project 1
 *
 * This file describes the functions to be implemented by the TCPclient.
 * You may also implement any auxillary functions you deem necessary.
 */

#include  "headers.h"

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

int createSocket(char * serverName, int port, struct sockaddr_in *dest);

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
 * Checks the server name for validity
 * 
 * return - 0 if valid, -1 if an invalid character is detected 
 * 
 */

int checkServerName(char *serverName);

/*
 * Copies the string starting at otherptr into the string ptr until a '\0' is encountered in otherptr.
 * ptr (field 1) receives otherptr (field 2)
 * 
 */

void copyString(char *ptr, char *otherptr, int *offset1);

/*
 * Checks the responses from the server for <error>, <reply>, and <replyLoadAvg> head and foot tags
 * 
 * foottaglength - the length of the foot tag to be checked (i.e., </reply> is 8)
 * offset - the offset pointer from the response
 * message - the display message for formatting the information contained by the server response
 * response - the unparsed response as received from the server
 * headtag - the BNF head tag to check for
 * foottag - the BNF foot tag to check for
 */

void checkResponse(int foottaglength, int offset, const char *message, 
        char *response, const char *headtag, const char *foottag);