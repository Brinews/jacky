/* ~~~~~~~~~~~~~~~Put Your Information Here ~~~~~~~~~~~~~~
 * Student Name:
 * Student ID:
 * Date:
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/***************************************************
File: analyze.c
Semantic analyzer implementation
for the TINY compiler
Compiler Construction: Principles and Practice
CS106 MUST FIT
Provided by Zhiyao Liang
 ***************************************************/

#include "libs.h"
#include "util.h"
#include "globals.h"
#include "scan.h"
#include "parse.h"
#include "analyze.h"
#include "parse_util.h"
#include "parse_print.h"
#include "symbtab.h"

static void top_symbtb_initialize(void);
static void pre_traverse( TreeNode * nd, SymbTab * st, SymbTab* (* pre_proc) (TreeNode *, SymbTab *));
static void post_traverse( TreeNode * nd, void (* post_proc) (TreeNode *));
static SymbTab * pre_proc(TreeNode * t, SymbTab * st);
static void post_proc(TreeNode *);
static void semantic_error(const TreeNode* nd, int errorNum);
static Boolean is_keyword(const char * name);
static Boolean check_int_return(const TreeNode* nd);
static Boolean check_void_return(const TreeNode* nd);
static void check_main(const TreeNode* nd);


// the symbol table on the top level.
SymbTab* symbolTable;

/*  top_STMT_initialize(void)
    [Computation]: 
    - initialize the top-level symbol table. 
    - Add the two dummy functions input and output
      int read(void)
      void write(int x)
      The TreeNode of these to functions are not on the syntax tree. And the body of these two functions are missing.
      We want all built-in functions can be found by the lookup function at the global symbol table.
   [Note]: 
   This function must be called before any other symbol table is initialized, and it should only be called once during the whole program.
 */
static void top_symbtb_initialize(void){
	int j;
	/* add the input() and output() functions. Assume they appear at line 0.*/
	TreeNode * inputNd  = new_dcl_node(FUN_DCL, 0);
	TreeNode * outputNd = new_dcl_node(FUN_DCL, 0);
	TreeNode * p1;
	TreeNode * p2;
	symbolTable = st_initialize();
	/*  The new_dcl_node will assign these fields to be NULL.
	inputNd->parent = outputNd->parent = inputNd->lSibling = inputNd->rSibling = outputNd->lSibling = outputNd->rSibling = NULL;
	 */
	inputNd->attr.dclAttr.type = INT_TYPE;
	outputNd->attr.dclAttr.type = VOID_TYPE;
	inputNd->attr.dclAttr.name = "read";
	outputNd->attr.dclAttr.name = "write";
	p1 = new_param_node(VOID_PARAM, 0);
	p1->attr.dclAttr.type = VOID_TYPE;
	p1->attr.dclAttr.name = "void"; /*this is not required */
	p1->type = VOID_TYPE; /* added */
	/* new_param_node() has already assigned the lineNum with 0.
	 * p1->lineNum = 0;
	 */
	p2 = new_param_node(VAR_PARAM, 0);
	p2->attr.dclAttr.type = INT_TYPE;
	p2->attr.dclAttr.name = "x";
	p2->type = INT_TYPE;
	/* p2->lineNum = 0; */
	inputNd->child[0] = p1;
	outputNd->child[0] = p2;
	st_insert_dcl(inputNd, symbolTable);
	st_insert_dcl(outputNd, symbolTable);
}



/*  pre_traverse()
    [Computation]:  
    - It is a generic recursive syntax tree traversal routine: it applies pre_proc in preorder to the tree pointed to by t.
    [Preconditions]:
    - st is not NULL.
 */
