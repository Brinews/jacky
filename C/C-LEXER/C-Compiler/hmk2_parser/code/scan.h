/****************************************************/
/* File: scan.h                                     */
/* The scanner interface for the C-Minus compiler   */
/* Compiler Construction: Principles and Practice   */
/* Book author:  Kenneth C. Louden                  */
/* Programming by Zhiyao Liang                      */
/* MUST FIT 2014 Fall                               */
/****************************************************/

#ifndef _SCAN_H_
#define _SCAN_H_

/* MAXRESERVED = the number of reserved words/ keywords */
// Change it to 6
// else if int return void while
// 11152013, add "do" in it to the keywords,  change 6 to 7
// 10242014, add "for" in into the keywords, change 7 to 8
#define MAX_RESERVED 8

/* MAXTOKENLEN is the maximum size of a token */
#define MAX_TOKEN_LEN 40

/* tokenString array stores the lexeme of each token */
//extern char tokenString[MAX_TOKEN_LEN+1];

/* the line number of the current token */
//extern int lineNum; /* source line number for listing */

typedef enum 
    /* book-keeping tokens */
   {EOFILE,ERROR,
    /* reserved words 
     if else int return void while 
    */
    IF,ELSE,INT,RETURN,VOID,WHILE,DO,FOR,
    /* multicharacter tokens */
    ID,NUM,
    /* special symbols for 
       + - * / < <= > >= == != = ; , ( ) [ ] { } 
    */
    PLUS,MINUS,TIMES,OVER,LT,LTE, GT, GTE, EQ, NEQ, ASSIGN, SEMI, COMMA, LPAREN,RPAREN, LBR, RBR, LCUR, RCUR   
   } TokenType;

typedef struct{
  const char * string;   
  // It is ok to assign  the return of copy_string(), which is char *, to string, 
  TokenType type;
} Token;

// A node in a double linked list
typedef struct TkNd{
  Token* token;
  int lineNum;
  struct TkNd * prev;
  struct TkNd * next;
}TokenNode;

/* tokenList is defined in scan.c */
extern TokenNode* tokenList;

/* function getToken returns the 
 * next token in source file
 */
Token* get_token(void);

/* Procedure print_token prints a token 
 * and its lexeme to the listing file
 */
//void printToken( TokenType, const char* );

void print_token(const Token * );

void print_the_token_list(void);

TokenNode * scan(void);


#endif
