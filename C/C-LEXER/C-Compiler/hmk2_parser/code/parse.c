/*
student name: 
student ID:
date: 2014-12-20
*/



/****************************************************
 File: parse.c                               
 The parser for the C-Minus compiler   
 Textbook: Compiler Construction: Principles and Practice   
 Book author:  Kenneth C. Louden                  
 Programming by Zhiyao Liang                      
 MUST FIT 2014 Fall
 ****************************************************/

#include "libs.h"
#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"
#include "parse_util.h"


//The syntaxTree that will be built by the parser
TreeNode * syntaxTree;

/* holds the current token during parsing */
TokenNode* thisTokenNode; 

/* The syntax grammar of C-
   -  Stings in the grammar are separated by at least one space. Strings include variables and constants.
   -  Variables are the words with all lower-case letters.
   -  Each constants, also called a terminal, is marked with a single-quote, such as 
   'int   'while   '<=   '; 
   Each constant is a token provided by the scanner. Each different constant in this grammar has a different token type, defined by the scanner. 
   'ID and 'NUM are constants, they  mean an id token and a number token, respectively. 
   -  The vertical bar | is used to separate rules. It is not a variable nor a constant.
   -  EMPTY means the empty string, which is often represented as lambda or epsilon in grammars.
   EMPTY is not a variable nor a constant. Rule 0 is used to explain its meaning. There is no token representing EMPTY.
   -  It is a context-free grammar. Each rule has the form:
   variable ===> production1 | production2 | ...
 */

 /*  This grammar has been majorly updated, quite different from the textbook. The following links are useful 

      Reference
      [1]  http://www.cs.man.ac.uk/~pjj/bnf/bnf.html
      [2]  http://stackoverflow.com/questions/13146830/no-loop-condition-in-for-and-while-loop
      [3]  http://port70.net/~nsz/c/c89/c89-draft.html
      [4]  http://web.ics.purdue.edu/~cs240/misc/operators.html
      [5]  http://stackoverflow.com/questions/6502819/comma-operator-and-comma-seperator-in-c
*/

/*
      shorter names:
      statement 	: stmt
      declaration	: dcl
      expression	: expr
      function		: fun
 
      Shorter names are used when two names are combined together.
*/

/*
      0.  EMPTY     		===> 

      1.  program    		===>  dcl-list

      2.  dcl-list  		===>  dcl-list declaration | declaration

      3.  declaration		===>  var-dcl | fun-dcl | array-dcl

      4.  type-specifier  	===>  'int | 'void

      5.  var-dcl 		===>  type-specifier 'ID ';

      6.  array-dcl		===>  type-specifier 'ID  '[ 'NUM  '] ';

      7.  fun-dcl	     	===>  type-specifier 'ID '( params ') compound-stmt

      8.  params  		===>  param-list | 'void

      9.  param-list 		===>  param-list ', param | param

      10. param  		===>  type-specifier 'ID | type-specifier 'ID '[ ']

      11. compound-stmt  	===>  '{ local-dcl-list  stmt-list '}

      12. local-dcl-list	===>  local-dcl local-dcl-list | EMPTY

      13. local-dcl		===>  var-dcl | array-dcl

      14. stmt-list       	===>  stmt-list statement | EMPTY

      15. statement 		===>  compound-stmt | selection-stmt | while-stmt | do-while-stmt | for-stmt | return-stmt | null-stmt | expr-stmt

      16. selection-stmt  	===>  'if '( expression ') statement else-part 

      17. else-part		===>  'else statement | EMPTY

      18. while-stmt 		===>  'while '( expression ') statement

      19. do-while-stmt 	===>  'do statement 'while '( expression ') ';

      20. for-stmt		===>  'for '( expr-or-empty '; expr-or-empty '; expr-or-empty ') statement

      21. expr-or-empty 	===>  expression | EMPTY

      22. return-stmt   	===>  'return expr-or-empty ';

      23. null-stmt		===>  ';

      24. expr-stmt 		===>  expression ';

      25  expression		===>  comma-expr

      26. comma-expr 		===>  comma-expr ', assignment-expr  | assignment-expr

      27. assignment-expr 	===>  equality-expr |  lhs '= assignment-expr

      28. lhs 	           	===>  ID | array-element

      29. array-element	   	===>  ID '[ expression ']

      30. equality-expr	   	===>  equality-expr eqop relational-expr | relational-expr

      31. eqop 			===>  '== | '!=

      32. relational-expr	===>  relation-expr relop additive-expr | additive-expr

      33. relop  	       	===>  '<= | '< | '> | '>= 

      34. additive-expr		===>   additive-expr addop multiplicative-expr | multiplicative-expr

      35. addop			===>  '+ | '-

      36. multiplicative-expr	===>  multiplicative-expr mulop primary-expr | primary-expr

      37. mulop  		===>  '* | '/

      38. primary-expr      	===>  '( expression ') | ID | array-element | call | 'NUM

      39. call  		===>  ID '( arg-list ')

      40. arg-list		===>  arg-list ', assignment-expr | assignment-expr | EMPTY

 */


/* function prototypes for recursive calls
 * Each variable in the grammar corresponds to a function.
 * So, N functions corresponding to the N variables in the grammar.
 * The dash in variable names are replaced by underscore in function names,
 * e.g.  variable param-list corresponds to the function param_list().
 */

