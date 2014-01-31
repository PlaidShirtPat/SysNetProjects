/* 
 * File         : TCPclient.c
 * Author       : Michael Hood
 * Title        : Systems and Networks II - Project 1
 * Description  : Client c file
 *
 */

#include "headers.h"
#include "TCPclient.h"

/*
 * Name        : createSocket()
 * Description : 
 *
 * Creates a streaming socket and connects to a server.
 *
 * serverName - the ip address or hostname of the server given as a string
 * port       - the port number of the server
 * dest       - the server's address information; the structure should be created with information
 *              on the server (like port, address, and family) in this function call
 *
 * return value - the socket identifier or a negative number indicating the error if a connection could not be established
 */

int createSocket(char *serverName, int port, struct sockaddr_in *dest){
    //Check port number for validity
    if (port < 0 || port > 65535){
        printf("\nError: The port # must be between 0 and 65535");
        return -1;
    }
    //Check server name for validity
    if(checkServerName(serverName) < 0){
        printf("\nError: The server name has invalid characters");
        return -1;
    }
    
    //Create a TCP socket
    int s = socket(AF_INET, SOCK_STREAM, 0); //Creates the TCP socket (Stream), gets the socket file descriptor
    //Check if valid sfd
    if (s < 0){
        perror("\nError: Socket creation failed");
        return -1;
    }
    
    //Get info about destination host (server/recipient/other host/etc.)
    struct hostent *hostptr;
    hostptr = gethostbyname(serverName); //Gets the hostent struct from the cmd line arg
    //Check if valid info was given by gethostbyname()
    if(hostptr == NULL){
        perror("\nError: Getting host name failed");
        return -1;
    }
    memset((void *) dest, 0, (size_t)sizeof(dest)); //Clears the server socket address struct with 0s
   
    //Fill in destination address structure (3 fields, other 2 not used)
    dest->sin_family = (sa_family_t)(AF_INET);     //Fills in socket family (IPv4))
    memcpy((void *)&dest->sin_addr,
           (void *)hostptr->h_addr, hostptr->h_length); //Fills in the host address
    dest->sin_port = htons((uint16_t)port);//Fills in the port number (host to network short)
    
    //Connect to server
    if(connect(s, (struct sockaddr *)dest, sizeof(*dest)) < 0){
        perror("\nError:Connection or binding failed");
        return -1;
    }
    return s;
}

/*      
 * Name        : sendRequest()
 * Description : 
 *
 * Sends a request for service to the server. This is an asynchronous call to the server, 
 * so do not wait for a reply in this function.
 * 
 * sock    - the socket identifier
 * request - the request to be sent encoded as a string
 * dest    - the server's address information
 *
 * return   - 0, if no error; otherwise, a negative number indicating the error
 */

int sendRequest(int sock, char *request, struct sockaddr_in *dest){
    //If the msg is not loadavg, pack the msg into BNF tags <echo> and </echo>
    if(strncmp(request, "<loadavg/>", 10) != 0){
        char msghead[7] = {'<','e','c','h','o','>','\0'};
        char msgfoot[8] = {'<','/','e','c','h','o','>','\0'};
        char holder[256+1]; //Holds the message as it's being built
        int offset1 = 0;  //Pointer offsets for traversing the holder
        copyString(holder, msghead, &offset1);  //Copy the msghead into the holder
        copyString(holder, request, &offset1);   //Copy the message buffer into the holder
        copyString(holder, msgfoot, &offset1);  //Copy the msgfoot into the holder
        //Copy the entire built message back into buffer so that two sets of send/recv pairs don't need to be written
        offset1 = 0;             //Reset the offset
        copyString(request, holder, &offset1);
    }
    //Send data to destination
    if(send(sock, request, strlen(request), 0) < 0){
        perror("\nError: Send failure");
        return -1;
    }
    return 0;
}

