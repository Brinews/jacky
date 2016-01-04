/****************************************************
 File: rd_parse.c
 A compiler for the C-Minus language

 MUST CS106 2015 Fall
 Programming designed by the teacher: Liang, Zhiyao
 ****************************************************/

#include "../utility_files/util.h"
#include "../scanner_files/scan.h"
#include "parse.h"
#include "parse_util.h"
#include "parse_print.h"
#include "rd_parser.h"

Bool RD_PARSE_DEBUG = FALSE;  /* by default as false, do not print debug information */

Bool RD_PARSE_KEEP_TREE_ON_ERROR = FALSE; /* by default as false, functions do not keep a parse tree when some error found*/

Bool RD_PARSE_EXIT_ON_ERROR = FALSE;  /* when true, the quit the problem when parser find some error */


/* The syntax grammar of C-
   -  Stings in the grammar are separated by at least one space. Strings include variables and constants.
   -  Variables are the words with all lower-case letters.
   -  Each constants, also called a terminal, is marked with a single-quote, to make it more obvious, terminal names are all upper-case .
   'INT   'WHILE   '<=   ';
   Each constant is a token provided by the scanner. Each different constant in this grammar has a different token type, defined by the scanner. 
   'ID and 'NUM are constants, they  means an id token and a number token, respectively. 
   -  The vertical bar | is used to separate rules. It is not a variable nor a constant.
   -  EMPTY means the empty string, which is often represented as lambda or epsilon in grammars.
   EMPTY is not a variable nor a constant. Rule 0 is used to explain its meaning. There is no token representing EMPTY.
   -  It is a context-free grammar. Each rule has the form:
   variable ===> productionode1 | productionode2 | ...
 */

/*
      shorter names:
      statement 	: stmt
      declaration	: dcl
      expression	: expr
      function		: fun

      Shorter names are used when two names are combined together.


      0.  EMPTY     		===> 

      1.  program    		===>  dcl-list

      2.  dcl-list  		===>  dcl-list declaration | declaration

      3.  declaration		===>  var-dcl | fun-dcl | array-dcl

      4.  type-specifier  	===>  'INT | 'VOID

      5.  var-dcl 			===>  type-specifier 'ID ';

      6.  array-dcl			===>  type-specifier 'ID  '[ 'NUM  '] ';

      7.  fun-dcl	     	===>  type-specifier 'ID '( params ') compound-stmt

      8.  params  			===>  param-list | 'VOID

      9.  param-list 		===>  param-list ', param | param

      10. param  			===>  type-specifier 'ID | type-specifier 'ID '[ ']

      11. compound-stmt  	===>  '{ local-dcl-list  stmt–list '}

      12. local-dcl-list	===>  local-dcl local-dcl-list | EMPTY

      13. local-dcl			===>  var-dcl | array-dcl

      14. stmt-list       	===>  stmt-list statement | EMPTY

      15. statement 		===>  compound-stmt | selection-stmt | while-stmt | do-while-stmt | for-stmt | return-stmt | null-stmt | expr-stmt

      16. selection-stmt  	===>  'if '( expression ') statement else-part 

      17. else-part			===>  'ELSE statement | EMPTY

      18. while-stmt 		===>  'WHILE '( expression ') statement

      19. do-while-stmt 	===>  'do statement 'WHILE '( expression ') ';

      20. for-stmt			===>  'FOR '( expr-or-empty '; expr-or-empty '; expr-or-empty ') statement

      21. expr-or-empty 	===>  expression | EMPTY

      22. return-stmt   	===>  'RETURN expr-or-empty ';

      23. null-stmt			===>  ';

      24. expr-stmt 		===>  expression ';

      25  expression		===>  comma-expr

      26. comma-expr 		===>  comma-expr ', assignment-expr  | assignment-expr

      27. assignment-expr 	===>  equality-expr |  lhs '= assignment-expr

      28. lhs 	           	===>  ID | array-element

      29. array-element	   	===>  ID '[ expression '] | ( expression ) '[ expression ']

      30. equality-expr	   	===>  equality-expr eqop relational-expr | relational-expr

      31. eqop 				===>  '== | '!=

      32. relational-expr		===>  relation-expr relop additive-expr | additive-expr

      33. relop  	       	===>  '<= | '< | '> | '>= 

      34. additive-expr		===>   additive-expr addop multiplicative-expr | multiplicative-expr

      35. addop				===>  '+ | '-

      36. multiplicative-expr	===>  multiplicative-expr mulop primary-expr | primary-expr

      37. mulop  			===>  '* | '/

      38. primary-expr      ===>  '( expression ') | ID | array-element | call | 'NUM

      39. call  			===>  ID '( arg-list ')

      40. arg-list			===>  arg-list ', assignment-expr | assignment-expr | EMPTY

 */


/* function prototypes for recursive calls
 * Each variable in the grammar corresponds to a function.
 * So, N functions corresponding to the N variables in the grammar.
 * The dash in variable names are replaced by underscore,
 * e.g.  variable param-list corresponds to the function param_list().
 */

/* Computation of these functions:
 * - Assuming a list of token-nodes is already provided by the scanner.
 *   The last token node has the END token. Otherwise, the scanner should have reported some error. With assumption is useful since
 *   each function do not need to check if info->thisTokenNode==NULL, since the END token should be reached first, which should indicate
 *   the end of parsing, success or failure. However, be careful to expression like info->thisTokenNode->next->next. Make sure that
 *   NULL->field will never occur.  someTokenNode->prev usually is safe, since normally the previous node had been examined.  However, ->next should be
 *   careful for the case that it is NULL.
 * - The parser consumes the token nodes in the list, one-by-one, from the beginning to the end.
 * - The variable info->thisTokenNode points to the current token node that needs to be processed.
 * - Each function consumes a sequence of token nodes, and constructs a syntax tree according to the grammar.
 * - When success (no error):
 *   -  The function returns a pointer to the top TreeNode in the constructed syntax tree, which is not NULL.
 *   -  The variable info->thisTokenNode moves to the first TokenNode that is not processed, immediately after the
 *      last token node that is already processed.
 * - When failure (error occurred): depending on the value of the global variable RD_PARSE_KEEP_TREE_ON_ERROR: .
 *   - if RD_PARSE_KEEP_TREE_ON_ERROR is FALSE
 *   	-  The function returns NULL.
 *   	-  The spaces that are allocated during this function call to build a tree are freed.
 *   	-  The value of info->thisTokenNode is not specified. It points to some node in the tokenList, not earlier then the place where this function call starts.
 *   - if RD_PARSE_KEEP_TREE_ON_ERROR if TRUE
 *   	- The function stops computation and returns a tree that has been constructed so far.
 * - It is expected that the parser will stop upon the first error detected. For example, when error is detected when constructing a child subtree,
 *   (upon the return of a callee function) then the process of constructing the parent tree stops (the caller function returns ).
 * - When some error occurs during a function call:
 *   -  At least one error message should be printed out.
 *   -  One error message should be printed at the first place where it is detected,
 *      so the exact line number where the error occurs will be printed.
 *   -  When a function, say fun() is called, if some error occurred during the call,
 *      some error message(s) should be printed by this call of fun().  The caller of fun() can also, optionally,
 *      print some error message, since fun() returns NULL and NULL means failure in the situation. Doing so requires
 *      the caller remembering the location of info->thisTokenNode, since fun() will move info->thisTokenNode to later tokens.
 *      So, it is possible to see a stack of error messages, upon one error.  Note that this is optional. This parser
 *      adopts a simple way, the caller often choose to not print error messages, if any error, fun() should have reported it.
 *      In other words, this parser only require that error is reported at the lowest level where some tokens are directly analyzed.
 * - The function type_specifier() does not return a tree, because the type information is only used as a field in a
 *   declaration tree node. this function check if info->thisTokenNode has a INT or VOID token. If so, it returns INT or VOID,
 *   and consumes info->thisTokenNode (move to the next token node). If not, it returns END.
 * - Be careful about NULL
 * - It is not always true that a tree that is a NULL means failure. Especially, EMPTY in the grammar could
 *     represent some NULL tree, and it is ok, not an error.
 * - When some error occurs, the whole program may continue if the variable ExitOnError, which is defined in
 *     main.c, is set as FALSE. So be prepared that after error occurs, the function still need to deal with some
 *     NULL tree.
 * - This parser can be changed in the following ways, which are not done in this parser so far.
 *   - When on error occur, do not stop the parsing, continue to find more errors. One challenge of doing so is
 *     how to put the position of info->thisTokenNode when resuming parsing after error occurs. A possible way is to continue the parsing after error
 *     but do not require  a whole big parse tree is constructed, which is useless now; instead, try to find a token to resume parsing, and
 *     construct several smaller trees. For example, when a WHILE token is met, then parsing a while expression begin. when a IF token is met, then parsing of a selection
 *     statement begin, and so on. The process may ignore some tokens until one token that shows a new beginning of parsing.
 */

