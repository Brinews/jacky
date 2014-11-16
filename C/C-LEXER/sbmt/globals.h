/****************************************************/
/* File: globals.h                                  */
/* Global types and vars for C-Minus compiler       */
/* must come before other include files             */
/* Compiler Construction: Principles and Practice   */
/* Book author:  Kenneth C. Louden                  */
/* Programming by Zhiyao Liang                      */
/* MUST FIT 2014 Fall                               */
/****************************************************/

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

/*  Instead of puting these library head files here, let these file be included in each file where they are needed */

#include <stdio.h>
/*
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
*/

// include util.c so that the boolean type can be used
#include "util.h"

/* source and listing are defined in main.c */
extern FILE* source; /* source code text file */
extern FILE* listing; /* listing output text file */


/**************************************************/
/***********   Flags for tracing       ************/
/**************************************************/

/* EchoSource = TRUE causes the source program to
 * be echoed to the listing file with line numbers
 * during parsing
 */
extern const Boolean EchoSource;

/* TraceScan = TRUE causes token information to be
 * printed to the listing file as each token is
 * recognized by the scanner
 */
extern const Boolean TraceScan;

extern Boolean errorFound; 

extern const Boolean Pause;

extern const Boolean ExitOnError;

#endif