static void pre_traverse( TreeNode * t, SymbTab * st, SymbTab* (* pre_proc) (TreeNode *, SymbTab *)){
	if (t != NULL){
		SymbTab* newSt = pre_proc(t, st);
		int i;
		for (i=0; i < MAX_CHILDREN; i++)
			pre_traverse(t->child[i], newSt, pre_proc);
		pre_traverse(t->rSibling, st, pre_proc);
		/* siblings of the node t share the same scope with nd, since they are in the same same block. */
	}
}


/*  post_traverse()
    [Computation]:  
    - It is a generic recursive syntax tree traversal routine: it applies post_proc in post-order to the tree pointed to by t.
 */
static void post_traverse( TreeNode * t, void (* post_proc) (TreeNode *)){
	if (t != NULL){
		int i;
		for (i=0; i < MAX_CHILDREN; i++)
			post_traverse(t->child[i],post_proc);
		post_proc(t);
		post_traverse(t->rSibling, post_proc);
	}
}


/*  build_symb_tab()
    [Computation]:
    - constructs the symbol table by preorder traversal of the syntax tree
    - pre_proc is applied to each tree node. 
 */
void build_symb_tab(TreeNode * tree){
	/*initialize the symbol_table */
	top_symbtb_initialize();
	pre_traverse(tree, symbolTable, pre_proc);
	check_main(syntaxTree);
}


/* type_check()
   [Computation]: 
   - type_check performs type checking by a post-order syntax tree traversal. 
   - post_proc is applied to each tree node.
 */
void type_check(TreeNode * tree)
{ post_traverse(tree, post_proc);
}

/*  semantic_error()
    [Computation]:
    - Print out error messages according to an error number.
    Semantic errors are numbered as documented in the file c-minus.pdf.
    [Preconditions]:
    - nd must the tree node where some semantic error is detected. 
 */
static void semantic_error(const TreeNode* nd, int errorNum){
	fprintf(listing, "!!! Semantics error %d at line %d : ", errorNum, nd->lineNum);
	errorFound = TRUE;
	switch(errorNum){
	case 1:
		fprintf(listing, "%s is already defined in the current scope.\n\n", nd->attr.dclAttr.name);
		break;
	case 2:
		fprintf(listing, "%s is a keyword, which cannot be a declared name.\n\n", nd->attr.dclAttr.name);
		break;
	case 3:
		fprintf(listing, "%s is a variable, or an array, or a parameter that is not void; it cannot be declared with the type void.\n\n", nd->attr.dclAttr.name);
		break;
	case 4:
		fprintf(listing, "%s is a function with return type int, but a branch of its body does not return an integer.\n\n", nd->attr.dclAttr.name);
		break;
	case 5:
		fprintf(listing, "%s is a function with return type void, but a branch of its body returns an integer.\n\n", nd->attr.dclAttr.name);
		break;
	case 6:
		fprintf(listing, "No proper declaration can be found for the ID %s.\n\n", nd->attr.exprAttr.name);
		break;
	case 7:
		fprintf(listing, "No declaration can be found for the array %s.\n\n", nd->attr.exprAttr.name);
		break;
	case 8:
		fprintf(listing, "No function declaration can be found with the name %s\n\n", nd->attr.exprAttr.name);
		break;
	case 9:
		fprintf(listing, "For the call of the function %s, the number of arguments does not match with the number of parameters of its declaration \n\n", nd->attr.exprAttr.name);
		break;
	case 10:
		fprintf(listing, "For the call of the function %s, one argument has a type that does not match with the type of its corresponding parameter of its declaration \n\n", nd->attr.exprAttr.name);
		break;
	case 11:
		/* fprintf(listing, "The index expression of the array %s does not have int type. \n\n", nd->attr.exprAttr.name); */
		/* fixed an error */
		fprintf(listing, "The index expression does not have int type on the RHS. \n\n");
		break;
	case 12:
		fprintf(listing, "The LHS expression of the index expression does not have address type. \n\n");
		break;
	case 13:
		fprintf(listing, "For the multiplicative operator: ");
		print_token_type(nd->attr.exprAttr.op);  /*print_token_type() is declared in parse_util.h */
		fprintf(listing, " there is an operand that does not have the int type. \n\n");
		break;
	case 14:
		fprintf(listing, "For the relational operator: ");
		print_token_type(nd->attr.exprAttr.op);  /*print_token_type() is declared in parse_util.h */
		fprintf(listing, "it is not true that the two operand expressions both have the integer type, or both have the address type. ");
		break;
	case 15:
		fprintf(listing, "For the additive operator: ");
		print_token_type(nd->attr.exprAttr.op);  /* print_token_type() is declared in parse_util.h */
		fprintf(listing, " there is an operand that has the void type. \n\n");
		break;
	case 16:
		fprintf(listing, "For the plus operator: ");
		print_token_type(nd->attr.exprAttr.op);  /* print_token_type() is declared in parse_util.h */
		fprintf(listing, "both operands are addresses. \n\n");
		break;
	case 17:
		fprintf(listing, "For the assignment operator: ");
		print_token_type(nd->attr.exprAttr.op);  /* print_token_type() is declared in parse_util.h */
		fprintf(listing, " the RHS operand does not have the int type. \n\n");
		break;
	case 18:
		fprintf(listing, "For the assignment operator: ");
		print_token_type(nd->attr.exprAttr.op);  /* print_token_type() is declared in parse_util.h */
		fprintf(listing, " the LHS operand does not have the address type. \n\n");
		break;
	case 19:
		fprintf(listing, "The program does not have the function int main(void). \n\n");
		break;
	default:
		fprintf(listing, "!!! Bad code, semantic_error(), wrong error number: %d\n\n", errorNum);
		break;
	}
}  

