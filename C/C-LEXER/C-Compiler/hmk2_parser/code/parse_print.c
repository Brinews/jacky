/****************************************************
 File: parse_print.c
 The parser for the C-Minus compiler
 Textbook: Compiler Construction: Principles and Practice
 Book author:  Kenneth C. Louden
 Programming by Zhiyao Liang
 MUST FIT 2013 Fall
 MUST FIT 2014 Fall
 ****************************************************/
#include "libs.h"
#include "globals.h"
#include "parse.h"

/* macros of  increase/decrease indentation */
//#define INDENT indentNum+=2
//#define UNINDENT indentNum-=2
#define INDENT_GAP 2


void print_expr_type(ExprType t){
	switch(t) {
	case VOID_TYPE: fprintf(listing, "void"); break;
	case INT_TYPE: fprintf(listing, "int"); break;
	case ADDR_TYPE: fprintf(listing, "address"); break;
	default: fprintf(listing, "Error ExpType"); break;
	}
}

/* print_token_type() prints a token type of a token
   and its lexeme to the listing file.
   This function may have some general usage, not just by the parser
   added FOR  13/nov/2014
 */
void print_token_type( TokenType tokenTp){
	switch (tokenTp){
	case IF: fprintf(listing,"if"); break;
	case ELSE: fprintf(listing,"else"); break;
	case WHILE: fprintf(listing,"while"); break;
	case DO: fprintf(listing, "do"); break;
	case FOR: fprintf(listing, "for"); break;
	case INT: fprintf(listing,"INT"); break;
	case VOID: fprintf(listing,"void"); break;
	case RETURN: fprintf(listing,"return"); break;
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
	case EOFILE: fprintf(listing,"EOF"); break;
	case NUM: fprintf(listing, "NUM");  break;
	case ID:  fprintf(listing, "ID");   break;
	case ERROR: fprintf(listing, "ERROR:"); break;
	default: /* should never happen */
		fprintf(listing,"Unknown token type: ");
		break;
	}
}


/* printSpaces indents by printing spaces */
static void print_spaces(int indentNum) {
	int i;
	for (i=0;i<indentNum;i++)
		fprintf(listing," ");
}

/* procedure print_tree prints a syntax tree to the
   listing file using indentation to indicate subtrees
   handle FOR_STMT  13/nov/2014
 */
void print_tree( TreeNode * tree ){
	int i;

	/* Variable indentNum is used by printTree to
	 * store current number of spaces to indent
	 */
	static int indentNum = 0;

	indentNum+= INDENT_GAP;
	while (tree != NULL) {
		print_spaces(indentNum); /* Each case only prints one line, If print more than one line, need use printSpaces() first.*/
		//fprintf(listing,"%d ",  tree->lineNum);
		if (tree->nodeKind == DCL_ND){
			fprintf(listing, "Declare:  ");
			print_expr_type(tree->attr.dclAttr.type);
			fprintf(listing, " %s ", tree->attr.dclAttr.name );
			// print the [size] only if it is an array.
			switch(tree->kind.dcl){
			case ARRAY_DCL:
				fprintf(listing, "[%d]\n", tree->attr.dclAttr.size );
				break;
			case FUN_DCL:
				fprintf(listing, "function with parameters :\n");
				// Function parameters will be saved as child[0] of the node
				break;
			case VAR_DCL:
				// do nothing
				fprintf(listing, "\n");
				break;
			default:
				fprintf(listing,"Unknown DclNode kind\n");
				break;
			}
		}
		else if (tree->nodeKind==PARAM_ND){
			fprintf(listing, "Parameter: ");
			print_expr_type(tree->attr.dclAttr.type);
			if(tree->attr.dclAttr.type != VOID_TYPE){
				fprintf(listing," %s", tree->attr.dclAttr.name);
				if (tree->kind.param == ARRAY_PARAM)
					fprintf(listing, "[ ]");
			}
			fprintf(listing, "\n");
		}
		else if(tree->nodeKind==STMT_ND) {
			switch (tree->kind.stmt) {
			case SLCT_STMT:
				fprintf(listing,"If ");
				if (tree->child[2] != NULL)  // has else part
					fprintf(listing, " with ELSE \n");
				else
					fprintf(listing, " without ELSE \n");
				break;
				//  case ITER_STMTMT:
			case WHILE_STMT:
				fprintf(listing,"while stmt: \n");
				break;
			case FOR_STMT:
				fprintf(listing,"for stmt: \n");
				break;
			case DO_WHILE_STMT:
				fprintf(listing,"do while stmt: \n");
				break;
			case EXPR_STMT:
				fprintf(listing,"Expression stmt: \n");
				break;
			case CMPD_STMT:
				fprintf(listing,"Compound Stmt:\n");
				break;
			case RTN_STMT:
				fprintf(listing,"Return \n");
				//if there is a return value, it is  child[0].
				break;
			case NULL_STMT:
				fprintf(listing, "Null statement:  ;\n");
				break;
			default:
				fprintf(listing,"Unknown StmtNode kind\n");
				break;
			}
		}
		else if(tree->nodeKind==EXPR_ND) {
			switch (tree->kind.expr) {
			case OP_EXPR:
				fprintf(listing,"Operator: ");
				print_token_type(tree->attr.exprAttr.op);
				fprintf(listing,"\n");
				break;
			case CONST_EXPR:
				fprintf(listing,"Const: %d\n",tree->attr.exprAttr.val);
				break;
			case ID_EXPR:
				fprintf(listing,"ID: %s\n",tree->attr.exprAttr.name);
				break;
			case ARRAY_EXPR:
				fprintf(listing,"Array: %s, with member index:\n",tree->attr.exprAttr.name);
				break;
			case CALL_EXPR:
				fprintf(listing,"Call function: %s, with arguments:\n", tree->attr.exprAttr.name);
				break;
				/* arguments are listed as  child[0]
				  remove ASN_EXP, since it is just an operator expression 13/NOV/2014
            case ASN_EXP:
	        fprintf(listing,"Assignment, with LHS and RHS:\n");
	        break;
				 */
			default:
				fprintf(listing,"Unknown ExpNode kind\n");
				break;
			}
		}
		else fprintf(listing,"Unknown node kind\n");
		for (i=0;i<MAX_CHILDREN;i++)
			print_tree(tree->child[i]);
		tree = tree->rSibling;
	}// end of while loop.
	indentNum -= INDENT_GAP;
}