static TreeNode * parse(RDinfo * info);

/* By the above grammar, it is easy to know if a variable can produce EMPTY or not. 
   For a variable  A, say A() is the corresponding function of A. However, sometimes a NULL tree can be normal.

   If A() returns a value result of the type TreeNode *,
   then the caller of A() can know if A() is successful or not, depending on the  result->ok
   If result->ok is FALSe, the caller does not need to print error message for this.
      The caller of A() can quickly do failure process: parse_fail_return().

   In the following, if a function marked with "can return NULL", it means in the grammar, the variable can generate EMPTY.
 */

/* 1 */   
static TreeNode * program(RDinfo * info, Bool *  ok);

/* 2 */
static TreeNode * dcl_list(RDinfo * info, Bool *  ok);

/* 3 */
static TreeNode * declaration(RDinfo * info, Bool *  ok);

/* 4 */
static TokenType  type_specifier(RDinfo * info);

/* 5 */
static TreeNode * var_dcl(RDinfo * info, Bool *  ok);

/* 6 */
static TreeNode * array_dcl(RDinfo * info, Bool *  ok);

/* 7 */
static TreeNode * fun_dcl(RDinfo * info, Bool *  ok);

/* 8 */
static TreeNode * params(RDinfo * info, Bool *  ok);

/* 9 */
static TreeNode * param_list(RDinfo * info, Bool *  ok);

/* 10 */
static TreeNode * param(RDinfo * info, Bool *  ok);

/* 11 */
static TreeNode * compound_stmt(RDinfo * info, Bool *  ok);

/* 12   can return NULL */
static TreeNode * local_dcl_list(RDinfo * info, Bool *  ok);

/* 13 */
static TreeNode * local_dcl(RDinfo * info, Bool *  ok);

/* 14 can return NULL */
static TreeNode * stmt_list(RDinfo * info, Bool *  ok);

/* 15 */
static TreeNode * statement(RDinfo * info, Bool *  ok);

/* 16 */ 
static TreeNode * selection_stmt(RDinfo * info, Bool *  ok);

/* 17 can return NULL */
static TreeNode * else_part(RDinfo * info, Bool *  ok);

/* 18  */
static TreeNode * while_stmt(RDinfo * info, Bool *  ok);

/* 19 */
static TreeNode * do_while_stmt(RDinfo * info, Bool *  ok);

/* 20 */
static TreeNode * for_stmt(RDinfo * info, Bool *  ok);

/* 21 Can return NULL */
static TreeNode * expr_or_empty(RDinfo * info, Bool *  ok);

/* 22 */
static TreeNode * return_stmt(RDinfo * info, Bool *  ok);

/* 23 */
static TreeNode * null_stmt(RDinfo * info, Bool *  ok);

/* 24 */
static TreeNode * expr_stmt(RDinfo * info, Bool *  ok);

/* 25 */
static TreeNode * expression(RDinfo * info, Bool *  ok);

/* 26 */
static TreeNode * comma_expr(RDinfo * info, Bool *  ok);

/* 27 */
static TreeNode * assignment_expr(RDinfo * info, Bool *  ok);

/* 28 */
static TreeNode * lhs(RDinfo * info, Bool *  ok);

/* 29 */
static TreeNode * array_element(RDinfo * info, Bool *  ok);

/* 30 */ 
static TreeNode * equality_expr(RDinfo * info, Bool *  ok);

/* 31 */
static TreeNode * eqop(RDinfo * info, Bool *  ok);

/* 32 */
static TreeNode * relational_expr(RDinfo * info, Bool *  ok);

/* 33 */
static TreeNode * relop(RDinfo * info, Bool *  ok);

/* 34 */
static TreeNode * additive_expr(RDinfo * info, Bool *  ok);

/* 35 */
static TreeNode * addop(RDinfo * info, Bool *  ok);

/* 36 */
static TreeNode * multiplicative_expr(RDinfo * info, Bool *  ok);

/* 37 */
static TreeNode * mulop(RDinfo * info, Bool *  ok);

/* 38 */
static TreeNode * primary_expr(RDinfo * info, Bool *  ok);

/* 39 */
static TreeNode * call(RDinfo * info, Bool *  ok);

/* 40  can return NULL */ 
static TreeNode * arg_list(RDinfo * info, Bool *  ok);


/* read() and write() are just functions 
 * It is improper to specify some read_stmt or write_stmt in the grammar.
 static TreeNode * read_stmt(RDinfo * info, Bool *  ok);
 static TreeNode * write_stmt(RDinfo * info, Bool *  ok);
 */

static const char* tokenString(TokenNode *tn)
{
	return tn->token->string;
}

/****************************************
 * the primary function of the parser
 ****************************************
 * Function parse returns the newly constructed parse/syntax tree;
 * and, the tree is saved as the parse-tree field in the parameter info.
 */
