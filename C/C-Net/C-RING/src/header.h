#ifndef _HEADER_H
#define _HEADER_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>

#define ReportError(msg)       {perror(msg); exit(-1);}

long random(); //, M= 2147483647;
void srandom(unsigned);
void IntroduceError(char*, double);
char *GenerateData(int);
int Server(), Client();
double floor(double);
char linebuffer[100];

#endif
