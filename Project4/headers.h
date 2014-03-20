/* 
 * File:   headers.h
 * Author: Michael Hood, Patrick Brown
 *
 * Created on January 26, 2014, 2:10 PM
 */

#ifndef HEADERS_H
#define	HEADERS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/wait.h>

int checkServerName(char *);
void copyString(char *, char *, int *);
void checkResponse(int, int, const char *, char *, const char *, const char *);


#ifdef	__cplusplus
}
#endif

#endif	/* HEADERS_H */