/* Computation of these functions:
 * - Assuming a list of token-nodes is already provided by the scanner.
 *   The last token node has the EOFILE token. Otherwise, the scanner should have reported some error. With assumption is useful since
 *   each function do not need to check if thisTokenNode==NULL, since the EOFILE token should be reached first, which should indicate
 *   the end of parsing, success or failure. However, be careful to expressions like thisTokenNode->next->next. Make sure that
 *   NULL->field will never occur (the function reach_node() is helpful).  
 *   someTokenNode->prev usually is safe, since normally the previous node had been examined.  However, ->next should be
 *   careful for the case when it is NULL.
 * - The parser consumes the token nodes in the list, one-by-one, from the beginning to the end.
 * - The variable thisTokenNode points to the current token node that needs to be processed.
 * - Each function consumes a sequence of token nodes, and constructs a syntax tree according to the grammar.
 * - When success (no error):
 *   -  The function returns a pointer to the top TreeNode in the constructed syntax tree.
 *   -  The variable thisTokenNode moves to the first TokenNode that is not processed, immediately after the
 *      last token node that is already processed.
 *   -  For the Boolean pointer argument ok, *ok is assigned with TRUE. 
 * - When failure (error occurred): depending on the value of the global variable verboseParser, the function returns as follows:
 *   - if verboseParser is FALSE
 *   	-  The function returns NULL.
 *   	-  The space of a tree that is allocated during this function call is freed.
 *   - if verboseParser if TRUE
 *   	- The function stops computation and returns a tree that has been constructed so far.
 * - It is expected that the parser will stop upon the first error detected. One reason for this is that in this parser location of thisTokenNode 
 *   is not adjusted to a proper token node to resume parsing. 
 *   For example, when error is detected when constructing a child subtree,
 *   (upon the return of a callee function) then the process of constructing the parent tree stops (the caller function returns ).
 * - When some error occurs during a function call:
 *   -  At least one error message should be printed out.
 *   -  One error message should be printed at the first place where it is detected,
 *      so the exact line number where the error occurs will be printed.
 *   -  When a function, say fun() is called, if some error occurred during the call,
 *      some error message(s) should be printed by this call of fun().  The caller of fun() can also, optionally,
 *      print some error message, since fun() returns NULL and NULL means failure in the situation. Doing so requires
 *      the caller remembering the location of thisTokenNode, since fun() will move thisTokenNode to later tokens.
 *      So, it is possible to see a stack of error messages, upon one error.  Note that this is optional. This parser
 *      adopts a simple way, the caller often choose to not print error messages, if any error, fun() should have reported it.
 *      In other words, this parser only require that error is reported at the lowest level where some tokens are directly analyzed.
 * - The function type_specifier() does not return a tree, because the type information is only used as a field in a
 *   declaration tree node. this function check if thisTokenNode has a INT or VOID token. If so, it returns INT or VOID,
 *   and consumes thisTokenNode (move to the next token node). If not, it returns the ERROR token type.
 * - Be careful about NULL. NULL->field will produce segmentation fault. 
 * - It is not always true that a tree that is a NULL means failure. Especially, EMPTY in the grammar could
 *     represent some NULL tree, and it is ok, not an error.
 * - When some error occurs, the whole program may continue if the variable ExitOnError, which is defined in
 *     main.c, is set as FALSE. So be prepared that after error occurs, the function still need to deal with some
 *     NULL tree.
 * - This parser can be changed in the following ways, which are not done in this parser so far.
 *   - When on error occur, do not stop the parsing, continue to find more errors. One challenge of doing so is
 *     how to put the position of thisTokenNode when resuming parsing after error occurs. A possible way is to continue the parsing after error
 *     but do not require  a whole big parse tree is constructed, which is useless now; instead, try to find a token to resume parsing, and
 *     construct several smaller trees. For example, when a WHILE token is met, then parsing a while expression begin. when a IF token is met, 
 *     then parsing of a selection statement begin, and so on. The process may ignore some tokens until one token that shows a new beginning of parsing.
 */

void parse(void);

/* By the above grammar, it is easy to know if a variable can produce EMPTY or not. 
   For a variable  A, say A() is the corresponding function of A. However, sometimes a NULL tree can be normal.

   If A() returns a value result of the type TreeNode *,
   then the caller of A() can know if A() is successful or not, depending on the  result->ok
   If result->ok is FALSe, the caller does not need to print error message for this.
      The caller of A() can quickly do failure process: parse_fail_return().

   In the following, if a function marked with "can return NULL", it means in the grammar, the variable can generate EMPTY.
 */

static const char* getTokenStr(TokenNode *tn);
static const char* copyString(const char *str, int lineno);

/* 1 */   
static TreeNode * program(Boolean * ok);

/* 2 */
static TreeNode * dcl_list(Boolean * ok);

/* 3 */
static TreeNode * declaration(Boolean * ok);

/* 4 */
static TokenType  type_specifier(void);

/* 5 */
static TreeNode * var_dcl(Boolean * ok);

/* 6 */
static TreeNode * array_dcl(Boolean * ok);

/* 7 */
static TreeNode * fun_dcl(Boolean * ok);

/* 8 */
static TreeNode * params(Boolean * ok);

/* 9 */
static TreeNode * param_list(Boolean * ok);

/* 10 */
static TreeNode * param(Boolean * ok);

/* 11 */
static TreeNode * compound_stmt(Boolean * ok);

/* 12   can return NULL */
static TreeNode * local_dcl_list(Boolean * ok);

/* 13 */
static TreeNode * local_dcl(Boolean * ok);

/* 14 can return NULL */
static TreeNode * stmt_list(Boolean * ok);

/* 15 */
static TreeNode * statement(Boolean * ok);

/* 16 */ 
static TreeNode * selection_stmt(Boolean * ok);

/* 17 can return NULL */
static TreeNode * else_part(Boolean * ok);

/* 18  */
static TreeNode * while_stmt(Boolean * ok);

/* 19 */
static TreeNode * do_while_stmt(Boolean * ok);

/* 20 */
static TreeNode * for_stmt(Boolean * ok);

/* 21 Can return NULL */
static TreeNode * expr_or_empty(Boolean * ok);

/* 22 */
static TreeNode * return_stmt(Boolean * ok);

