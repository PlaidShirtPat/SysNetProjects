/* 
 * File         : fileio.h
 * Author(s)    : Michael Hood, Patrick Brown
 * Project name : Systems and Networks II - Dr. Reichherzer - Spring 2014 - Project 3
 * Description  : This provides prototypes for the file IO functions for the token ring message board.
 *
 * Created on March 2, 2014, 5:53 PM
 */

#ifndef FILEIO_H
#define	FILEIO_H

#ifdef	__cplusplus
extern "C" {
#endif

///
///Includes
///
    
#include <stdio.h>      //For the FILE* datatype
    
///
///Prototypes
///
    
FILE* createFile();             //Opens and/or creates the message board shared file
int findLastMessage(FILE*);     //Finds the last message # in the file
int writeMsg(FILE*, char*);     //Writes a message to the message board file
char* readMsg(FILE*, int, char*);      //Reads a message from the message board file
void closeFile(FILE*);          //Closes the message board file
int checkIfMessageTooBig(char*);//Checks if the message is too big (>160 chars)

#ifdef	__cplusplus
}
#endif

#endif	/* FILEIO_H */