static Boolean is_keyword(const char * name){
	if( 	(strcmp(name, "if") == 0) ||
			(strcmp(name, "else") == 0) ||
			(strcmp(name, "while") == 0) ||
			(strcmp(name, "int") == 0) ||
			(strcmp(name, "void") == 0) ||
			(strcmp(name, "do") == 0) ||
			(strcmp(name, "return") == 0) ||
			(strcmp(name, "for") == 0)

	)
		return TRUE;
	else
		return FALSE;
}

/* check_int_return()
 [parameter]:
   nd is the first item in a statement list
 [Return]:
 - TRUE if every branch of the statement list has "return int_EXPR"
 */
static Boolean check_int_return(const TreeNode* nd){
	if(nd == NULL)
		return FALSE;
	if((nd->nodeKind)==STMT_ND){
		switch(nd->kind.stmt){
		case RTN_STMT:  /* note that return ; is legal. */
			if(nd->child[0] != NULL &&  nd->child[0]->type == INT_TYPE)
				return TRUE;
			else
				return FALSE;
			break;
		case SLCT_STMT:
			if(check_int_return(nd->child[2]) && check_int_return(nd->child[1]))
				return TRUE;
			break;
		case CMPD_STMT:
			if(check_int_return(nd->child[1])) // child[0] is local declaration, child[1] is statement list.
				return TRUE;
			break;
		case WHILE_STMT:
			if(check_int_return(nd->child[1]))
				return TRUE;
			break;
		case DO_WHILE_STMT:
			if(check_int_return(nd->child[0]))
				return TRUE;
			break;
		case FOR_STMT:
			if(check_int_return(nd->child[3]))
				return TRUE;
			break;
			/*other cases like  EXPR_STMT  NULL_STMT are ignored. */
		default: break;  /* just to avoid some compiler complain, saying some cases are not handled.*/
		}
	}
	return check_int_return(nd->rSibling);
}


/* check_void_return()
 [parameter]:
   nd is the first item in a statement list
 [Return]:
 - TRUE if every branch of the statement list has  NO "return int_EXPR".
 Described more strongly, no statement like "return integerExpr; " can appear in the statement list.
 One consideration is  Since the return statement will immediately finish a function, only need
 to check the first met return statement, to see if it returns void or not. However, in a full-fledged C
 program, there can be goto statements. So it make sense to follow the strong description.
 */
