/* 
 * File         : main.c
 * Author       : Michael Hood, Patrick Brown
 * Project name : Systems and Networks II - Dr. Reichherzer - Spring 2014 - Project 3
 * Description  : This function is only for testing the fileio.c file's functions.
 *
 * Created on March 2, 2014, 5:52 PM
 */

///
///Includes
///

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileio.h"

/*
 * Name         : main()
 * Description  : Testing main for fileio functions
 */
int main(int argc, char** argv) {
    FILE* fp = createFile();
    printf("\nNum of msgs in file : %d, should be 0", findLastMessage(fp));
    printf("\nLength of string to be written  = %d", strlen("Hello"));
    if(writeMsg(fp, "Hello")){
        printf("\nwriteMsg() error, tried writing simple msg");
    }
    printf("\nNum of msgs in file : %d, should be 1", findLastMessage(fp));
    printf("\nLength of string to be written  = %d", strlen("12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"));
    if(writeMsg(fp, "12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890")){
        printf("\nwriteMsg() error, tried big msg (>160)");
    }
    printf("\nNum of msgs in file : %d, should be 1", findLastMessage(fp));
    printf("\nLength of string to be written  = %d", strlen("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"));
    if(writeMsg(fp, "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890")){
        printf("\nwriteMsg() error, tried msg of 160 chars");
    }
    printf("\nNum of msgs in file : %d, should be 2", findLastMessage(fp));
    if(writeMsg(fp, "<me>")){
        printf("\nwriteMsg() error, tried writing msg with BNF tag form that wasn't <message n =[#]> or </message>");
    }
    printf("\nNum of msgs in file : %d, should be 3", findLastMessage(fp));
    if(writeMsg(fp, "</message>")){
        printf("\nwriteMsg() error, tried writing </message> in body");
    }
    printf("\nNum of msgs in file : %d, should be 3", findLastMessage(fp));
    if(writeMsg(fp, "<message n=1>")){
        printf("\nwriteMsg() error, tried writing <message n=1> in body");
    }
    printf("\nNum of msgs in file : %d, should be 3", findLastMessage(fp));
    if(writeMsg(fp, "<message n=1")){
        printf("\nwriteMsg() error, tried writing <message n=1 in body, should work");
    }
    printf("\nNum of msgs in file : %d, should be 3", findLastMessage(fp));
    char retrieved[161];
    printf("\nMsg #1: \n%s", readMsg(fp, 1, retrieved));
    printf("\nMsg #2: \n%s", readMsg(fp, 2, retrieved));
    printf("\nMsg #3: \n%s", readMsg(fp, 3, retrieved));
    printf("\nMsg #4: \n%s", readMsg(fp, 4, retrieved));
    return (EXIT_SUCCESS);
}