TreeNode * parse(RDinfo * info) {
	Bool status;
	info->parseTree = NULL;
	/* always assume one token is read in advance.
	  Starting with first token in the token list. */
	if(RD_PARSE_DEBUG){
		printf( "Function call history : \n");
		printf( "%20s  %-10s%s\n", "function", "LineNum","token");
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);
		print_token(info->thisTokenNode->token);
		printf( "\n");
	}
	if(info->thisTokenNode == NULL) {
		printf( "parse(): The token list is NULL, the scanner is wrong. \n");
		return NULL;
	}
	if(check(info->thisTokenNode, END)){
		printf( "parse(): The token list reaches END before parsing\n");
		return NULL;
	}
	info->parseTree = program(info, &status);
	if (info->parseTree == NULL){
		printf( "parse(): Empty syntax tree after parsing, \n");
		info->parseErrorFound = TRUE;
		return NULL;
	}
	// The last token should be END.
	if(check(info->thisTokenNode, END)==FALSE){
		printf( "Error, parser does not handle some remaining tokens\n");
		info->parseErrorFound = TRUE;
	}
	if(status==FALSE || info->parseErrorFound )
		printf( "Error found in parsing, some error message should have been printed\n");
	connect_parent(NULL, info->parseTree);
	return info->parseTree;
}


/*1   program  ===>  dcl-list */

TreeNode * program(RDinfo * info, Bool *  ok){
	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}
	return  dcl_list(info, ok);
}

/* 2 dcl-list  ===>  dcl-list declaration | declaration   
 * When succeed, returns the first (left-most) node of a list, where each node in the list is returned by declaration().
 */
static TreeNode * dcl_list(RDinfo * info, Bool *  ok){
	TreeNode * first = NULL;
	TreeNode * last =  NULL;
	TreeNode * right = NULL;
	Bool status;
	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}
	// when the end of the sequence is not met, enter the loop
	// it is guaranteed that the token node list with end with END, otherwise
	// the scanner should have reported error.
	while(!check(info->thisTokenNode, END)){
		right = declaration(info, &status);
		if(last == NULL){ // when the first node of declaration list is obtained.
			first = last = right;
		}
		else if(right!=NULL){
			last ->rSibling = right;
			right->lSibling = last;
			last = right;
		}
		if(status==FALSE)
			return parse_bad_return(first, ok);
	}

	return parse_good_return(first, ok); /* success */
}


/* 3  declaration  ===>  var-dcl | fun-dcl | array-dcl 
 * When succeed, returns a tree-noded that is returned by var-dcl(), fun-dcl(), or array-dcl().
 */
static TreeNode * declaration(RDinfo * info, Bool *  ok){
	TreeNode *node;
	*ok = FALSE;

	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}

	/* var declaretion */
	if ((node = var_dcl(info, ok)) != NULL) return parse_good_return(node, ok);
	/* array declaretion */
	if ((node = array_dcl(info, ok)) != NULL) return parse_good_return(node, ok);
	/* fun declaretion */
	if ((node = fun_dcl(info, ok)) != NULL) return parse_good_return(node, ok);

	return parse_bad_return(NULL, ok);

}

/* 4 type-specifier  ===>  'INT | 'VOID
   When the type of the current token is
   int , return INT
   void, return VOID
   Before return, move the current token (info->thisTokenNode) to its next when succeed.

   Otherwise failure, return END, so END works as an error signal here. Report error when failure, without moving the current token node.
 */
static TokenType type_specifier(RDinfo * info){
	if(RD_PARSE_DEBUG)
	{printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");}
	if(check(info->thisTokenNode, INT)) {
		next_token_node(info);
		return INT;
	}
	if(check(info->thisTokenNode, VOID)) {
		next_token_node(info);
		return VOID;
	}
	syntax_error(info, info->thisTokenNode, "type_specifier(): need to have int or void");
	return END;
}


/* 5  var-dcl  ===>  type-specifier 'ID '; 
   When succeed, returns a declaration-node,
   - the kind of this node is variable-declaration,
   - the type of its declaration-attributes is INT_TYPE or VOID_TYPE according to the token type of info->thisTokenNode.
   - the name of its declaration-attributes is the token string of the ID token.
 */
static TreeNode * var_dcl(RDinfo * info, Bool *  ok){
	TreeNode * node = NULL;
	int lineNum = info->thisTokenNode->lineNum;

	TokenNode *savedNode = info->thisTokenNode;


	if (check(info->thisTokenNode, VOID) || check(info->thisTokenNode, INT)) {
		TokenNode *n  = reach_node(info->thisTokenNode, 1);
		TokenNode *node2 = reach_node(info->thisTokenNode, 2);

		if (check(n, ID) && check(node2, SEMI)) {

			if(RD_PARSE_DEBUG){
				printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
			}

			TokenType type = type_specifier(info); /* already move next one token */

			info->thisTokenNode = reach_node(savedNode, 3);

			node = new_dcl_node(VAR_DCL, lineNum);
			node->attr.dclAttr.type = (type == VOID ? VOID_TYPE : INT_TYPE);
			node->attr.dclAttr.name = string_clone(tokenString(n));

			return parse_good_return(node, ok);
		}
	}

	/* restore original node */
	info->thisTokenNode = savedNode;

	*ok = FALSE;
	return NULL;

}

/*6   array-dcl		===>  type-specifier 'ID  '[ 'NUM  '] ';
 * When succeed, returns a declaration node
 * - the declaration-kind of this node is array-declaration.
 * - the type of its declaration-attributes is INT_TYPE or VOID_TYPE according to the return of type-specifier().
 * - the name of its declaration-attributes is the token string of the ID token.
 * - the size of its declaration-attributes is the integer value of the 'NUM token ( can use the atoi() function to
 *   translate the token string of the NUM token into an integer).
 */

static TreeNode*  array_dcl(RDinfo * info, Bool *  ok){
	TreeNode * node = NULL;
	int lineNum = info->thisTokenNode->lineNum;

	TokenNode *savedNode = info->thisTokenNode;
	TokenNode *n  = reach_node(info->thisTokenNode, 1);
	TokenNode *node2 = reach_node(info->thisTokenNode, 2);
	TokenNode *node3 = reach_node(info->thisTokenNode, 3);
	TokenNode *node4 = reach_node(info->thisTokenNode, 4);
	TokenNode *node5 = reach_node(info->thisTokenNode, 5);

	if (check(info->thisTokenNode, VOID) || check(info->thisTokenNode, INT)) {
		if (check(n, ID) && check(node2,LBR) && check(node3, NUM) && check(node4, RBR) && check(node5, SEMI)) {

			if(RD_PARSE_DEBUG){
				printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
			}

			TokenType type = type_specifier(info); /* already move next one token */

			info->thisTokenNode = reach_node(savedNode, 6);

			node = new_dcl_node(ARRAY_DCL, lineNum);
			node->attr.dclAttr.type = (type == VOID ? VOID_TYPE : INT_TYPE);
			node->attr.dclAttr.size = atoi(tokenString(node3));
			node->attr.dclAttr.name = string_clone(tokenString(n));

			return parse_good_return(node, ok);
		}
	}

	*ok = FALSE;
	return NULL;

}

/* 7.  fun-dcl  ===>  type-specifier 'ID '( params ') compound-stmt
 * When succeed, returns a declaration node,
 * - the declaration-kind of this node is function-declaration.
 * the type of its declaration-attributes is INT_TYPE or VOID_TYPE according to the return of type-specifier() .
 * - the name of its declaration-attributes is the token string of the ID token.
 * - child[0] is the what is returned by params().
 * - child[1] is the what is returned by compound-stmt
 */
static TreeNode * fun_dcl(RDinfo * info, Bool *  ok){
	TreeNode *node = new_dcl_node(FUN_DCL, info->thisTokenNode->lineNum);
	Bool status;

	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}

	TokenType type = type_specifier(info); /* already move next one token */
	TokenNode *idNode = info->thisTokenNode;

	if (type != END && match_move(info, ID, "fun-dcl(): need 'ID")) {

			node->attr.dclAttr.type = (type == VOID ? VOID_TYPE : INT_TYPE);
			node->attr.dclAttr.name = string_clone(tokenString(idNode));

			if (match_move(info, LPAR, "fun-dcl(): need LPAR"))
				if (node->child[0] = params(info, &status), status)
					if (match_move(info, RPAR, "fun-dcl(): nedd RPAR"))
						if (node->child[1] = compound_stmt(info, &status), status)
							return parse_good_return(node, ok);
	}

	return parse_bad_return(node, ok);

}