static Boolean check_void_return(const TreeNode* nd){
	if(nd == NULL)
		return TRUE;
	if(nd->nodeKind==STMT_ND){
		switch(nd->kind.stmt){
		case RTN_STMT:
			if(nd->child[0] != NULL && (nd->child[0]->type != VOID_TYPE))
				return FALSE;
			break;
		case SLCT_STMT:
			if( (check_void_return(nd->child[1]) == FALSE) || (check_void_return(nd->child[2])==FALSE))
				return FALSE;
			break;
		case CMPD_STMT:
			if(check_void_return(nd->child[1])==FALSE)
				return FALSE;
			break;
		case WHILE_STMT:
			if(check_void_return(nd->child[1])==FALSE)
				return FALSE;
			break;
		case DO_WHILE_STMT:
			if(check_void_return(nd->child[0])==FALSE)
				return FALSE;
			break;
		case FOR_STMT:
			if(check_void_return(nd->child[3])==FALSE)
				return FALSE;
			break;
			//other cases  EXPR_STMT  NULL_STMT are ignored.
		default: break;  /* just to avoid some compiler complain, saying some cases are not handled.*/
		}
	}
	return check_void_return(nd->rSibling);
}


/* pre_proc()  
[Parameters]:
- nd is node in the syntax tree.
- st is the symbol table that corresponds to the block where nd appears. st is not NULL (it is initialized). 
[Computation]:
   Detailed description in c-minus.pdf
   [Updating the syntax tree]:
   - Attach a new symbol table to st when a new block is reached.
     A current symbol table is the new one, otherwise, it is st.
   - if the node is a declaration node, insert a bucket list record for this declaration into the current symbol table.
   - if the node is reference of a name, look up in the current symbol table  to find the bucket-list-record of the declaration of the name, and insert a line list record into the bucket list record.
   [Errors that should be detected]
   -  1, 2, 3, 6,  7,  8,  9
[Return]
   - If a new symbol table is attached, return it. Otherwise, return the parameter st.
 */
