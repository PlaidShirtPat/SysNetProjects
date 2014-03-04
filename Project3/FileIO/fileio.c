/* 
 * File         : fileio.c
 * Author(s)    : Michael Hood, Patrick Brown
 * Project name : Systems and Networks II - Dr. Reichherzer - Spring 2014 - Project 3
 * Description  : This provides prototypes for the file IO functions for the token ring message board.
 * Notes        : This implementation uses fixed-size (160 characters) messages to simplify the reading/writing process and
 *                so that when retrieved a message can easily be stored in a string to be returned.
 *                Messages are 160 characters (including whitespace characters and CR and LF) because SMS messages 
 *                are that length.
 * Created on March 2, 2014, 5:53 PM
 */

///
///Includes
///

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileio.h"

///
///Functions
///

/*
 * Name         : createFile()
 * Description  : Opens and/or creates a file called "board.txt" for appending, reading, and writing. 
 *                Output operations append data to the end of the file. Can use fseek, fsetpos, and rewind 
 *                if necessary with "a+" modifier on fopen().
 */

FILE* createFile(){
    FILE * fp;
    fp = fopen("msgbrd.txt", "a+");      //Create file for append (reading and writing)
    //If file pointer is null after fopen(), print error and exit with failure
    if (fp == NULL){
        perror("\nFile creation error : ");
        exit(EXIT_FAILURE);
    }
    return fp;          //Return the file pointer
}

/*
 * Name         : findLastMessage()
 * Description  : Scans through the open file to find the message number of the very last message in the
 *                message board file.
 * Returns      : An int of the very last message in the shared, message-board file, a 0 if no msgs in file
 */

int findLastMessage(FILE *fp){
    fseek(fp, 0, SEEK_SET);     //Sets the file pointer to the beginning of the file
    int status;         //Holds each upcast-to-int character read from the file, an EOF or an '<' triggers an event
    char string[11] = {'m', 'e', 's', 's', 'a', 'g', 'e', ' ', 'n', '=', '\0'}; //Comparison string to check for
    char check[11];     //The sequence of characters grabbed from file that should hold the <message> tag for comparison
    char number[10] = {'0', '\0'};    //Holds the number read from the messages, starts at 0
    do{
        status = fgetc(fp);     //Grab a character
        if (status == -1) break;        //If there's nothing in the file, break the loop
        //If there is a less than character, check for <message n=[number]> tag
        if((char)status == '<'){
            int checkpos = 0;   //Holds the position in the checking string (characters grabbed from file)
            check[checkpos++] = (char)fgetc(fp); //Get next character
            //If the next character is not a '/' as in </message>, then continue checking
            if(check[0] != '/'){
                //Grab the next 9 characters
                for(; checkpos <= 9; checkpos++){
                    check[checkpos] = (char)fgetc(fp); //Get next character
                }
                check[10] = '\0';       //Set the null character at the end of the check string
                //If the string matches an opening <message> tag, then read the number, else continue looking for 
                //next message number
                if (!strcmp(check, string)){
                    int i = 0;
                    char chold = fgetc(fp);     //Grab next char
                    while(chold != '>'){
                        number[i++] = chold;    //Put char into number[]
                        chold = fgetc(fp);      //Grab next char
                    }
                    number[i++] = '\0';         //Adds in a null character after the number
                }
            }
        }
    }while(status != EOF);
    return atoi(number);        //Returns the converted-to-int string containing the message number of the last
                                //message in the file
}

/*
 * Name         : writeMsg()
 * Description  : Appends a message indicated by msg to the file stream fp. All write operations when using "a+" as
 *                file open type will append and don't need to be seeked to
 * Returns      : 0 if ok, 1 if error
 */

int writeMsg(FILE *fp, char *msg){
    //Checks if the message is too big
    if (checkIfMessageTooBig(msg)){
        printf("\nError: Couldn't write message");
        return 1;
    }
    int msgnum = findLastMessage(fp);  //Finds the last message number in the file
    fprintf(fp, "<message n=%d>\n", ++msgnum);  //Writes the header
    fprintf(fp, "%s\n", msg);                   //Writes the body
    fprintf(fp, "</message>\n");                //Writes the footer
    fflush(fp);         //Flushes the output buffer for the file pointer
    return 0;
}