/*
 * Name        : receiveResponse()
 * Description : 
                                                                                                 *
 * Receives the server's response formatted as an XML text string.
 *
 * sock     - the socket identifier
 * response - the server's response as an XML formatted string to be filled in by this function;
 *            memory is allocated for storing response
 *
 * return   - 0, if no error; otherwise, a negative number indicating the error
 */

int receiveResponse(int sock, char *response){
    //Read data from destination
    bzero(response, 256);               //Zero out the response for receiving
    char *ptr = response;               //Pointer for traversing the response
    //Do recv and error checking on it
    if (recv(sock, (void *)ptr, 256, 0) < 0){
        perror("\nError: Receive failed");
        return -1;
    }
    //Check if reply tags
    checkResponse(8, 7, "Message: ", response, "<reply>", "</reply>");
    //Check if load average tags
    checkResponse(15, 14, "Load average - 1 min, 5 min, 15 min\n", response, "<replyLoadAvg>", "</replyLoadAvg>");
    //Check if error tags
    checkResponse(8, 7, "Error: ", response, "<error>", "/error>");
    return 0;
}

/*
 * Name        : printResponse()
 * Description : 
 *
 * Prints the response to the screen in a formatted way.
 *
 * response - the server's response as an XML formatted string
 *
 */

void printResponse(char* response){
    printf("\n%s", response);
}

/*
 * Name        : closeSocket()
 * Description : 
 *
 * Closes the specified socket
 *
 * sock - the ID of the socket to be closed
 * 
 * return - 0, if no error; otherwise, a negative number indicating the error
 */
int closeSocket(int sock){
    //Close socket
    if (close(sock) < 0){
        perror("\nError: Socket close failed");
        return -1;
    }
    return 0;
}

/*
 * Name        : checkServerName()
 * Description : Checks the server name for valid characters.
 * Returns     : 0 if valid, -1 if an invalid character is detected 
 */

int checkServerName(char *serverName){
    //Check server name for validity
    int charnum = 0;    //Holds the array offset for the hostname validity check
    do{
        if ( ( *(serverName+charnum) >= 97 && *(serverName+charnum) <= 122) ||        //Chars a-z
             ( *(serverName+charnum) >= 65 && *(serverName+charnum) <= 90) ||     //Chars A-Z
             ( *(serverName+charnum) >=48 && *(serverName+charnum) <=57) ||       //Chars 0-9
             ( *(serverName+charnum) == 45)                                 //The '-' char
                ){
            ;
        }
        else{
            printf("\nError: Invalid hostname\nMust include only characters between a-z, A-Z, 0-9, and -\n");
            return -1;
        }
        charnum++;              //Increase the offset for checking the serverName validity
    }while(*(serverName+charnum) != '\0');
    return 0;
}

/*
 * Name        : copyString()
 * Description : Copies the string starting at otherptr into the string ptr until a '\0' is encountered in otherptr.
 *               ptr (field 1) receives otherptr (field 2)
 */

void copyString(char *ptr, char *otherptr, int *offset1){
    int offset2 = 0;
    while(*(otherptr + offset2) != '\0'){
            *(ptr + *offset1) = *(otherptr + offset2);          //Copies the character
            offset2++;                                          //Offset of giving string
            (*offset1)++;                                       //Offset of receiving string
    }
    *(ptr + *offset1) = '\0';                                   //Add null character to end
}

/*
 * Name        : checkResponse()
 * Description : Checks the response
 */

void checkResponse(int foottaglength, int offset, const char *message, 
        char *response, const char *headtag, const char *foottag){
    int endoffset = strlen(response) - foottaglength; //End of msg body before </reply> foot
    char *foot = response + strlen(response) - foottaglength;
    
    //If the tag on the received message matches what we're looking for
    if(!strncmp(response, headtag, offset) && !strncmp(foot, foottag, foottaglength)){
        printf("\n%s", message);               //Print the information message about the received message
        while(offset != endoffset){
            printf("%c", *(response + offset)); //Print the response
            offset++;
        }
    }
    
}

/*
 * Name        : 
 * Description : 
 */