static SymbTab* pre_proc(TreeNode * nd, SymbTab * st){
	/* !!!!!!!!! Put your code here !!!!!!!!!!!!! */

	SymbTab *st_add = st;
	BucketList bl = NULL;

	switch(nd->nodeKind) {
		case DCL_ND:
		  switch (nd->kind.dcl) {
			  case VAR_DCL:
			  case ARRAY_DCL:
				  /* void x; error */
				  if (nd->attr.dclAttr.type == VOID_TYPE) {
					  semantic_error(nd, 3);
				  }
				  else {

					  SymbTab *st_now = st_add;
					  if (nd->parent == NULL)
						  st_now = symbolTable;

					  bl = st_lookup(st_now, nd->attr.dclAttr.name);

					  if (bl == NULL || bl->nd->nodeKind != DCL_ND
							  || st_now != bl->st) {
						  /* add new bucketlist node */
						  st_insert_dcl(nd, st_now);

						  if (nd->kind.dcl == ARRAY_DCL)
							  nd->type = ADDR_TYPE;
						  else
							  nd->type = INT_TYPE;
					  }
					  else {
						  if (st_now == bl->st)
							  semantic_error(nd, 1);
					  }
				  }

				  break;

			  case FUN_DCL:
				  bl = st_lookup(symbolTable, nd->attr.dclAttr.name);
				  if (bl == NULL) {
					  st_insert_dcl(nd, st_add);

					  nd->type = nd->attr.dclAttr.type;
				  }
				  else {
					  semantic_error(nd, 1);
				  }

				  /* add new symtab */
				  st_add = st_attach(symbolTable);

				  break;
		  }

		  break;

		case PARAM_ND:
		  switch (nd->kind.dcl) {
			  case VAR_DCL:
			  case ARRAY_DCL:
				  /* void x; error */
				  if (nd->attr.dclAttr.type == VOID_TYPE
						  && (NULL != nd->attr.dclAttr.name
							  || "" != nd->attr.dclAttr.name))
					  semantic_error(nd, 1);
				  else {
					  bl = st_lookup(st_add, nd->attr.dclAttr.name);
					  if (bl != NULL && bl->nd->nodeKind == PARAM_ND)
						  semantic_error(nd, 1);
					  else {
						  st_insert_dcl(nd, st_add);
						  nd->type = nd->attr.dclAttr.type;

						  if (nd->kind.dcl == ARRAY_DCL)
							  nd->type = ADDR_TYPE;
					  }
				  }
			  break;
		  }
			
		  break;

		/* for ref */
		case EXPR_ND:
		  switch (nd->kind.expr) {
			  case CONST_EXPR:
				  nd->type = INT_TYPE;
				  break;

			  case ID_EXPR:
				  bl = st_lookup(st_add, nd->attr.exprAttr.name);
				  if (bl == NULL) {
					  semantic_error(nd, 6);
				  }

				  if (bl != NULL) {
					  st_insert_ref(nd, bl);

					  /* update exprtype */
					  nd->type = bl->nd->type;
				  }

				  break;

			  case CALL_EXPR:

				  bl = st_lookup(st, nd->attr.exprAttr.name);

				  if (bl == NULL)
					  semantic_error(nd, 8);
				  else {
					  /* declaration */
					  int para_count = 0;
					  TreeNode *node = bl->nd->child[0];
					  while (node != NULL && 
							  node->nodeKind == PARAM_ND) {
						  para_count++;

						  node = node->rSibling;
					  }

					  /* function with void as parameter */
					  if (para_count == 1
							  && bl->nd->child[0]->type == VOID_TYPE)
						  para_count--;


					  /* caller */
					  int call_paras = 0;
					  node = nd->child[0];

					  while (node != NULL) {
						  ++call_paras;
						  node = node->rSibling;
					  }

					  /*
					  printf("Dn=%d,Cn=%d\n", 
							  para_count, call_paras);
					  */

					  if (para_count != call_paras)
						  semantic_error(nd, 9);
					  else {
						  st_insert_ref(nd, bl);
						  /* update exprtype */
						  nd->type = bl->nd->attr.dclAttr.type;
					  }
				  }

				  break;
		  }
		  break;

		default:
		  break;
	}

	return st_add;
}


/* post_proc()
[Parameters]:
- nd is node in the syntax tree.
[Computation]:
   Detailed description in c-minus.pdf
   [Updating the syntax tree]:
   -  Update the type field of an expression node.
   [Errors that should be detected]
   -  10, 11, 12, 13, 14, 15, 16, 17, 18,  4, 5
 */