/* 8
   params  ===>  param-list | 'VOID
   when params is void it must surrounded by ( ).
   Also observe that that in a param, after void there must not be an ID, and ) must appear.
   No need to worry about the case of declaration like: void x, since it will be checked by semantics analyzer.

 When succeed,
 - returns a parameter-node when void appears
    - the parameter-kind is void-parameter,
 - or returns what is returned by param_list();
 */
TreeNode * params(RDinfo * info, Bool *  ok){
	TokenNode *node1 = reach_node(info->thisTokenNode, 0);
	TokenNode *node2 = reach_node(info->thisTokenNode, 1);

	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}

	if (check(node1, VOID) && check(node2, RPAR)) {
		TreeNode *node = new_param_node(VOID_PARAM, node1->lineNum);
		node->attr.dclAttr.type = VOID_TYPE;
		info->thisTokenNode = node2;

		return parse_good_return(node, ok);
	}

	return param_list(info, ok);

}

/* 9    param-list  ===>  param-list ', param | param
 * When succeed, returns the first node of a list. Each node in the list is returned by param().
 *
 */
static TreeNode * param_list(RDinfo * info, Bool *  ok){
	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}

	TreeNode *first = NULL;
	TreeNode *last  = NULL;
	TreeNode *right = NULL;
	Bool status;
	
	while (!check(info->thisTokenNode, RPAR)) {

		if (right != NULL) {
			if (!match_move(info, COMMA, "param_list(): need COMMA"))
					return parse_bad_return(first, ok);
		}

		right = param(info, &status); /* parameters */
		
		if (last == NULL) {
			first = last = right;
		} else if (right != NULL) {
			last->rSibling = right;
			right->lSibling = last;

			last = right;
		}

		if (status == FALSE) return parse_bad_return(first, ok);
	}

	return parse_good_return(first, ok);

}

/* 10  param  ===>  type-specifier 'ID | type-specifier 'ID '[  ']
   When succeed, Returns a parameter node.
   - The parameter-kind of the node is either variable-parameter, or array-parameter.
   - the name of the declaration-attribute of this node is the string of ID.
   - the name of the declaration-attribute of this node is INT_TYPE or VOID_TYPE depending on the return of type-specifier.
 */
TreeNode * param(RDinfo * info, Bool *  ok){
	int lookAhead = 0;

	TokenNode *node1 = reach_node(info->thisTokenNode, 1);
	TokenNode *node2 = reach_node(info->thisTokenNode, 2);
	TokenNode *node3 = reach_node(info->thisTokenNode, 3);

	TokenType type = type_specifier(info); /* already move next one token */

	if (type != END) {
			ParamKind pkind = VAR_PARAM;
			if (type == VOID) pkind = VOID_PARAM;

			if (check(node1, ID)) {
				lookAhead = 1;

				if (check(node2, LBR) && check(node3, RBR)) {
					pkind = ARRAY_PARAM;
					lookAhead += 2;
				}

				if(RD_PARSE_DEBUG){
					printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
				}

				TreeNode *node = NULL;
				node = new_param_node(pkind, node1->lineNum);
				node->attr.dclAttr.type = (type == INT ? INT_TYPE : VOID_TYPE);
				node->attr.dclAttr.name = string_clone(tokenString(node1));

				info->thisTokenNode = reach_node(info->thisTokenNode, lookAhead);

				return parse_good_return(node, ok);
			}
	}

	return parse_bad_return(NULL, ok);

}


/*11  compound-stmt  ===>  '{ local-dcl-list stmt–list '}
 * When succeed, returns a statement-node.
 * - the statement-kind of this node is compound-statement.
 * - child[0] is what is returned by local_dcl_list().
 * - child[1] is what is returned by stmt-list().
 */
TreeNode * compound_stmt(RDinfo * info, Bool *  ok){

	Bool status;
	TreeNode *node = new_stmt_node(CMPD_STMT, info->thisTokenNode->lineNum);

	if (match_move(info, LCUR, "compound-stmt(): need LCUR"))
		if (node->child[0] = local_dcl_list(info, &status), status)
			if (node->child[1] = stmt_list(info, &status), status)
				if (match_move(info, RCUR, "compund-stmt(): need RCUR")) {
					if(RD_PARSE_DEBUG){
						printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
					}

					return parse_good_return(node, ok);
				}

	return parse_bad_return(node, ok);

}


/* 12   local-dcl-list  ===>  local-dcl local-dcl-list | EMPTY
   Can possibly return NULL; It is fine, since local-declaration-list can normally be empty.
   Does not allow function declaration locally in a block.
   In reality a local declaration has to start with int, but this grammar allows  void x, which
     is an semantic error and it will be checked by a semantics analyzer.
   Observe that any token that is not INT or VOID means the end of (not part of) local declarations.

   When succeed, returns
   - NULL if info->thisTokenNode is not INT nor VOID
   - or returns the first node in a list, where each node is returned by local_dcl().
 */

TreeNode * local_dcl_list(RDinfo * info, Bool *  ok){
	TreeNode * node = NULL;
	Bool status;

	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}

	node = local_dcl(info, &status);

	if (node == NULL) return NULL;

	TreeNode *sibling = local_dcl_list(info, ok);

	if (sibling != NULL) {
		node->rSibling = sibling;
		sibling->lSibling = node;
	}

	return parse_good_return(node, ok);

}

/* 13   local-dcl		===>  var-dcl | array-dcl
 * If info->thisTokenNode is ID followed by [, returns array_dcl(),
 * otherwise, returns var_dcl().
 */
static TreeNode * local_dcl(RDinfo * info, Bool *  ok){
	TreeNode * node = NULL;
	Bool status;

	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}

	if ((node = var_dcl(info, ok)) != NULL) { return parse_good_return(node, ok); }
	if ((node = array_dcl(info, ok)) != NULL) return parse_good_return(node, ok);

	return parse_bad_return(NULL, ok);

}