/* 23 */
static TreeNode * null_stmt(Boolean * ok);

/* 24 */
static TreeNode * expr_stmt(Boolean * ok);

/* 25 */
static TreeNode * expression(Boolean * ok);

/* 26 */
static TreeNode * comma_expr(Boolean * ok);

/* 27 */
static TreeNode * assignment_expr(Boolean * ok);

/* 28 */
static TreeNode * lhs(Boolean * ok);

/* 29 */
static TreeNode * array_element(Boolean * ok);

/* 30 */ 
static TreeNode * equality_expr(Boolean * ok);

/* 31 */
static TreeNode * eqop(Boolean * ok);

/* 32 */
static TreeNode * relational_expr(Boolean * ok);

/* 33 */
static TreeNode * relop(Boolean * ok);

/* 34 */
static TreeNode * additive_expr(Boolean * ok);

/* 35 */
static TreeNode * addop(Boolean * ok);

/* 36 */
static TreeNode * multiplicative_expr(Boolean * ok);

/* 37 */
static TreeNode * mulop(Boolean * ok);

/* 38 */
static TreeNode * primary_expr(Boolean * ok);

/* 39 */
static TreeNode * call(Boolean * ok);

/* 40  can return NULL */ 
static TreeNode * arg_list(Boolean * ok);


/****************************************
 * the primary function of the parser
 ****************************************
 * Function parse returns the newly
 * constructed syntax tree
 */
void parse(void) {
	Boolean status;
	syntaxTree = NULL;
	/* always assume one token is read in advance.
	  Starting with first token in the token list. */
	thisTokenNode = tokenList;
	if(TraceParse){
		fprintf(listing, "Function call history : \n");
		fprintf(listing, "%20s  %-10s%s\n", "function", "LineNum","token");
		fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}
	if(thisTokenNode == NULL) {
		fprintf(listing, "parse(): The token list is NULL, the scanner is wrong. \n");
		return ;
	}
	if(check(thisTokenNode, EOFILE)){
		fprintf(listing, "parse(): The token list reaches EOFILE before parsing\n");
		return ;
	}
	syntaxTree = program(&status);
	if (syntaxTree == NULL){
		fprintf(listing, "parse(): Empty syntax tree after parsing, \n");
		errorFound = TRUE;
		return ;
	}
	if(check(thisTokenNode, EOFILE)==FALSE){
		fprintf(listing, "Error, parser does not handle some remaining tokens\n");
		errorFound = TRUE;
	}
	if(status==FALSE || errorFound )
		fprintf(listing, "Error found in parsing, some error message should have been printed\n");
	//match(thisTokenNode, EOFILE, "parse(): Need to have EOF token"); // The last token should be EOFILE.
	connect_parent(NULL, syntaxTree);
	return ;
}


/*1   program  ===>  dcl-list */

