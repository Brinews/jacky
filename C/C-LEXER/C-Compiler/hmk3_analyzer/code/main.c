/****************************************************/
/* File: main.c                                     */
/* Main program for C-Minus compiler                */
/* Compiler Construction: Principles and Practice   */
/* Book author:  Kenneth C. Louden                  */      
/* Programming by Zhiyao Liang                      */
/* MUST FIT 2014 Fall                               */
/****************************************************/

#include "libs.h"
#include "util.h"
#include "globals.h"
#include "scan.h"
#include "parse.h"
#include "parse_print.h"
#include "analyze.h"
#include "symbtab.h"

/* allocate global variables */

FILE * source;
FILE * listing;

/* allocate and set tracing flags. All flags are saved in main.c to control the behavior of the compiler*/
const Boolean EchoSource = TRUE; /* whether to print a line of source code when the scanner reads it */
const Boolean TraceScan = TRUE;  /* whether to print each token when it is obtained by the scanner*/
const Boolean TraceParse = TRUE;  /* whether to print the parse tree after parsing */
const Boolean TraceAnalyze = TRUE;  /* to test the analyzer */

/* True means something wrong found? It should be set with FALSE before each.
   I.e, when parser begins, erroFound = FALSE, so that if it turns to TRUE, it means error is found by parser.
   Same to other phrases.
 */
Boolean errorFound = FALSE;

const Boolean SCAN_ONLY = FALSE; /* quit after scaning */
const Boolean PARSE_ONLY = FALSE; /* quit after parsing */
const Boolean ANALYZE_ONLY = TRUE;
const Boolean ExitOnError = FALSE;  /* quit the whole program on the first error */
const Boolean TestTokenList = FALSE; /* Whether to print the token list */
Boolean const Pause = FALSE;		/* Can be used do tasks step by step, like scanning the source code line by line */

/*
 - If it is TRUE, when parser function found an error, it returns a tree that has be built so far.
 - If it is FALSE, the parser function returns NULL, and release the memory of the tree that it has built so far.
 */
Boolean const VerboseParser = FALSE;

int main( int argc, char * argv[] )
{ 
	char pgm[120]; /* C- program code file name */
	if (argc != 2){
		fprintf(stderr,"usage: %s <filename>\n",argv[0]);
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

	if(errorFound){
		fprintf(listing, "\n\n!!!Some error with the scanning \n\n"
				"It is recommended to fix the scanning errors before doing parsing\n" );
		my_pause();
	}
	else{
		fprintf(listing, "\n\n Scanning is sucessful   \n\n");
	}


#if SCAN_ONLY
	goto END;
#endif

	errorFound = FALSE; // Reset it to record the error of parsing

	parse();

	if(TraceParse){
		fprintf(listing, "\n\n The syntax tree: \n\n");
		print_tree(syntaxTree);
	}

	if(errorFound){
		fprintf(listing, "\n\n!!!Some error with the parsing!!! \n\n"
				"It is recommended to fix the parsing errors before other compiling tasks.\n" );
		my_pause();
		goto END; /* no point to continue */
	}
	else{
		fprintf(listing, "\n\n Parsing is successful   \n\n");
	}


#if PARSE_ONLY
	goto END;
#endif  

	/* now do static semantics analysis */
	  errorFound = FALSE;
	  fprintf(listing,"\nBuilding Symbol Table...\n\n");

	  build_symb_tab(syntaxTree);

	  if(errorFound==TRUE){
		  fprintf(listing, "\n\n!!!Some error in building the symbol table!!! \n\n"
				  "It is recommended to fix these errors before other compiling tasks.\n"
				  "Now the compiler quits.\n");
		  my_pause();
		  goto END;
	  }
	  else{
		  fprintf(listing, "\n\nBuilding the symbol table  is successful   \n\n");
		  if (TraceAnalyze){
			  fprintf(listing,"\nSymbol table:\n\n");
			  st_print(listing, symbolTable);
		  }
	  }

	 errorFound = FALSE;
	 fprintf(listing,"\nChecking Types...\n");

	 type_check(syntaxTree);

	 if(errorFound){
	    fprintf(listing, "\n\n!!!Some error is found during type checking!!! \n\n"
	            "It is recommended to fix these errors before other compiling tasks.\n"
	            );
	    my_pause();
	    goto END;
	  }
	  else{
	    fprintf(listing, "\n\nType-checking is successful.   \n\n");
	  }


	END:
	fclose(source);
	printf("Bye\n");
	return 0;
}