static void post_proc(TreeNode * nd){
	/* !!!!!!!!! Put your code here !!!!!!!!!!!!! */
	BucketList bl = NULL;
	TreeNode *node = NULL, *trn = NULL;

	switch(nd->nodeKind) {
	  case DCL_ND:
		if (nd->kind.dcl == FUN_DCL) {
			if (nd->attr.dclAttr.type == INT_TYPE) {
				if (check_int_return(nd->child[1]) == FALSE)
					semantic_error(nd, 4);
			}
			else if (nd->attr.dclAttr.type == VOID_TYPE) {
				if (check_void_return(nd->child[1]) == FALSE)
					semantic_error(nd, 5);
			}
		}
		break;

	  case EXPR_ND:
	    switch (nd->kind.expr) {
		  case CALL_EXPR:
			  /* find definition */
			  bl = st_lookup(symbolTable, nd->attr.exprAttr.name);
			  
			  /* para-meters type check */
			  trn = NULL;

			  /* definition parameters */
			  node = bl->nd->child[0];
			  trn = nd->child[0];

			  while (node != NULL 
					  && node->type != VOID_TYPE 
					  && node->nodeKind == PARAM_ND) {


				  /* printf("%d,%d\n", node->type, trn->type); */

				  if (trn  == NULL || trn->type != node->type) {
					  semantic_error(nd, 10);
				  }

				  trn = trn->rSibling;
				  node = node->rSibling;
			  }

			  break;

		  case OP_EXPR:

			  if (nd->attr.exprAttr.op == LBR) {
				  node = nd->child[0];

				  if (node->type != ADDR_TYPE)
					  semantic_error(node, 12);

				  trn = nd->child[1];
				  
				  if (trn->type != INT_TYPE)
					  semantic_error(trn, 11);

				  nd->type = INT_TYPE;
			  }
			  else if (nd->attr.exprAttr.op == TIMES
					  || nd->attr.exprAttr.op == OVER) {

				  node = nd->child[0];

				  if (node->type != INT_TYPE) {
					  semantic_error(nd, 13);
				  }

				  node = nd->child[1];

				  if (node->type != INT_TYPE) {
					  semantic_error(nd, 13);
				  }

				  /* update exprtype */
				  nd->type = nd->child[0]->type;
			  }
			  else if (nd->attr.exprAttr.op == PLUS
					  || nd->attr.exprAttr.op == MINUS) {

				  node = nd->child[0];

				  if (node->type == VOID_TYPE)
					  semantic_error(nd, 15);

				  node = nd->child[1];

				  if (node->type == VOID_TYPE)
					  semantic_error(nd, 15);

				  if (nd->attr.exprAttr.op == PLUS) {
					  node = nd->child[0];
					  trn = nd->child[1];

					  if (node->type == ADDR_TYPE &&
							  trn->type == ADDR_TYPE)
						  semantic_error(nd, 16);
				  }

				  /* update exprtype */
				  nd->type = nd->child[0]->type;

				  if (nd->attr.exprAttr.op == MINUS) {
					  if (nd->child[0]->type == ADDR_TYPE
							  && nd->child[1]->type == ADDR_TYPE)
						  nd->type = INT_TYPE;
				  }
			  }
			  else if (nd->attr.exprAttr.op == LT ||
					  nd->attr.exprAttr.op == LTE ||
					  nd->attr.exprAttr.op == GT ||
					  nd->attr.exprAttr.op == GTE ||
					  nd->attr.exprAttr.op == EQ ||
					  nd->attr.exprAttr.op == NEQ) {
				  node = nd->child[0];
				  trn = nd->child[1];

				  if (!(node->type == INT_TYPE 
							  && node->type == trn->type) 
						  && !(node->type == ADDR_TYPE 
							  && node->type == trn->type)) 
					  semantic_error(nd, 14);

				  nd->type = INT_TYPE;
			  }
			  else if (nd->attr.exprAttr.op == ASSIGN) {

				  node = nd->child[0];

				  /* why use address type ? */
				  if (node->type != INT_TYPE)
					  semantic_error(nd, 18);

				  node = nd->child[1];

				  if (node->type != INT_TYPE)
					  semantic_error(nd, 17);

				  nd->type = nd->child[0]->type;
			  }

			  break;
		}
		break;
		
	  default:
		break;
	}
}

/*
 * returns  without complain if in the program there is a function definition of int main(void)
 * Otherwise, call semantic_error(syntaxTree, 19);
 */
static void check_main(const TreeNode* nd){
	/* !!!!!!!!! Put your code here !!!!!!!!!!!!! */
	BucketList bl = st_lookup(symbolTable, "main");

	if (bl != NULL) {

		TreeNode *tn = bl->nd;
		if (tn != NULL 
				&& tn->type == INT_TYPE
				&& tn->child[0]->type == VOID_TYPE)
			return;
	}

	semantic_error(syntaxTree, 19);
}