/*  14.
    stmt-list       	===>  stmt-list statement | EMPTY

    According to the grammar, statement-list only appears in compound statement. So the ending token, the one after the last token
    of a statement-list, should be }, token type RCUR.
     Between if and else there could be a stmt, but that is a single statement, not a statment list
    A  statement list cannot appear before the end of file. 

    When succeed, returns
    - NULL if info->thisTokenNode is }
    - otherwise, returns the first node in a list, where each node in the list is returned by stmt_list().
 */
TreeNode * stmt_list(RDinfo * info, Bool *  ok){

	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}

	TreeNode *first = NULL;
	TreeNode *last  = NULL;
	TreeNode *right = NULL;
	Bool status;

	while (!check(info->thisTokenNode, RCUR)) {
		right = statement(info, &status);

		if (last == NULL) {
			first = last = right;
		}
		else if (right != NULL) {
			last->rSibling = right;
			right->lSibling = last;

			last = right;
		}

		if (status == FALSE) return parse_bad_return(first, ok);
	}

	return parse_good_return(first, ok);
}

/*15
 statement 		===>  compound-stmt | selection-stmt | while-stmt | do-while-stmt | for-stmt | return-stmt | null-stmt | expr-stmt
 Returns the return of a call to the corresponding function, depending on info->thisTokenNode:
 IF, WHILE, DO, FOR, {, RETURN, or ;
 */
static TreeNode * statement(RDinfo * info, Bool *  ok){

	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}

	TreeNode *node = NULL;
	Bool status;

	switch (info->thisTokenNode->token->type) {
		case LCUR: 	node = compound_stmt(info, &status);	break;
		case IF: 	node = selection_stmt(info, &status); 	break;
		case WHILE: node = while_stmt(info, &status); 		break;
		case DO: 	node = do_while_stmt(info, &status); 	break;
		case FOR: 	node = for_stmt(info, &status); 		break;
		case RETURN:node = return_stmt(info, &status); 		break;
		case SEMI: 	node = null_stmt(info, &status); 		break;
		default : 	node = expr_stmt(info, &status); 		break;
	}

	if (node != NULL && status == TRUE) return parse_good_return(node, ok);
	else 								return parse_bad_return(node, ok);

}

/* 16  selection-stmt  ===>  'if '( expression ') statement else-part
 * When succeed, returns a statement-node
 * - The statement-kind of this node is selection-statement
 * - child[0] is returned by expression().
 * - child[1] is returned by statement().
 * - child[2] is returned by else_part(), which could normally be NULL.
 *
 */
TreeNode * selection_stmt(RDinfo * info, Bool *  ok){

	Bool status;

	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}

	TreeNode *node = new_stmt_node(SLCT_STMT, info->thisTokenNode->lineNum);

	if (match_move(info, IF, "selection_stmt(): need IF") && match_move(info, LPAR, "selection_stmt(): need LPAR"))
			if (node->child[0] = expression(info, &status), status)
				if (match_move(info, RPAR, "selection_stmt(): need RPAR"))
					if (node->child[1] = statement(info, &status), status)
						if (node->child[2] = else_part(info, &status), status)
							return parse_good_return(node, ok);

	return parse_bad_return(node, ok);

}

/*
 17. else-part		===>  'ELSE statement | EMPTY
  If info->thisTokenNode is ELSE, returns statement(),
  otherwise, returns a good-return: NULL
 */
TreeNode * else_part(RDinfo * info, Bool *  ok){

	Bool status;
	TreeNode * node = NULL;

	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}

	if (check(info->thisTokenNode, ELSE)) {

		match_move(info, ELSE, "else_part(): need ELSE"); /* skip else keywork */
		node = statement(info, &status);

		if (status)  return parse_good_return(node, ok);
		else		 return parse_bad_return(node, ok);
	}
	
	return parse_good_return(NULL, ok);
}

/* 18
  while-stmt  ===>  'WHILE '( expression ') statement
  When succeed, returns a statement node
  - the statement kind is while-statement
  - child[0] is returned by expression().
  - child[1] is returned by statement().
 */
static TreeNode * while_stmt(RDinfo * info, Bool *  ok){
	Bool status;
	TreeNode * nd = new_stmt_node(WHILE_STMT, info->thisTokenNode->lineNum);
	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}
	if(match_move(info, WHILE, "while_stmt(): need WHILE") && match_move(info, LPAR, "while_stmt(): need LPAR"))
			if(nd->child[0] = expression(info, &status), status)
				if(match_move(info, RPAR, "while_stmt(): need RPAR"))
					if(nd->child[1] = statement(info, &status), status)
						return parse_good_return(nd, ok);
	return parse_bad_return(nd, ok);
}


/* 19
do-while-stmt  ===> 'do statement 'WHILE '( expression ')  ';
  When succeed, returns a statement node
  - the statement kind is do-while-statement
  - child[0] is returned by statement().
  - child[1] is returned by expression().
 */

static TreeNode * do_while_stmt(RDinfo * info, Bool *  ok){
	TreeNode * node = new_stmt_node(DO_WHILE_STMT, info->thisTokenNode->lineNum);
	Bool status;

	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}

	if (match_move(info, DO, "do_while_stmt(): need DO"))
		if(node->child[0] = statement(info, &status), status)
			if (match_move(info, WHILE, "do_while_stmt(): need WHILE") && match_move(info, LPAR, "do_while_stmt(): need LPAR"))
				  if (node->child[1] = expression(info, &status), status)
					  if (match_move(info, RPAR, "do_while_stmt(): need RPAR") && match_move(info, SEMI, "do_while_stmt(): need SEMI"))
						    return parse_good_return(node, ok);

	return parse_bad_return(node, ok);

}

/* 20
for-stmt		===>  'FOR '( expr-or-empty '; expr-or-empty '; expr-or-empty ') statement
  When succeed, returns a statement node
  - the statement kind is for-statement
  - child[0] is returned by expr_or_empty(), the initialization part.
  - child[1] is returned by expr_or_empty(), the condition part.
  - child[2] is returned by expr_or_empty(), the update part.
  - child[3] is returned by statement().
 */
static TreeNode * for_stmt(RDinfo * info, Bool *  ok){
	TreeNode * node = new_stmt_node(FOR_STMT, info->thisTokenNode->lineNum);
	Bool status;

	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}

	if (match_move(info, FOR, "fo_stmt(): need FOR") && match_move(info, LPAR, "for_stmt(): need LPAR"))
		  if(node->child[0] = expr_or_empty(info, &status), status)
			  if (match_move(info, SEMI, "for_stmt(): need SEMI"))
		         if(node->child[1] = expr_or_empty(info, &status), status)
			        if (match_move(info, SEMI, "for_stmt(): need SEMI"))
				      if (node->child[2] = expr_or_empty(info, &status), status)
					      if (match_move(info, RPAR, "for_stmt(): need RPAR"))
							  if (node->child[3] = statement(info, &status), status)
								return parse_good_return(node, ok);

	return parse_bad_return(node, ok);

}

/* 21 Can return NULL
   expr-or-empty 	===>  expression | EMPTY
   If info->thisTokenNode is ; or ), returns a good-return NULL, do not consume the ; or 0 token.
   Otherwise, returns what is returned by expression().
 */
