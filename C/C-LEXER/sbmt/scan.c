/*
Student Name: 
Student ID:

 */



/****************************************************/
/* File: scan.c                                     */
/* The scanner implementation for the TINY compiler */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/* Programming by Zhiyao Liang                      */
/* MUST FIT, 2014 Fall                              */
/****************************************************/

#include <ctype.h>  /* isdigit() isalpha() isspace() */
#include <stdio.h>
#include <stdlib.h>  /* for malloc */
#include <string.h>
#include "globals.h"
#include "util.h"
#include "scan.h"

/* states in scanner DFA */
typedef enum {
  START, READ_OVER, IN_COMMENT, WAIT_OVER, IN_NUM, IN_ID, READ_LT, READ_GT, READ_EQ, READ_NOT, UNGET_DONE, DONE, WRONG
} StateType;


/* BUF_LEN = length of the input buffer for
   source code lines */
#define BUF_LEN 256

// The line number of the current token, starting from 1, since lineNum++ appears before the first usage. 
static int lineNum = 0;

static char lineBuf[BUF_LEN]; /* holds the current line of the source program */
static int linePos = 0; /* current position in LineBuf, pointing to the first character that is not processed. */
static int bufSize = 0; /* current size of buffer string */

TokenNode* tokenList = NULL;


//static boolean EOF_flag = FALSE; 
/* corrects unget_next_char behavior on EOF */
// Better to handle EOF at the spot, instead put it as a global varaible, since it is only used in one situation.

/* 
   http://publib.boulder.ibm.com/infocenter/zvm/v6r1/index.jsp?topic=/com.ibm.zvm.v610.edclv/fgets.htm

   char *fgets(char *string, int n, FILE *stream);

   Reads bytes from a stream pointed to by stream into an array pointed to by string, starting at the position indicated by the file position indicator. Reading continues until the number of characters read is equal to n-1, or until a new-line character (\n), or until the end of the stream, whichever comes first. The fgets() function stores the result in string and adds a null character (\0) to the end of the string. The string includes the new-line character, if read.

   If EOF is reached after data has already been read into the string buffer, fgets() returns a pointer to the string buffer to indicate success. A subsequent call would return NULL since fgets() would reach EOF without reading any data.

*/


// use fgetc(FILE *) is also ok

/* - get_next_char fetches the next non-blank character
   from lineBuf.
   -  reading in a new line if every characters in lineBuf is
   processed 
   -  Returns EOF if EOF of the source file is reached, which means all
   characters of the file has been handled
*/

static int get_next_char(void){
  if (!(linePos < bufSize)){
    lineNum++;
    //    if (fgets(lineBuf,BUF_LEN-1,source))   // also ok, but prefer the explicit style
    if (fgets(lineBuf,BUF_LEN-1,source)!=NULL) {  // returns null when end of file is reached
      if(Pause) my_pause();
      if (EchoSource) fprintf(listing,"%4d: %s",lineNum,lineBuf);
      if(Pause) my_pause();
      bufSize = strlen(lineBuf);
      if(bufSize >= BUF_LEN-1)
	printf("Warning: a line of code exceeds the scanner's buffer size !!! \n");
      linePos = 0;
      return lineBuf[linePos++];
    }
    else {
      return EOF;  //EOF has value -1, so the return type is int, instead of unsigned int
    }
  }
  else return lineBuf[linePos++];
}


// unget_next_char backtracks one characterin lineBuf
// Should be aware of the EOF situation when it is called.  
// linePos cannot be negative.
static void unget_next_char(void) {
  //if (!EOF_flag)
  if(linePos>0) // for safty
    linePos-- ;
}


/* lookup table of reserved words */
// use char * to save a string constant will have a
//  warning: deprecated conversion from string constant to ‘char*’ 
// 11152013 added keyword "do"
// 10242014 added keyword "for"
static struct {
  const char* str;
  TokenType tok;
} reservedWords[MAX_RESERVED]
=
  {
    {"if",IF},{"else",ELSE},{"int",INT},
    {"while",WHILE},{"return",RETURN},{"void",VOID},
	{"do", DO}, {"for", FOR}
  };

/* lookup an identifier to see if it is a reserved word */
/* uses linear search */
static TokenType reserved_lookup (char * s){
  int i;
  for (i=0;i<MAX_RESERVED;i++)
    // strcmp returns 0 when two strings are equivalent
    if (!strcmp(s,reservedWords[i].str))
      return reservedWords[i].tok;
  return ID;
}

/* Procedure print_token prints a token 
 * and its lexeme to the listing file
 */
