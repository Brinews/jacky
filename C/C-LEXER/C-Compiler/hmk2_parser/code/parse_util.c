/****************************************************
 File: parse_util.c                               
 The utility tools of the parser 
 for the C-Minus compiler   
 Textbook: Compiler Construction: Principles and Practice   
 Book author:  Kenneth C. Louden                  
 Programming by Zhiyao Liang                      
 MUST FIT 2014 Fall
****************************************************/

#include "libs.h"
#include "util.h"
#include "scan.h"
#include "parse.h"
#include "parse_util.h"
#include "parse_print.h"
#include "globals.h"


/* Function new_stmt_node creates a new statement
   node for syntax tree construction.
   Will not return NULL.
*/
TreeNode * new_stmt_node(StmtKind k, int lineNum)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL) {
    fprintf(listing,"ERROR:  Out of memory at line %d\n",lineNum);
    exit(0);
  }
  else {
    for (i=0;i<MAX_CHILDREN;i++) t->child[i] = NULL;
    t->lSibling = NULL;
    t->rSibling = NULL;
    t->parent = NULL;
    t->lineNum = lineNum;  
    t->nodeKind = STMT_ND;
    t->kind.stmt = k;
    // For stmt_node, the attr field is useless. 
  }
  return t;
}

/* Function new_expr_node creates a new expression 
 * node for syntax tree construction
 * Some fields need be set by the calling code.
 */
TreeNode * new_expr_node(ExprKind k, int lineNum)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL){
    fprintf(listing,"ERROR: Out of memory error at line %d\n",lineNum);
    exit(0);
  }
  else {
    for (i=0;i<MAX_CHILDREN;i++) t->child[i] = NULL;
    t->parent = t->lSibling = t->rSibling = NULL;
    t->nodeKind = EXPR_ND;
    t->kind.expr = k;
    t->lineNum = lineNum;
    t->type = VOID_TYPE; 
    // setting the default attributes of the expression.
    // These attributes are useless and be updated with meaningful data during parsing.
    t->attr.exprAttr.op = ERROR;  
    t->attr.exprAttr.val = 0;
    t->attr.exprAttr.name = "";
  }
  return t;
}


/* Function newDclNode creates a new declaration 
 * node for syntax tree construction
 * Some fields need be set by the calling code.
 */
TreeNode * new_dcl_node(DclKind k, int lineNum)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL){
    fprintf(listing,"ERROR: Out of memory error at line %d\n",lineNum);
    exit(0);
  }
  else {
    for (i=0;i<MAX_CHILDREN;i++) t->child[i] = NULL;
    t->parent = t->lSibling = t->rSibling = NULL;
    t->nodeKind = DCL_ND;
    t->kind.dcl = k;
    t->lineNum = lineNum;
    // Setting the default attributes of the declaration
    // these attributes will be updated according to the context of parsing.
    t->attr.dclAttr.type = VOID_TYPE; 
    t->attr.dclAttr.name = "";
    t->attr.dclAttr.size = 0;
  }
  return t;
}


/* Function newDclNode creates a new parameter 
   node for syntax tree construction
   Some fields need be set by the calling code.
   Will not return NULL.
*/
TreeNode * new_param_node(ParamKind k, int lineNum)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL){
    fprintf(listing,"ERROR: Out of memory error at line %d\n",lineNum);
    exit(0);
  }
  else {
    for (i=0;i<MAX_CHILDREN;i++) t->child[i] = NULL;
    t->parent = t->lSibling = t->rSibling = NULL;
    t->nodeKind = PARAM_ND;
    t->kind.param = k;
    t->lineNum = lineNum; // Invalid number line number, will be updated lated.
    // Setting the default attributes of the declaration
    // these attributes will be updated according to the context of parsing.
    t->attr.dclAttr.type = VOID_TYPE; 
    t->attr.dclAttr.name = "";
    t->attr.dclAttr.size = 0;
  }
  return t;
}

// Called when The parser found that something wrong with the current token. 
// Note that \n at the end of the message that is printed out

void syntax_error(const TokenNode * tknd, const char * message) {
  fprintf(listing," !!! Error !!!  >>> ");
  fprintf(listing, "%s\n", message);
  if(tknd == NULL)
    fprintf(listing, "Found a NULL token node.\n");
  else{
    int lineNum ;
    /* One problem is that when last token in a line is missing, in the error message
     * The next line number is printed. That is not preferred, so, one choice is tknd->prev->lineNum.
     */
    //lineNum = (tknd->prev==NULL) ? tknd->lineNum : tknd->prev->lineNum;
    lineNum = tknd->lineNum;
    fprintf(listing,"Syntax error at the token %s, in line %d \n  ",
	    tknd->token->string,
	    lineNum);
    }
  errorFound = TRUE;
  if(ExitOnError)
    exit(0);
}


