/****************************************************/
/* File: main.c                                     */
/* Main program for C-Minus compiler                */
/* Compiler Construction: Principles and Practice   */
/* Book author:  Kenneth C. Louden                  */      
/* Programming by Zhiyao Liang                      */
/* MUST FIT 2014 Fall                               */
/****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "globals.h"
#include "scan.h"

/* allocate global variables */

FILE * source;
FILE * listing;

/* allocate and set tracing flags */
const Boolean EchoSource = TRUE; // to print the source code when reading it. 
const Boolean TraceScan= TRUE;  // to print each token when it is obtained by the scanner.
//int TraceParse = FALSE;
//int TraceAnalyze = FALSE;
//int TraceCode = FALSE;


const Boolean Pause = FALSE;

Boolean errorFound = FALSE; // Whether  some error is found

const Boolean ExitOnError = TRUE; // Wether to  exit the program when the first error is found

const Boolean TestTokenList = TRUE; // Wether to test (print) the token list

int main( int argc, char * argv[] )
{ 
  char pgm[120]; /* C- program code file name */
  if (argc != 2)
    { fprintf(stderr,"usage: %s <filename>\n",argv[0]);
      exit(1);
    }
  strcpy(pgm,argv[1]) ;
  if (strchr (pgm, '.') == NULL)
    strcat(pgm,".cm");  // the C-Minus program has a .cm extension
  source = fopen(pgm,"r");
  if (source==NULL)
  { fprintf(stderr,"File %s not found\n",pgm);
    exit(1);
  }
  listing = stdout; /* send listing to screen */
  fprintf(listing,"\nC-Minus COMPILATION: %s\n",pgm);
  tokenList = scan();
  if(TestTokenList){
    fprintf(listing, "\nThis is the token list obtained by the scanner:\n");
    print_the_token_list();
  }
  fclose(source);
  return 0;
}