//void print_token( TokenType token, const char* tokenString )
// 20131115 added keyword "do"
// 20141024 added keyword "for"
// There is no newline printed. The caller of this function is responsible to control newline or not. 
void print_token( const Token * tk) {
  switch (tk->type) {
  case IF:
  case ELSE:
  case WHILE:
  case INT:
  case VOID:
  case RETURN:
    fprintf(listing, "Keyword: %s",tk->string);
    break;
  case ASSIGN: fprintf(listing,"="); break;
  case EQ: fprintf(listing,"=="); break;
  case NEQ: fprintf(listing, "!="); break;
  case LT: fprintf(listing,"<"); break;
  case LTE: fprintf(listing,"<="); break;
  case GT: fprintf(listing,">"); break;
  case GTE: fprintf(listing,">="); break;
  case LPAREN: fprintf(listing,"("); break;
  case RPAREN: fprintf(listing,")"); break;
  case LBR: fprintf(listing,"["); break;
  case RBR: fprintf(listing,"]"); break;
  case LCUR: fprintf(listing,"{"); break;
  case RCUR: fprintf(listing,"}"); break;
  case SEMI: fprintf(listing,";"); break;
  case COMMA: fprintf(listing,","); break;
  case PLUS: fprintf(listing,"+"); break;
  case MINUS: fprintf(listing,"-"); break;
  case TIMES: fprintf(listing,"*"); break;
  case OVER: fprintf(listing,"/"); break;
  case EOFILE: fprintf(listing,"EOF!"); break;
  case ERROR: fprintf(listing,"ERROR!!!"); break;
  case NUM:
    fprintf(listing, "NUM, val= %s",tk->string);
    break;
  case ID:
    fprintf(listing, "ID, name= %s",tk->string);
    break;
    /*case ERROR:
      fprintf(listing,
      "ERROR: %s",tk->string);
      break; */
  default: /* should never happen */
    fprintf(listing,"Unknown token: %s",tk->string);
    break;
  }
}