/* match()
   parameters:  
   - tknd    the current token node during parsing
   - expected  The token type that the current token should match.
   -  msg   The error message that should be printed out when something is wrong. 

   behavior: 
   -  Does not move the curren token node.  
   -  When the current token match with the expected token type.
   -  Returns TRUE if the token type matches, otherwise return FALSE.
   -  If the token type does not match, error messages are printed out, a
  
*/

Boolean match(const TokenNode *tknd, TokenType expected,  const char* errorMsg)
{ 
  if( (tknd !=NULL) && (expected == tknd->token->type))
    return TRUE;
  // next_token_node();
  else{
    fprintf(listing, "\n Expecting a token: ");
    print_token_type(expected);
    fprintf(listing, "  But error occurs:\n"); 
    syntax_error(tknd, errorMsg);
    return FALSE;
  }
}

// match the current token node (thisTokenNode )  with the expected type
// If the match is successful, move the current token node to its next neighbor
Boolean match_move(TokenType expected, const char * errorMsg){
  if(match(thisTokenNode, expected, errorMsg)) 
    { next_token_node(); 
      return TRUE;
    } 
  else
    return FALSE;
}

// move the current token node to its next one. 
// If the current token node is NULL, it is an error. 
void next_token_node(void){
  if(thisTokenNode == NULL){
    fprintf(listing, "next_token_node():  meet a NULL token node\n");
    errorFound = TRUE;
  }
  else
    thisTokenNode = thisTokenNode -> next;
} 

/* checking  return a node, call it theOne, which is decided by nd and steps.
 if steps is 0,  theOne is nd
 if steps is 1,  theOne is nd->next
 if steps is -1, theOne is nd->prev
 if steps is 2,  theOne is nd->next->next
 if steps is -2, theOne is nd->prev->prev
 returns FALSE when not matching, but without reporting error. This function carefully avoid the case when
 -> operator is applied to a NULL tokenNode.
 Note that no error is reported. when NULl is met
*/
TokenNode * reach_node(const TokenNode * nd, int steps){
	int j = 0;
	const TokenNode * theOne = nd;
	do{
		if(theOne == NULL){
			/*
			fprintf(listing, "reach_node(), NULL token node met\n");
			errorFound = TRUE;
			*/
			break;
		}
		if(j == steps)
			break;
		(steps>0)? (theOne = theOne->next, j++) : (theOne = theOne->prev, j--);
	}while(1);
	return  (TokenNode *) theOne;
}

/* check if the type of nd is tp. No error is reported when nd is NULL */
Boolean check(const TokenNode * nd, TokenType tp){
	if ((nd!=NULL) && nd->token->type == tp)
  		return TRUE;
  	else
  		return FALSE;


}
// make sure that every node is connected to its parent. 
// parent should be the parent node of nd. 
void connect_parent(TreeNode * parent, TreeNode * nd){
  int j = 0;
  if(nd == NULL)
    return;
  nd->parent = parent;
  while(j<MAX_CHILDREN){
    connect_parent(nd, nd->child[j]);
    j = j + 1;
  }
  connect_parent(parent, nd->rSibling);
}


// find_closing_mark()
// Giving a starting mark of [, (, or {, find the token node of the paring closing mark of ], ) or }
// parameter:  from should be the starting place of searching, and from should be a left mark, i.e, [ { or (. 
// The token type of from should be LPAREN, LBR, or LCUR; otherwise, NULL is returned. 

TokenNode* find_closing_mark(const TokenNode* from) {
  TokenType openType = from->token->type;
  TokenType closeType ;
  TokenNode* current;
  int j = 1; // count the number of opening marks that have not found its pair. 
  if (from == NULL) return NULL;
  switch(openType){
  case LPAREN: closeType = RPAREN; break;
  case LCUR: closeType = RCUR; break;
  case LBR: closeType = RBR; break;
  default: 
    fprintf(listing," The argument of find_closing_mark() is not a token node of  [ ( or{ \n" );
    return NULL;
  }
  current = from -> next;
  while(current != NULL){
	  if(check(current, closeType)) 
		  if(j == 1) 
			  return current;
		  else 
			  j--;
	  else if(check(current,  openType))
		  j++;
	  current = current->next;
  }
  return NULL; 
}

// free the space allocated to the tree whose top is nd.
void free_tree(TreeNode * nd){
  int j;
  if (nd==NULL)
    return ;
  // free the children
  for(j=0 ; j<MAX_CHILDREN; j++)
    free_tree(nd->child[j]);
  // free the siblings (free a list)
  free_tree(nd->rSibling);
  // free this token node
  free(nd);
  return;
}

TreeNode * parse_bad_return(TreeNode * top, Boolean * ok){
	*ok = FALSE;
	if(VerboseParser == TRUE){ /* keep the tree */
		return top;
	}

	else{
		free_tree(top);  /*release the memory of the tree */
		return NULL;
	}
}


TreeNode * parse_good_return(TreeNode * top, Boolean * ok){
	*ok = TRUE;
	return top;
}