/*
 * Name         : readMsg()
 * Description  : Seeks and reads a message indicated by msgnum
 * Returns      : A pointer to the string containing the message
 *                NULL if no such message #
 */

char* readMsg(FILE* fp, int msgnum, char *writeme){
    fseek(fp, 0, SEEK_SET);     //Sets the file pointer to the beginning of the file
    int status;         //Holds each upcast-to-int character read from the file, an EOF or an '<' triggers an event
    char string[11] = {'m', 'e', 's', 's', 'a', 'g', 'e', ' ', 'n', '=', '\0'}; //Comparison string to check for
    char check[11];     //The sequence of characters grabbed from file that should hold the <message> tag for comparison
    char number[10] = {'0', '\0'};    //Holds the number read from the messages, starts at 0
    do{
        status = fgetc(fp);     //Grab a character
        //If there is a less than character, check for <message n=[number]> tag
        if((char)status == '<'){
            int checkpos = 0;   //Holds the position in the checking string (characters grabbed from file)
            check[checkpos++] = (char)fgetc(fp); //Get next character
            //If the next character is not a '/' as in </message>, then continue checking
            if(check[0] != '/'){
                //Grab the next 9 characters
                for(; checkpos <= 9; checkpos++){
                    check[checkpos] = (char)fgetc(fp); //Get next character
                }
                check[10] = '\0';       //Set the null character at the end of the check string
                //If the string matches an opening <message> tag, then read the number, else continue looking for 
                //next message number
                if (!strcmp(check, string)){
                    int i = 0;
                    char chold = fgetc(fp);     //Grab next char
                    while(chold != '>'){
                        number[i++] = chold;    //Put char into number[]
                        chold = fgetc(fp);      //Grab next char
                    }
                    number[i++] = '\0';         //Adds in a null character after the number
                    //If the number just read is the message number we're looking for
                    if(atoi(number) == msgnum){
                        int offset = 0;
                        char msg[161];
                        do{
                                msg[offset++] = fgetc(fp);
                                if (offset > 10){
                                    if ( (*(msg+offset) == '>') && (*(msg+offset-1) == 'e') && (*(msg+offset-2) == 'g') && 
                                        (*(msg+offset-3) == 'a') && (*(msg+offset-4) == 's') && (*(msg+offset-5) == 's') && 
                                        (*(msg+offset-6) == 'e') && (*(msg+offset-7) == 'm') && (*(msg+offset-8) == '/') && 
                                        (*(msg+offset-9) == '>') ){
                                        break;
                                    }
                                }
                        }while(1);
                        //If offset greater than </message> size and the last 10 characters are </message> chop off the tag
                        msg[offset-9] = '\0';       //Set the first character of the section to be chopped off
                                                    //to the null char
                        strcpy(writeme, msg);
                        return writeme;                 //Return the msg that was read
                     }
                }
            }
        }
    }while(status != EOF);
    //If it reaches an EOF and finds no matching message number print error msg and return NULL
    printf("\nError: No such message #");
    return NULL;
}

/*
 * Name         : closeFile()
 * Description  : Closes the file open for reading/writing to the token ring message board
 */

void closeFile(FILE* fp){
    if (fclose(fp) == EOF){
        perror("\nFile close error :  ");
    }
    return;
}

/*
 * Name         : checkIfMessageTooBig()
 * Description  : Checks if the message is greater than the limit of 160 chars.
 * Returns      : 0 if ok, 1 if too big
 */

int checkIfMessageTooBig(char *msg){
    int pos = 0;        //Position in message string and, incidentally, character count
    while(*(msg+pos) != '\0'){
        if (pos > 160){
            printf("\nError: Message is too large.\n\tMust be <= 160 characters");
            return 1;
        }
        pos++;
    }
    return 0;
}

/*
 * Name         :
 * Description  :
 */