/****************************************/
/* the primary function of the scanner  */
/****************************************/
/* function getToken returns the 
 * next token in source file
 This function is implemented very mechanically according to the DFA graph shown in class. 
 The basic computation is sketched below :

- There is a big loop for repeated call of get_next_char();
- Before the loop, 
   .  create a variable of current state, with value START.
   .  Create a character array with the length enough to save a longest token (use MAX_TOKEN_LEN). This array  will be used to save a token string.  
   .  Create a variable for token type. 
- Loop continues until the current state is DONE, WRONG, or UNGET_DONE. Inside the loop, do the following
   . get a character by calling get_next_char(). 
   . For each input character,  do the following:
      1. Decide the next current state, update the current state variable.  
      2. Decide whether to save the character in to the char array of token string. 
need to unget the input character? (only at wong, unget_done of done states need to consider it)
      3. Decide whether to save the character into token string? 
      4. IF the next state is START or DONE or UNGET_DONE, which means ia token is obtained, decided what is the type of the token? 
- When a token is obtained, make sure the following is satisfied
  . If the STATE is UNGET_DONE, need to put the character back to the input stream, by calling the function unget_next_char(). 
  . If the state is WRONG, the flag errorFound is TRUE. 
  . If the token type is ERROR or EOFILE, its token string can be anything, better to be an empty string. 
  . If the TraceScan flag is TRUE, print out the token and the line number in a line. 
*/
Token * get_token(void) {
  /**
   Provide your code here !!!
   */ 
  Token *token = NULL;
  TokenType type;
  StateType state = START;

  char symbol[MAX_TOKEN_LEN+1];
  int sym_i = 0;
  char nextchar;
  Boolean save;

  while (state != DONE && state != WRONG
		  && state != UNGET_DONE) {
	nextchar = get_next_char(); 
	save = TRUE;
	/* 1. decide next state */
	switch (state) { 

	 case START:
	  if (isdigit(nextchar)) state = IN_NUM;
	  else if (isalpha(nextchar)) state = IN_ID;
	  else if (nextchar == '=') state = READ_EQ;
	  else if (nextchar == '!') state = READ_NOT;
	  else if (nextchar == '>') state = READ_GT;
	  else if (nextchar == '<') state = READ_LT;
	  else if (nextchar == '/') state = READ_OVER;
	  else if (nextchar == ' ' || nextchar == '\t'
				|| nextchar == '\r' || nextchar == '\n')
		  save = FALSE;
	  else {
			state = DONE; /* single char state */
			switch (nextchar) {
				case EOF:
					save = TRUE;
					type = EOFILE;
					break;
				case '+':
					type = PLUS;
					break;
				case '-':
					type = MINUS;
					break;
				case '*':
					type = TIMES;
					break;
				case ';':
					type = SEMI;
					break;
				case ',':
					type = COMMA;
					break;
				case '(':
					type = LPAREN;
					break;
				case ')':
					type = RPAREN;
					break;
				case '[':
					type = LBR;
					break;
				case ']':
					type = RBR;
					break;
				case '{':
					type = LCUR;
					break;
				case '}':
					type = RCUR;
					break;
				default:
					type = ERROR;
					break;
			}
		}
		break;
	 case IN_NUM:
		if (!isdigit(nextchar)) {
			unget_next_char();
			save = FALSE;
			state = DONE;
			type = NUM;
		}
		break;
	 case IN_ID: /* only letter id */
		if (!isalpha(nextchar)) {
			unget_next_char();
			save = FALSE;
			state = DONE;
			type = ID;
		}
		break;
	 case READ_LT:
		state = DONE;
		if (nextchar == '=') {
			type = LTE;
			save = TRUE;
		}
		else {
			unget_next_char();
			type = LT;
			save = FALSE;
		}
		break;
	 case READ_GT:
		state = DONE;
		if (nextchar == '=') {
			type = GTE;
			save = TRUE;
		}
		else {
			unget_next_char();
			type = GT;
			save = FALSE;
		}
		break;
	 case READ_EQ:
		state = DONE;
		if (nextchar == '=') {
			type = EQ;
			save = TRUE;
		}
		else {
			unget_next_char();
			type = ASSIGN;
			save = FALSE;
		}
		break;
	 case READ_NOT:
		state = DONE;
		if (nextchar == '=') {
			type = NEQ;
			save = TRUE;
		}
		else {
			unget_next_char();
			save = FALSE;
			type = ERROR;
		}
		break;
	 case READ_OVER:
		if (nextchar == '*') {
			state = IN_COMMENT;
			save = FALSE;
			--sym_i; /* unput '/' */
		} else { /* div */
			unget_next_char();
			state = DONE;
			type = OVER;
			save = FALSE;
		}
		break;
	 case IN_COMMENT:
		save = FALSE;
		if (nextchar == '*') {
			state = WAIT_OVER;
		}
		break;
	 case WAIT_OVER:
		if (nextchar == '/') {
			state = START;
			save = FALSE;
		}
		else {
			state = IN_COMMENT;
			save = FALSE;
		}
		break;
	 default:
		state = DONE;
		type = ERROR;
		break;
	}

	if (save == TRUE && (sym_i <= MAX_TOKEN_LEN))
		symbol[sym_i++] = nextchar;

	if (state == DONE) {
		symbol[sym_i] = '\0';
		if (type == ID)
			type = reserved_lookup(symbol);
	}
 } // end while

 if (TraceScan) {
	 printf("Line Number:%d, TokenString:%s, TokenType:%d\n",
			 lineNum, symbol, type);
 }

 token = (Token *) malloc(sizeof(Token));
 token->string = (char *) malloc(sizeof(strlen(symbol)) + 1);
 strcpy(token->string, symbol);
 token->type = type;

 return token;

}// end of get_token()


/* The scan() function will repeatedly call the get_token() function until an EOFILE token is obtained. Whenever a token is returned by the get_token() function, including the last EOFILE token, it is inserted in to a new token node, together with the current line number. This token node is attached to the end of a growing double-linked list. The head of this list is returned by the scan() function. 

Note that in main() function, the return of scan() is assgned to the tokenList variable. 
 */
TokenNode* scan(void){
  /**
   Provide your code here !!!
   */ 
  TokenNode *head = NULL, *current = NULL;
  Token *token = NULL;

  while ((token = get_token()) != NULL) {
	  /* prepare TokenNode */
	  TokenNode *p = (TokenNode *)malloc(sizeof(TokenNode));
	  if (p == NULL) {
		  printf("Waring: malloc node error.\n");
		  break;
	  }

	  p->token = token;
	  p->lineNum = lineNum;
	  p->prev = p->next = NULL;

	  /* insert TokenNode into TokenList */
	  if (head == NULL) {
		  head = p;
		  current = p;
	  }
	  else {
		  p->prev = current;
		  current->next = p;

		  current = p; /* point to tail */
	  }
		  
	  if (token->type == EOFILE) {
		  break;
	  }
  }

  return head;
}


static void print_token_list(const TokenNode * lis){
  const TokenNode * nd = lis;
  while(nd!=NULL){
    fprintf(listing, "%d: ", nd->lineNum);
    print_token(nd->token);
    fprintf(listing, "\n"); // new line
    nd = nd->next;
  }
}

void print_the_token_list(void){
  print_token_list(tokenList);
}