static TreeNode * expr_or_empty(RDinfo * info, Bool *  ok){
	TreeNode *node = NULL;
	Bool status;

	node = expression(info, &status);

	if (status) {
		if(RD_PARSE_DEBUG){
			printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
		}

		return parse_good_return(node, ok);
	}

	return parse_bad_return(node, ok);
}

/* 22
  return-stmt  ===>  'RETURN expr-or-empty ';
  When succeed, returns a statement-node.
  - The statement-kind of this node is return-statement.
  - child[0] is the return of expr_or_empty();
 */
TreeNode * return_stmt(RDinfo * info, Bool *  ok){
	TreeNode * node = new_stmt_node(RTN_STMT, info->thisTokenNode->lineNum);
	Bool status;

	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}

	if (match_move(info, RETURN, "return_stmt(): need RETURN"))
		if(node->child[0] = expr_or_empty(info, &status), status)
			if (match_move(info, SEMI, "return_stmt(): need SEMI"))
				return parse_good_return(node, ok);

	return parse_bad_return(node, ok);

}


/* 23
null-stmt ===> ';
When succeed, returns a statement-node.
- the statement-kind of this node is null-statement.

 */
static TreeNode * null_stmt(RDinfo * info, Bool *  ok){
	TreeNode * node = new_stmt_node(NULL_STMT, info->thisTokenNode->lineNum);

	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}

	if (match_move(info, SEMI, "null_stmt(): need SEMI")) return parse_good_return(node, ok);

	return parse_bad_return(node, ok);

}

/*  24
 expr-stmt  ===>  expression ';
 When succeed, returns a statement-node
 - the statement-kind of this node is expression-statement.
 - child[0] is the return of expression().
 */
static TreeNode * expr_stmt(RDinfo * info, Bool *  ok){
	TreeNode * node = new_stmt_node(EXPR_STMT, info->thisTokenNode->lineNum);
	Bool status;

	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}

	node->child[0]=expression(info, &status);

	if (status && match_move(info, SEMI, "expr_stmt(): need SEMI")) return parse_good_return(node, ok);

	return parse_bad_return(node, ok);

}


/* 25
expression		===>  comma-expr
returns what is returned by comma_expr().
 */
static TreeNode * expression(RDinfo * info, Bool *  ok){

	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}

	/* comma expression */
	TreeNode *node = comma_expr(info, ok);

	if (*ok == TRUE) return parse_good_return(node, ok);
	else 			 return parse_bad_return(node, ok);

}

/* 26
comma-expr 		===>  comma-expr ', assignment-expr  | assignment-expr
When succeed, returns
- a single node returned by assignment_expr(), if after calling assignment_expr() info->thisTokenNode is not COMMA.
- or, an expression-node, whose expression-kind is operator-expression, and the op field in this expression-attributes is COMMA.
  This node is the top of a tree. child[0] is the top of a smaller and similar tree, and child[1] is node returned by assignment_expr().
 */
static TreeNode * comma_expr(RDinfo * info, Bool *  ok){

	TreeNode *first = NULL;
	TreeNode *last  = NULL;
	TreeNode *right = NULL;
	Bool status;

	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}
	
	while (!(check(info->thisTokenNode, RPAR) || check(info->thisTokenNode, RBR) || check(info->thisTokenNode, SEMI))) {

		if (right != NULL) {
			if (!match_move(info, COMMA, "comma_expr() : need COMMA"))
				return parse_bad_return(first, ok);
		}

		right = assignment_expr(info, &status);
		
		if (last == NULL) {
			first = last = right;
		}
		else if (right != NULL) {
			last->rSibling = right;
			right->lSibling = last;

			last = right;
		}

		if (status == FALSE) return parse_bad_return(first, ok);
	}

	return parse_good_return(first, ok);

}


/*
      27. assignment-expr 	===>  equality-expr |  lhs '= assignment-expr
   When succeed, returns:
   - if the first several tokens has the form:
     ID =
     or
     ID [...] =
     returns an expression-node, whose expression-kind is operator-expression, and the op field in its expression-attribute is ASN.
      - child[0] is returned by lhs()
      - child[1] is returned by assignment_expr().
   - Otherwise,  returns what is is returned by equality_expr().
 */

static TreeNode * assignment_expr(RDinfo * info, Bool *  ok){
	TreeNode * nd = NULL;
	Bool status;
	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}
	if(check(info->thisTokenNode, ID) && check(info->thisTokenNode->next, ASN)) /* ID = ...*/
		goto ASSIGNMENT;
	if(check(info->thisTokenNode, ID) &&  check(info->thisTokenNode->next, LBR)  ) {
		TokenNode * closeBR = find_closing_mark(info->thisTokenNode->next);
		if(closeBR == NULL) {/* did not find the closing ] */
			syntax_error(info, info->thisTokenNode->next, "cannot find the closing ]" );
			goto FAILURE;
		}
		if (check(closeBR->next, ASN))  /* ID[...] =  */
			goto ASSIGNMENT;
	}
	if(check(info->thisTokenNode, LPAR)){
		TokenNode * closePN = find_closing_mark(info->thisTokenNode);
		if(closePN==NULL){
			syntax_error(info, info->thisTokenNode , "cannot find the closing )" );
			goto FAILURE;
		}
		if(check(closePN->next, LBR)) {
			TokenNode * closeBR = find_closing_mark(closePN->next);
			if(closeBR==NULL){
				syntax_error(info, closePN->next , "cannot find the closing ]" );
				goto FAILURE;
			}
			if(check(closeBR->next, ASN))  /* (expr)[...] = */
				goto ASSIGNMENT;
		}
	}
	return equality_expr(info, ok); /* must be an equality-expression */
	ASSIGNMENT:
	nd = new_expr_node(OP_EXPR, info->thisTokenNode->lineNum);
	nd->attr.exprAttr.op  = ASN;
	if(nd->child[0] = lhs(info, &status), status)
		if(match_move(info, ASN, "assignment_expr(): need = ") )
			if(nd->child[1] = assignment_expr(info, &status), status)
				return parse_good_return(nd, ok);
	FAILURE:
	return parse_bad_return(nd, ok);
}

/* 28
lhs 	                ===>  ID | array-element
When succeed:
- if the first several tokens has the form  ID[...], returns what is returned by array_element().
- Otherwise, returns an expression-node
  - the expression-kind is id-expression.
  - the name of its expression-attribute is the string of ID.
 */
static TreeNode * lhs(RDinfo * info, Bool *  ok){
	TreeNode * node = NULL;
	Bool status;

	if(RD_PARSE_DEBUG) {
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}

	TokenNode *node0 = info->thisTokenNode;
	TokenNode *node1 = reach_node(info->thisTokenNode, 1);

	if (check(node1, LBR)) {
		node = array_element(info, ok);
		if (*ok == TRUE) return parse_good_return(node, ok);
	}
	else if (check(info->thisTokenNode, ID)) {
		if (match_move(info, ID, "lhs(): need ID")) {
			node = new_expr_node(ID_EXPR, node0->lineNum);
			node->attr.exprAttr.name  = 
				string_clone(tokenString(node0));

			return parse_good_return(node, ok);
		}
	}

	return parse_bad_return(node, ok);

}