TreeNode * program(Boolean * ok){
	if(TraceParse){
		fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	return  dcl_list(ok);
}

/* 2 dcl-list  ===>  dcl-list declaration | declaration   
 */
static TreeNode * dcl_list(Boolean * ok){
	TreeNode * first = NULL;
	TreeNode * last =  NULL;
	TreeNode * right = NULL;
	Boolean status;
	if(TraceParse) {
		fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}
	/* when the end of the sequence is not met, enter the loop
	  it is guaranteed that the token node list with end with EOFILE, otherwise
	  the scanner should have reported error.  
        */
	while(!check(thisTokenNode, EOFILE)){

		right = declaration(&status);

		if(last == NULL){ // assign the first node of declaration list
			first = last = right;
		}
		else if(right!=NULL){
			last ->rSibling = right;
			right ->lSibling = last;
			last = right;
		}

		if(status==FALSE)
			return parse_bad_return(first, ok);
	}

	return parse_good_return(first, ok); /* success */
}


/* 3  declaration  ===>  var-dcl | fun-dcl | array-dcl 
 */
static TreeNode * declaration(Boolean * ok){
	TreeNode *node = NULL;
	*ok = FALSE;

	if (TraceParse) {
		fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	node = var_dcl(ok);
	if (node != NULL) return node;

	node = array_dcl(ok);
	if (node != NULL) return node;

	node = fun_dcl(ok);
	if (node != NULL) return node;

	return node;
}

/* 4 type-specifier  ===>  'INT | 'VOID
   When the type of the current token is
   int , return INT
   void, return VOID
   otherwise failure, return ERROR
   move the current token to its next when succeed. 
   Report error when failure, without moving the current token.
 */
static TokenType type_specifier(void){
	if(TraceParse) {
		fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}
	if(check(thisTokenNode, INT)) {
		next_token_node();
		return INT;
	}
	if(check(thisTokenNode, VOID)) {
		next_token_node();
		return VOID;
	}
	syntax_error(thisTokenNode, "type_specifier(): need to have int or void");
	return ERROR;
}


/* 5  var-dcl  ===>  type-specifier 'ID '; 
   Possibly returns NULL if there is no variable declaration
 */
static TreeNode * var_dcl(Boolean * ok){
	TreeNode * node = NULL;
	int lineNum = thisTokenNode->lineNum;

	TokenNode *old = thisTokenNode;

	TokenNode *n  = reach_node(thisTokenNode, 1);
	TokenNode *n2 = reach_node(thisTokenNode, 2);

	if (check(thisTokenNode, VOID) || check(thisTokenNode, INT)) {
			if (NULL != n && check(n, ID)
							&& NULL != n2 && check(n2, SEMI)) {

					if(TraceParse) {
							fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
					}

					TokenType type = type_specifier(); /* already move next one token */

					thisTokenNode = reach_node(old, 3);

					node = new_dcl_node(VAR_DCL, lineNum);
					node->attr.dclAttr.type = (type == VOID ? VOID_TYPE : INT_TYPE);
					node->attr.dclAttr.name = copyString(getTokenStr(n), lineNum);

					*ok = TRUE;
					return node;
			}
	}

	thisTokenNode = old;

	*ok = FALSE;
	return NULL;
}

//6   array-dcl		===>  type-specifier 'ID  '[ 'NUM  '] ';

static TreeNode*  array_dcl(Boolean * ok){
	TreeNode * node = NULL;
	int lineNum = thisTokenNode->lineNum;

	TokenNode *old = thisTokenNode;
	TokenNode *n  = reach_node(thisTokenNode, 1);
	TokenNode *n2 = reach_node(thisTokenNode, 2);
	TokenNode *n3 = reach_node(thisTokenNode, 3);
	TokenNode *n4 = reach_node(thisTokenNode, 4);
	TokenNode *n5 = reach_node(thisTokenNode, 5);

	if (check(thisTokenNode, VOID) || check(thisTokenNode, INT)) {
		if (n != NULL && check(n, ID)
				&& n2 != NULL && check(n2,LBR)
				&& n3 != NULL && check(n3, NUM)
				&& n4 != NULL && check(n4, RBR)
				&& n5 != NULL && check(n5, SEMI)) {

			if(TraceParse) {
					fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
			}

			TokenType type = type_specifier(); /* already move next one token */

			thisTokenNode = reach_node(old, 6);

			node = new_dcl_node(ARRAY_DCL, lineNum);
			node->attr.dclAttr.type = (type == VOID ? VOID_TYPE : INT_TYPE);
			node->attr.dclAttr.size = atoi(getTokenStr(n3));
			node->attr.dclAttr.name = copyString(getTokenStr(n), lineNum);

			*ok = TRUE;

			return node;
		}
	}

	*ok = FALSE;
	return NULL;
}

/* 7.  fun-dcl  ===>  type-specifier 'ID '( params ') compound-stmt
 *
 */
static TreeNode * fun_dcl(Boolean * ok){
	TreeNode *node = new_dcl_node(FUN_DCL, thisTokenNode->lineNum);
	Boolean status;

	if(TraceParse) {
			fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	TokenType type = type_specifier(); /* already move next one token */
	TokenNode *id = thisTokenNode;

	if (type != ERROR && match_move(ID, "fun-dcl: need 'ID")) {

			node->attr.dclAttr.type = (type == VOID ? VOID_TYPE : INT_TYPE);
			node->attr.dclAttr.name = copyString(getTokenStr(id), id->lineNum);

			if (match_move(LPAREN, "fun-dcl: need LPAREN"))
					if (node->child[0] = params(&status), status)
							if (match_move(RPAREN, "fun-dcl: nedd RPAREN"))
									if (node->child[1] = compound_stmt(&status), status)
											return parse_good_return(node, ok);
	}

	/* failure */
	return parse_bad_return(node, ok);
}

/* 8
   params  ===>  param-list | 'void
   when params is void it must surrounded by ( ).
   Also observe that that in a param, after void there must be an ID.
   No need to worry about a parameter like "void x", since it will be checked by semantics analyzer.
 */
TreeNode * params(Boolean * ok){
	TokenNode *n1 = reach_node(thisTokenNode, 0);
	TokenNode *n2 = reach_node(thisTokenNode, 1);

	if(TraceParse) {
			fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	if (n1 != NULL && check(n1, VOID)
					&& n2 != NULL && check(n2, RPAREN)) {

			TreeNode *node = new_param_node(VOID_PARAM, n1->lineNum);
			node->attr.dclAttr.type = VOID_TYPE;
			node->attr.dclAttr.name = copyString(getTokenStr(n1), n1->lineNum);

			thisTokenNode = n2;

			*ok = TRUE;
			return node;
	}

	return param_list(ok);
}

/* 9    param-list  ===>  param-list ', param | param
 *
 */
static TreeNode * param_list(Boolean * ok){
	if(TraceParse) {
			fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	TreeNode *first = NULL;
	TreeNode *last  = NULL;
	TreeNode *right = NULL;
	Boolean status;
	
	while (!check(thisTokenNode, RPAREN)) {

		if (right != NULL) {
			if (!match_move(COMMA, "param_list(): need COMMA"))
					return parse_bad_return(first, ok);
		}

		right = param(&status);
		
		if (last == NULL) {
			first = last = right;
		}
		else if (right != NULL) {
			last->rSibling = right;
			right->lSibling = last;

			last = right;
		}

		if (status == FALSE)
			return parse_bad_return(first, ok);
	}

	return parse_good_return(first, ok);
}

/* 10  param  ===>  type-specifier 'ID | type-specifier 'ID '[  ']

   Returns a parameter node.
 */
TreeNode * param(Boolean * ok){
	if(TraceParse) {
			fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	int steps = 0;

	TokenNode *n1 = reach_node(thisTokenNode, 1);
	TokenNode *n2 = reach_node(thisTokenNode, 2);
	TokenNode *n3 = reach_node(thisTokenNode, 3);

	TokenType type = type_specifier(); /* already move next one token */
	TreeNode *node = NULL;

	if (type != ERROR) {
			ParamKind pkind = VAR_PARAM;
			if (type == VOID) pkind = VOID_PARAM;

			if (n1 != NULL && check(n1, ID)) {
				steps = 1;

				if (n2 != NULL && check(n2, LBR)
								&& n3 != NULL && check(n3, RBR)) {
						pkind = ARRAY_PARAM;
						steps += 2;
				}

				node = new_param_node(pkind, n1->lineNum);
				node->attr.dclAttr.type = (type == INT ? INT_TYPE : VOID_TYPE);
				node->attr.dclAttr.name = copyString(getTokenStr(n1), n1->lineNum);

				thisTokenNode = reach_node(thisTokenNode, steps);

				return parse_good_return(node, ok);
			}
	}

	return parse_bad_return(NULL, ok);
}


/*11  compound-stmt  ===>  '{ local-dcl-list stmt-list '}
 */
TreeNode * compound_stmt(Boolean * ok){
	if(TraceParse) {
			fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	Boolean status;
	TreeNode *node = new_stmt_node(CMPD_STMT, thisTokenNode->lineNum);

	if (match_move(LCUR, "compound-stmt: need LCUR"))
		if (node->child[0] = local_dcl_list(&status), status)
				if (node->child[1] = stmt_list(&status), status)
						if (match_move(RCUR, "compund-stmt: need RCUR"))
								return parse_good_return(node, ok);

	return parse_bad_return(node, ok);
}


/* 12   local-dcl-list  ===>  local-dcl local-dcl-list | EMPTY
   Can possibly return NULL; It is ok, since it can normally be NULL.
   does not allow function declaration.
   In reality a local declaration has to start with int, but this grammar allows  void x;
   This is an semantic error and it will be checked by a semantics analyzer.
   Observe that any token that is not INT or VOID means the end of local declarations.
 */

TreeNode * local_dcl_list(Boolean * ok){
	TreeNode * node = NULL;
	Boolean status;

	if(TraceParse) {
		fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	node = local_dcl(&status);

	if (node == NULL) return NULL;

	TreeNode *sibling = local_dcl_list(ok);

	if (sibling != NULL) {
			node->rSibling = sibling;
			sibling->lSibling = node;
	}

	*ok = TRUE;
	return node;
}

/* 13   local-dcl		===>  var-dcl | array-dcl
 */
static TreeNode * local_dcl(Boolean * ok){
	TreeNode * node = NULL;
	Boolean status;

	if(TraceParse) {
		fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	node = var_dcl(ok);
	if (node != NULL) {
			*ok = TRUE;
			return node;
	}

	node = array_dcl(ok);
	if (node != NULL) {
			*ok = TRUE;
			return node;
	}

	*ok = FALSE;
	return NULL;
}

/*  14. 
    stmt-list       	===>  stmt-list statement | EMPTY

    According to the grammar, statement-list only appears in compound statement. So the ending token, the one after the last token
    of a statement-list, should be }, token type RCUR.
     between if and else there could be a stmt, but that is a single statement, not a statment list
    A  statement list cannot appear before the end of file. 
 */
TreeNode * stmt_list(Boolean * ok){
	if(TraceParse) {
			fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	TreeNode *first = NULL;
	TreeNode *last  = NULL;
	TreeNode *right = NULL;
	Boolean status;

	while (!check(thisTokenNode, RCUR)) {
		right = statement(&status);

		if (last == NULL) {
			first = last = right;
		}
		else if (right != NULL) {
			last->rSibling = right;
			right->lSibling = last;

			last = right;
		}

		if (status == FALSE)
				return parse_bad_return(first, ok);
	}

	return parse_good_return(first, ok);
}

/*15
 statement 		===>  compound-stmt | selection-stmt | while-stmt | do-while-stmt | for-stmt | return-stmt | null-stmt | expr-stmt
 */
static TreeNode * statement(Boolean * ok){
	if(TraceParse) {
			fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	TreeNode *node = NULL;
	Boolean status;

	switch (thisTokenNode->token->type) {
		case LCUR: node = compound_stmt(&status); break;
		case IF: node = selection_stmt(&status); break;
		case WHILE: node = while_stmt(&status); break;
		case DO: node = do_while_stmt(&status); break;
		case FOR: node = for_stmt(&status); break;
		case RETURN: node = return_stmt(&status); break;
		case SEMI: node = null_stmt(&status); break;
		default : node = expr_stmt(&status); break;
	}

	if (node != NULL && status == TRUE)
			return parse_good_return(node, ok);
	else
			return parse_bad_return(node, ok);
}

/* 16  selection-stmt  ===>  'if '( expression ') statement else-part 
 *
 */
TreeNode * selection_stmt(Boolean * ok){
	if(TraceParse) {
			fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	TreeNode *node = new_stmt_node(SLCT_STMT, thisTokenNode->lineNum);
	Boolean status;

	if (match_move(IF, "selection_stmt(): need IF"))
		if (match_move(LPAREN, "selection_stmt(): need LPAREN"))
			if (node->child[0] = expression(&status), status)
				if (match_move(RPAREN, "selection_stmt(): need RPAREN"))
					if (node->child[1] = statement(&status), status)
						if (node->child[2] = else_part(&status), status)
							return parse_good_return(node, ok);

	return parse_bad_return(node, ok);
}

/*
      17. else-part		===>  'else statement | EMPTY
 */
TreeNode * else_part(Boolean * ok){
	if (check(thisTokenNode, ELSE)) {
		TreeNode * node = NULL;
		Boolean status;
		if(TraceParse) {
				fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
		}

		match_move(ELSE, ""); /* skip else keywork */

		node = statement(&status);

		if (status)
			return parse_good_return(node, ok);
		else
			return parse_bad_return(node, ok);
	}
	else
		return parse_good_return(NULL, ok);
}

/* 18
  while-stmt  ===>  'while '( expression ') statement
 */
static TreeNode * while_stmt(Boolean * ok){
	TreeNode * nd = new_stmt_node(WHILE_STMT, thisTokenNode->lineNum);
	Boolean status;
	if(TraceParse) {
		fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	if(match_move(WHILE, "while_stmt(): need WHILE"))
		if(match_move(LPAREN, "while_stmt(): need LPAREN"))
			if(nd->child[0] = expression(&status), status)
				if(match_move(RPAREN, "while_stmt(): need RPAREN"))
					if(nd->child[1] = statement(&status), status)
						return parse_good_return(nd, ok);
	/* failure */
	return parse_bad_return(nd, ok);
}


/* 19  
do-while-stmt  ===> 'do statement 'while '( expression ')  ';
 */

static TreeNode * do_while_stmt(Boolean * ok){
	TreeNode * node = new_stmt_node(DO_WHILE_STMT, thisTokenNode->lineNum);
	Boolean status;
	if(TraceParse) {
		fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	if (match_move(DO, "do_while_stmt(): need DO"))
		if(node->child[0] = statement(&status), status)
			if (match_move(WHILE, "do_while_stmt(): need WHILE"))
			  if (match_move(LPAREN, "do_while_stmt(): need LPAREN"))
				  if (node->child[1] = expression(&status), status)
					  if (match_move(RPAREN, "do_while_stmt(): need RPAREN"))
					    if (match_move(SEMI, "do_while_stmt(): need SEMI"))
						    return parse_good_return(node, ok);

	return parse_bad_return(node, ok);
}

/* 20 
for-stmt		===>  'for '( expr-or-empty '; expr-or-empty '; expr-or-empty ') statement
 */
static TreeNode * for_stmt(Boolean * ok){
	TreeNode * node = new_stmt_node(FOR_STMT, thisTokenNode->lineNum);
	Boolean status;
	if(TraceParse) {
		fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	if (match_move(FOR, "fo_stmt(): need FOR"))
	  if (match_move(LPAREN, "for_stmt(): need LPAREN"))
		  if(node->child[0] = expr_or_empty(&status), status)
			  if (match_move(SEMI, "for_stmt(): need SEMI"))
		      if(node->child[1] = expr_or_empty(&status), status)
			      if (match_move(SEMI, "for_stmt(): need SEMI"))
				      if (node->child[2] = expr_or_empty(&status), status)
					      if (match_move(RPAREN, "for_stmt(): need RPAREN"))
									if (node->child[3] = statement(&status), status)
						        return parse_good_return(node, ok);

	return parse_bad_return(node, ok);
}

/* 21 Can return NULL. If thisTokenNode is ; or ), then consider empty if found.
   expr-or-empty 	===>  expression | EMPTY
 */
static TreeNode * expr_or_empty(Boolean * ok){
	TreeNode *node = NULL;
	Boolean status;

	if (node = expression(&status), status) {
		if(TraceParse) {
				fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
		}
	}

	return parse_good_return(node, ok);
}

/* 22
  return-stmt  ===>  'return expr-or-empty ';
 */
TreeNode * return_stmt(Boolean * ok){
	TreeNode * node = new_stmt_node(RTN_STMT, thisTokenNode->lineNum);
	Boolean status;

	if(TraceParse) {
		fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	if (match_move(RETURN, "return_stmt(): need RETURN"))
		if(node->child[0] = expr_or_empty(&status), status)
			if (match_move(SEMI, "return_stmt(): need SEMI"))
				return parse_good_return(node, ok);

	return parse_bad_return(node, ok);
}


/* 23  
null-stmt ===> ';

 */
static TreeNode * null_stmt(Boolean * ok){
  TreeNode * node = new_stmt_node(NULL_STMT, thisTokenNode->lineNum);

	if(TraceParse) {
		fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	if (match_move(SEMI, "null_stmt(): need SEMI"))
		return parse_good_return(node, ok);

	return parse_bad_return(node, ok);
}

/*  24 
 expr-stmt  ===>  expression ';
 *
 */
static TreeNode * expr_stmt(Boolean * ok){
	TreeNode * node = new_stmt_node(EXPR_STMT, thisTokenNode->lineNum);
	Boolean status;

	if(TraceParse) {
		fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	if (node->child[0]=expression(&status), status)
	  if (match_move(SEMI, "expr_stmt(): need SEMI"))
		  return parse_good_return(node, ok);

	return parse_bad_return(node, ok);
}


/* 25 
expression		===>  comma-expr
 */
static TreeNode * expression(Boolean * ok){
	if(TraceParse) {
		fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	TreeNode *node = comma_expr(ok);

	if (*ok == TRUE) return parse_good_return(node, ok);
	else return parse_bad_return(node, ok);
}

/* 26 
comma-expr 		===>  comma-expr ', assignment-expr  | assignment-expr
 */
static TreeNode * comma_expr(Boolean * ok){
	if(TraceParse) {
			fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	/* TODO: can build a tree by the way of relational_expr */

	TreeNode *first = NULL;
	TreeNode *last  = NULL;
	TreeNode *right = NULL;
	Boolean status;
	
	while (!(check(thisTokenNode, RPAREN)
							|| check(thisTokenNode, SEMI))) {

		if (right != NULL) {
			if (!match_move(COMMA, "comma_expr() : need COMMA"))
					return parse_bad_return(first, ok);
		}

		right = assignment_expr(&status);
		
		if (last == NULL) {
			first = last = right;
		}
		else if (right != NULL) {
			last->rSibling = right;
			right->lSibling = last;

			last = right;
		}

		if (status == FALSE)
			return parse_bad_return(first, ok);
	}

	return parse_good_return(first, ok);
}

/*
      27. assignment-expr 	===>  equality-expr |  lhs '= assignment-expr
 */

static TreeNode * assignment_expr(Boolean * ok){
	TreeNode * nd = NULL;
	Boolean status;
	if(TraceParse) {
		fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	if(check(thisTokenNode, ID) && check(thisTokenNode->next, ASSIGN))
		goto ASSIGNMENT;
	if(check(thisTokenNode, ID) && check(thisTokenNode->next, LBR)){
		TokenNode * closeBR = find_closing_mark(thisTokenNode->next);
		if(closeBR == NULL) /* did not find the closing ] */
			goto FAILURE;
		if (check(closeBR->next, ASSIGN))
			goto ASSIGNMENT;
	}
	return equality_expr(ok); /* must be an equality-expression */
	ASSIGNMENT:
	nd = new_expr_node(OP_EXPR, thisTokenNode->lineNum);
	nd->attr.exprAttr.op  = ASSIGN;
	if(nd->child[0] = lhs(&status), status)
		if(match_move(ASSIGN, "assignment_expr(): need ASSIGN") )
			if(nd->child[1] = assignment_expr(&status), status)
				return parse_good_return(nd, ok);
	FAILURE:
	return parse_bad_return(nd, ok);
}

/* 28 
lhs 	                ===>  ID | array-element
 */
static TreeNode * lhs(Boolean * ok){
	TreeNode * node = NULL;
	Boolean status;
	if (TraceParse) {
		fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	TokenNode *n0 = thisTokenNode;
	TokenNode *n1 = reach_node(thisTokenNode, 1);

	if (check(n1, LBR)) {
			node = array_element(ok);
			if (*ok == TRUE) return parse_good_return(node, ok);
	}
	else if (check(thisTokenNode, ID)) {
			if (match_move(ID, "lhs(): need ID")) {
					node = new_expr_node(ID_EXPR, n0->lineNum);
					node->attr.exprAttr.name  = 
							copyString(getTokenStr(n0), n0->lineNum);

					return parse_good_return(node, ok);
			}
	}

	return parse_bad_return(node, ok);
}

/* 29 
 array-element	        ===>  ID '[ expression ']
 */
static TreeNode * array_element(Boolean * ok){
	TreeNode * node = NULL;
	Boolean status;

	if (TraceParse) {
		fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	if (match_move(ID, "array_element() : need ID"))
		if (match_move(LBR, "array_element() : need LBR"))
			if (node = expression(&status), status)
				if (match_move(RBR, "array_element() : need RBR"))
					return parse_good_return(node, ok);

	return parse_bad_return(node, ok);
}

/*    30. 
equality-expr	        ===>  equality-expr eqop relational-expr | relational-expr 
 */
static TreeNode * equality_expr(Boolean * ok){
	if (TraceParse) {
		fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	TreeNode *node = relational_expr(ok);
	TreeNode *root  = NULL;

	if (node == NULL) return NULL;

	Boolean status;

	if (check(thisTokenNode, EQ) || check(thisTokenNode, NEQ)) {

			root = eqop(ok);

			root->child[0] = node;
			node = root;

			thisTokenNode = reach_node(thisTokenNode, 1);

			if (node != NULL)
					node->child[1] = equality_expr(ok);
	}
	
	return parse_good_return(node, ok);
}


/* 31.
   eqop 				===>  '== | '!=
   Creates a expr_node, but its two children are not assigned. Other code should handle it.
 */
TreeNode * eqop(Boolean * ok){

  if (TraceParse) {
		fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	if (check(thisTokenNode, EQ) || check(thisTokenNode, NEQ)) {
		TreeNode *node = new_expr_node(OP_EXPR, thisTokenNode->lineNum);
		node->attr.exprAttr.op  = thisTokenNode->token->type;
		
		return parse_good_return(node, ok);
	}

	return parse_bad_return(NULL, ok);
}

/* 32
   relational-expr		===>  relational-expr relop additive-expr | additive-expr
 */

TreeNode * relational_expr(Boolean * ok){
	if (TraceParse) {
		fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	TreeNode *node = additive_expr(ok);
	TreeNode *root  = NULL;

	if (node == NULL) return NULL;

	Boolean status;

	if (check(thisTokenNode, LT) || check(thisTokenNode, LTE)
					|| check(thisTokenNode, GT) || check(thisTokenNode, GTE)) {

			root = relop(ok);

			root->child[0] = node;
			node = root;

			thisTokenNode = reach_node(thisTokenNode, 1);

			if (node != NULL)
					node->child[1] = relational_expr(ok);
	}
	
	return parse_good_return(node, ok);
}

/*33.
  relop  	       	===>  '<= | '< | '> | '>=
 */

TreeNode * relop(Boolean * ok){

  if (TraceParse) {
		fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	if (check(thisTokenNode, LT) || check(thisTokenNode, LTE)
					|| check(thisTokenNode, GT) || check(thisTokenNode, GTE)) {
		TreeNode *node = new_expr_node(OP_EXPR, thisTokenNode->lineNum);
		node->attr.exprAttr.op  = thisTokenNode->token->type;
		
		return parse_good_return(node, ok);
	}

	return parse_bad_return(NULL, ok);
}



/* 34
  additive-expr		===> additive-expr addop multiplicative-expr | multiplicative-expr
  Note that addtive-expression will be expanded into a form like:
   term  addop term  addop term ...
   where term means multiplicative-expr. additive-expr in C- is similar to simple-exp in TINY.
   We have to handle associativity here.  For example the expression 3-4-5-6, which is allowed by the grammar.
   Earlier computed operation should appear at the lower level of the syntax tree.
 */
TreeNode * additive_expr(Boolean * ok){
  if (TraceParse) {
		fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	TreeNode *node = multiplicative_expr(ok);
	TreeNode *root  = NULL;

	if (node == NULL) return NULL;

	Boolean status;

	if (check(thisTokenNode, PLUS) || check(thisTokenNode, MINUS)) {

			root = addop(ok);

			root->child[0] = node;
			node = root;

			thisTokenNode = reach_node(thisTokenNode, 1);

			if (node != NULL)
					node->child[1] = additive_expr(ok);
	}
	
	return parse_good_return(node, ok);
}



/* 35
 addop				===>  '+ | '-
 */
TreeNode *  addop(Boolean * ok){
  if (TraceParse) {
		fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	if (check(thisTokenNode, PLUS) || check(thisTokenNode, MINUS)) {
		TreeNode *node = new_expr_node(OP_EXPR, thisTokenNode->lineNum);
		node->attr.exprAttr.op  = thisTokenNode->token->type;
		
		return parse_good_return(node, ok);
	}

	return parse_bad_return(NULL, ok);
}

/* 36
   multiplicative-expr	===>  multiplicative-expr mulop primary-expr | primary-expr
   multiplicative-expr is similar to term, which is mentioned in the grammar of C- in textbook,
   which is also similar to term in Tiny
   term will be expanded into a form like:
   primary-expr mulop primary-expr mulop primary-expr ...
 */
TreeNode * multiplicative_expr(Boolean * ok){
  if (TraceParse) {
		fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	TreeNode *node = primary_expr(ok);
	TreeNode *root  = NULL;

	if (node == NULL) return NULL;

	Boolean status;

	if (check(thisTokenNode, TIMES) || check(thisTokenNode, OVER)) {

			root = mulop(ok);

			root->child[0] = node;
			node = root;

			thisTokenNode = reach_node(thisTokenNode, 1);

			if (node != NULL)
					node->child[1] = multiplicative_expr(ok);
	}
	
	return parse_good_return(node, ok);
}



/*37
 mulop  ===>  '* | '/
 */
static TreeNode *  mulop(Boolean * ok){
  if (TraceParse) {
		fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	if (check(thisTokenNode, TIMES) || check(thisTokenNode, OVER)) {
		TreeNode *node = new_expr_node(OP_EXPR, thisTokenNode->lineNum);
		node->attr.exprAttr.op  = thisTokenNode->token->type;
		
		return parse_good_return(node, ok);
	}

	return parse_bad_return(NULL, ok);
}


/* 38
 primary-expr      ===>  '( expression ') | ID | array-element | call | 'NUM
 primary-expr is similar to factor mentioned in the grammar of C- in the textbook, which has the form
 factor  ===>  ( expression ) | var | call | NUM
 construct a tree of a factor
 */
TreeNode * primary_expr(Boolean * ok) {
	
  if (TraceParse) {
		fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	TreeNode *node = NULL;
	Boolean status;

	TokenNode *n0 = thisTokenNode;
	TokenNode *n1 = reach_node(thisTokenNode, 1);

	if (check(thisTokenNode, ID)) {
			if (n1 != NULL && check(n1, LBR)) {
				node = array_element(&status);
			}
			else if (n1 != NULL && check(n1, LPAREN)) {
				node = call(&status);
			}
			else {
				node = new_expr_node(ID_EXPR, thisTokenNode->lineNum);
				node->attr.exprAttr.name = copyString(getTokenStr(thisTokenNode), thisTokenNode->lineNum);
				status = TRUE;

				match_move(ID, "primary_expr(): need ID");
			}

			if (status) return parse_good_return(node, ok);
			else return parse_bad_return(node, ok);
	}
	else if (check(thisTokenNode, NUM)) {
			node = new_expr_node(CONST_EXPR, thisTokenNode->lineNum);
			node->attr.exprAttr.val = atoi(getTokenStr(thisTokenNode));

			match_move(NUM, "primary_expr(): need NUM");

			return parse_good_return(node, ok);
	}
	else if (check(thisTokenNode, LPAREN)) {
			if (match_move(LPAREN, "primary_expr(): need LPAREN"))
				if (node = expression(&status), status)
					if (match_move(RPAREN, "primary_expr(): need RPAREN"))
						return parse_good_return(node, ok);
	}

	return parse_bad_return(node, ok);
}

/* 39
 call  			===>  ID '( arg-list ')
 */
static TreeNode * call(Boolean * ok){
	if (TraceParse) {
		fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	TreeNode *node = NULL;

	Boolean status;

	TokenNode *n0 = thisTokenNode;

	if (match_move(ID, "call() : need ID"))
		if (match_move(LPAREN, "call(): need LPAREN")) {

			node = new_expr_node(CALL_EXPR, n0->lineNum);
			node->attr.exprAttr.name = copyString(getTokenStr(n0), n0->lineNum);

			if (node->child[0] = arg_list(&status), status)
				if (match_move(RPAREN, "call(): need RPAREN"))
					return parse_good_return(node, ok);
		}

	return parse_bad_return(node, ok);
}



/* 40
 arg-list		===>  arg-list ', assignment-expr | assignment-expr | EMPTY
 */
static TreeNode * arg_list(Boolean * ok){
	if (TraceParse) {
		fprintf(listing, "%20s  %-10d", __FUNCTION__, thisTokenNode->lineNum);  print_token(thisTokenNode->token); fprintf(listing, "\n");
	}

	TreeNode * first = NULL;
	TreeNode * last =  NULL;
	TreeNode * right = NULL;
	Boolean status;

	while (!check(thisTokenNode, RPAREN)) {

		if (right != NULL) {
			if (!match_move(COMMA, "arg_list(): need COMMA"))
					return parse_bad_return(first, ok);
		}

		right = assignment_expr(&status);
		
		if (last == NULL) {
			first = last = right;
		}
		else if (right != NULL) {
			last->rSibling = right;
			right->lSibling = last;

			last = right;
		}

		if (status == FALSE)
			return parse_bad_return(first, ok);
	}

	/* EMPTY is ok */
	return parse_good_return(first, ok);
}

/*
 * helper functions 
 *
 * Function copyString allocates and makes a new
 * copy of an existing string
 */
static const char * copyString(const char * s, int lineno)
{ 
	int n;
	char * t;

	if (s==NULL) return NULL;

	n = strlen(s)+1;
	t = malloc(n);

	if (t==NULL)
		fprintf(listing,"Out of memory error at line %d\n", lineno);
	else strcpy(t,s);

	return t;
}

static const char* getTokenStr(TokenNode *tn)
{
	return tn->token->string;
}