/* 29
   array-element	        ===>  ID '[ expression '] | '( expression ') '[ expression ']
When succeed, returns an expression-node,
- the expression-kind of this node is operator-expression.
- the op field of its expression-attribute is LBR ( representing [ ), which denotes the index operator.
- If info->thisTokenNode is (, child[0] is what is returned by expression().
- Otherwise, if info->thisTokenNode is ID, child[0] is an expression-node with expression-kind as id-expression, and the name in its expression-attribute is the
  string of the id.
- child[1] is what is returned by expression(), corresponding to the expression between [ ].
 */
static TreeNode * array_element(RDinfo * info, Bool *  ok){
	TreeNode * node = NULL;
	Bool status;

	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}

	if (match_move(info, ID, "array_element() : need ID") && match_move(info, LBR, "array_element() : need LBR"))
			if (node = expression(info, &status), status)
				if (match_move(info, RBR, "array_element() : need RBR"))
					return parse_good_return(node, ok);

	return parse_bad_return(node, ok);


}


/*    30.
equality-expr	        ===>  equality-expr eqop relational-expr | relational-expr 
When succeed:
- returns a single node that is returned by relational_expr(), if after calling relational_expr(), info->thisTokenNode is not == or !=.
- or, returns an expression-node that is returned by eqop(),
  This node is the top of a tree. child[0] is the top of a smaller and similar tree, and child[1] is node returned by relational_expr().
*/
static TreeNode * equality_expr(RDinfo * info, Bool *  ok){

	Bool status;

	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}

	TreeNode *node = relational_expr(info, ok);
	TreeNode *subRootNode  = NULL;

	if (node == NULL) return NULL;

	if (check(info->thisTokenNode, EQ) || check(info->thisTokenNode, NEQ)) {

		subRootNode = eqop(info, ok);

		subRootNode->child[0] = node;
		node = subRootNode;

		info->thisTokenNode = reach_node(info->thisTokenNode, 1);

		if (node != NULL) node->child[1] = equality_expr(info, ok);
	}
	
	return parse_good_return(node, ok);

}


/* 31.
   eqop 				===>  '== | '!=
   Creates a expr_node, but its two children are not assigned. Other code should handle it.

 When succeed, returns an expression-node.
 - Its expression-kind is operator-expression.
 - the op field of its expression-attribute is either EQ or NEQ
 - Its child are not assigned with any value by this function.

 */
TreeNode * eqop(RDinfo * info, Bool *  ok){

	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}

	if (check(info->thisTokenNode, EQ) || check(info->thisTokenNode, NEQ)) {
		TreeNode *node = new_expr_node(OP_EXPR, info->thisTokenNode->lineNum);
		node->attr.exprAttr.op  = info->thisTokenNode->token->type;
		
		return parse_good_return(node, ok);
	}

	return parse_bad_return(NULL, ok);

}

/* 32
   relational-expr		===>  relational-expr relop additive-expr | additive-expr
   When succeed:
- returns a single node that is returned by additive_expr(), if after calling additive_expr(), info->thisTokenNode is not one of the relational operators > < <= >=.
- or, returns an expression-node that is returned by relop(),
  This node is the top of a tree. child[0] is the top of a smaller and similar tree, and child[1] is node returned by additive_expr().
 */

TreeNode * relational_expr(RDinfo * info, Bool *  ok){

	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}

	TreeNode *node = additive_expr(info, ok);

	if (node == NULL) return NULL;

	Bool status;

	if (check(info->thisTokenNode, LT) || check(info->thisTokenNode, LTE) || check(info->thisTokenNode, GT) || check(info->thisTokenNode, GTE)) {

		TreeNode *subRootNode  = NULL;
		subRootNode = relop(info, ok);

		subRootNode->child[0] = node;
		node = subRootNode;

		info->thisTokenNode = reach_node(info->thisTokenNode, 1);

		if (node != NULL) node->child[1] = relational_expr(info, ok);
	}

	return parse_good_return(node, ok);

}

/*33.
  relop  	       	===>  '<= | '< | '> | '>=
  When succeed:
  returns an expression-node, whose expression-kind is operator-expression.
  - The op field of its expression-attribute is the token-type corresponding to the the relational operator.
  - The child fields of this node are not assigned by any value in this function.
  */

TreeNode * relop(RDinfo * info, Bool *  ok){

	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}

	if (check(info->thisTokenNode, LT) || check(info->thisTokenNode, LTE) || check(info->thisTokenNode, GT) || check(info->thisTokenNode, GTE)) {

		TreeNode *node = new_expr_node(OP_EXPR, info->thisTokenNode->lineNum);
		node->attr.exprAttr.op  = info->thisTokenNode->token->type;
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

   When succeed:
   - returns a single node that is returned by multiplicative_expr(), if after calling multiplicative_expr(), info->thisTokenNode is not one of the additional-operators + or -.
   - or, returns an expression-node that is returned by addop(),
   This node is the top of a tree. child[0] is the top of a smaller and similar tree, and child[1] is node returned by multiplicative_expr().
   */
TreeNode * additive_expr(RDinfo * info, Bool *  ok){

	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}

	TreeNode *node = multiplicative_expr(info, ok);

	if (node == NULL) return NULL;

	Bool status;

	if (check(info->thisTokenNode, PLUS) || check(info->thisTokenNode, MINUS)) {

		TreeNode *subRootNode  = NULL;
		subRootNode = addop(info, ok);
		subRootNode->child[0] = node;
		node = subRootNode;

		info->thisTokenNode = reach_node(info->thisTokenNode, 1);

		if (node != NULL) node->child[1] = additive_expr(info, ok);
	}

	return parse_good_return(node, ok);

}



/* 35
   addop				===>  '+ | '-
   When succeed:
   returns an expression-node, whose expression-kind is operator-expression.
   - The op field of its expression-attribute is the token-type corresponding to the the additional operator + or -.
   - The child fields of this node are not assigned by any value in this function.
   */
TreeNode *  addop(RDinfo * info, Bool *  ok){

	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}

	if (check(info->thisTokenNode, PLUS) || check(info->thisTokenNode, MINUS)) {
		TreeNode *node = new_expr_node(OP_EXPR, info->thisTokenNode->lineNum);
		node->attr.exprAttr.op  = info->thisTokenNode->token->type;

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

   When succeed:
   - returns a single node that is returned by primary_expr(), if after calling primary_expr(), info->thisTokenNode is not one of the multiplicative-operators * or /.
   - or, returns an expression-node that is returned by mulop(),
   This node is the top of a tree. child[0] is the top of a smaller and similar tree, and child[1] is node returned by primary_expr().

*/
TreeNode * multiplicative_expr(RDinfo * info, Bool *  ok){

	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}

	TreeNode *node = primary_expr(info, ok);

	if (node == NULL) return NULL;

	Bool status;

	if (check(info->thisTokenNode, TIMES) || check(info->thisTokenNode, OVER)) {

		TreeNode *subRootNode  = NULL;

		subRootNode = mulop(info, ok);

		subRootNode->child[0] = node;
		node = subRootNode;

		info->thisTokenNode = reach_node(info->thisTokenNode, 1);

		if (node != NULL) node->child[1] = multiplicative_expr(info, ok); /* right child node */
	}

	return parse_good_return(node, ok);

}



/*37
  mulop  ===>  '* | '/
  When succeed, returns an expression node of the type operator-expression,
  The token type of info->thisTokenNode in info, is saved as the op field in its expression-attribute.
  The child fields of this node are not assigned by any value in this function.
  */
static TreeNode *  mulop(RDinfo * info, Bool *  ok){

	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}

	if (check(info->thisTokenNode, TIMES) || check(info->thisTokenNode, OVER)) {
		TreeNode *node = new_expr_node(OP_EXPR, info->thisTokenNode->lineNum);
		node->attr.exprAttr.op  = info->thisTokenNode->token->type;

		return parse_good_return(node, ok);
	}

	return parse_bad_return(NULL, ok);

}


/* 38
   primary-expr      ===>  '( expression ') | ID | array-element | call | 'NUM

   primary-expr is similar to factor mentioned in the grammar of C- in the textbook, which has the form
   factor  ===>  ( expression ) | var | call | NUM
   construct a tree of a factor

   When succeed: depending on info->thisTokenNode, this function returns :
   - an expression node of id-expression, when 'ID
   - an expression node of constant-expression, when 'NUM
   - an node returned by expression(), or array-element(), or call(), for the other matching cases.
   */
TreeNode * primary_expr(RDinfo * info, Bool *  ok) {

	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}

	TreeNode *node = NULL;
	Bool status;

	TokenNode *node0 = info->thisTokenNode;
	TokenNode *node1 = reach_node(info->thisTokenNode, 1);

	if (check(info->thisTokenNode, ID)) {
		if (check(node1, LBR)) {
			node = array_element(info, &status); /* is an array */
		}
		else if (check(node1, LPAR)) {
			node = call(info, &status); /* is a function call */
		}
		else {
			node = new_expr_node(ID_EXPR, info->thisTokenNode->lineNum);
			node->attr.exprAttr.name = string_clone(tokenString(info->thisTokenNode));
			status = TRUE;

			match_move(info, ID, "primary_expr(): need ID");

			return parse_good_return(node, ok);
		}

		if (status) return parse_good_return(node, ok);
	}
	else if (check(info->thisTokenNode, NUM)) {
		node = new_expr_node(CONST_EXPR, info->thisTokenNode->lineNum);
		node->attr.exprAttr.val = atoi(tokenString(info->thisTokenNode));

		match_move(info, NUM, "primary_expr(): need NUM");

		return parse_good_return(node, ok);
	}
	else if (check(info->thisTokenNode, LPAR)) {
		if (match_move(info, LPAR, "primary_expr(): need LPAR"))
			if (node = expression(info, &status), status)
				if (match_move(info, RPAR, "primary_expr(): need RPAR"))
					return parse_good_return(node, ok);
	}

	return parse_bad_return(node, ok);

}

/* 39
 call  			===>  ID '( arg-list ')
 When succeed, returns an expression node of function-call , and ID's string is saved as the name field of the exprAttr of this node,
 and child[0] is the list returned by arg-list().
 */
static TreeNode * call(RDinfo * info, Bool *  ok){

	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}

	TreeNode *node = NULL;

	Bool status;

	TokenNode *node0 = info->thisTokenNode;

	if (match_move(info, ID, "call() : need ID") && match_move(info, LPAR, "call(): need LPAR")) {

		node = new_expr_node(CALL_EXPR, node0->lineNum);
		node->attr.exprAttr.name = string_clone(tokenString(node0));

		if (node->child[0] = arg_list(info, &status), status)
			if (match_move(info, RPAR, "call(): need RPAR"))
				return parse_good_return(node, ok);
	}

	return parse_bad_return(node, ok);

}



/* 40
 arg-list			===>  arg-list ', assignment-expr | assignment-expr | EMPTY
 When success, returns the first (left-most) node of a list of tree-node. Each one in this list is the top of a tree returned by assignment-expr().
 It can normally return NULL if info->thisTokenNode is ), corresponding to the EMPTY case. Note that the ending ) is not consumed by this function.
 */
static TreeNode * arg_list(RDinfo * info, Bool *  ok){

	if(RD_PARSE_DEBUG){
		printf( "%20s  %-10d", __FUNCTION__, info->thisTokenNode->lineNum);  print_token(info->thisTokenNode->token); printf( "\n");
	}

	TreeNode * first = NULL;
	TreeNode * last =  NULL;
	TreeNode * right = NULL;
	Bool status;

	while (!check(info->thisTokenNode, RPAR)) {

		if (right != NULL) {
			if (!match_move(info, COMMA, "arg_list(): need COMMA"))
					return parse_bad_return(first, ok);
		}

		right = assignment_expr(info, &status);
		
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

/* Free the parse tree whose top is the parameter top. Especially, if top is the same as the parser's parseTree, change
 * its parseTree to NULL.
 */
static void rd_free_tree(Parser * p, TreeNode * top){
	RDinfo* info = (RDinfo *) p->info;
	if(info == NULL){
		puts("Null info of parser found by rd_free_tree");
		return;
	}
	free_tree(info->parseTree);
	if(top == info->parseTree){
		info->parseTree = NULL;
	}
	return;
}

/* Do parsing based on the parser p's info, which contains the token list */
static TreeNode * rd_parse(Parser * p){
	RDinfo* info = (RDinfo *) p->info;
	parse(info);
	return info->parseTree;
}

/* Set a parser's token list field of its information with the parameter lis */
static void rd_set_token_list(Parser * p, TokenList * lis){
	RDinfo * info = (RDinfo *) p->info;
	info->theTokenList = lis;
	if(lis != NULL)
		info->thisTokenNode = lis->head;
	else
		info->thisTokenNode = NULL;
}

/* Wrap print_tree() into a function of the parser to print a parse tree*/
static void rd_print_tree(Parser * p,  TreeNode * tree){
	RDinfo * info = (RDinfo *) p->info;
	if(tree == info->parseTree)
		puts("Now printing parse tree of the parser.");
	print_tree(tree);
}

/* Returns a recursive-descent parser. The token list parameter will be used by the parser*/
Parser * new_rd_parser(TokenList * tklist){
	Parser * parser = (Parser *)malloc(sizeof(Parser));
	RDinfo * info = (RDinfo *)malloc(sizeof(RDinfo));
	info->parseTree = NULL;
	info->theTokenList = tklist;
	if(tklist != NULL)
		info->thisTokenNode = tklist->head;
	else
		info->thisTokenNode = NULL;
	parser->info = info;
	parser->free_tree = rd_free_tree;
	parser->parse = rd_parse;
	parser->set_token_list = rd_set_token_list;
	parser->print_tree = rd_print_tree;
	return parser;
}













