/****************************************************
 File: parse_util.h                               
 The interface of the utility tools of the parser 
 for the C-Minus compiler   
 Textbook: Compiler Construction: Principles and Practice   
 Book author:  Kenneth C. Louden                  
 Programming by Zhiyao Liang                      
 MUST FIT 2014 Fall
****************************************************/

// This file should only be used by parse.c, since all it does is for helping parsing.

#ifndef _PARSE_UTIL_H_
#define _PARSE_UTIL_H_

#include "util.h"

void print_token_type( TokenType );

TreeNode * new_stmt_node(StmtKind, int);

TreeNode * new_expr_node(ExprKind, int );

TreeNode * new_dcl_node(DclKind, int);

TreeNode * new_param_node(ParamKind, int);

void syntax_error( const TokenNode *, const char *);

Boolean match(const TokenNode *, TokenType,  const char * );

void next_token_node(void);

Boolean match_move(TokenType expected, const char * msg);

TokenNode * reach_node(const TokenNode * nd, int steps);

Boolean check(const TokenNode * nd, TokenType tp);

TokenNode* find_closing_mark(const TokenNode* from);

void connect_parent(TreeNode * parent, TreeNode * nd);

void free_tree(TreeNode * nd);

extern TokenNode* thisTokenNode;

TreeNode * parse_bad_return(TreeNode * top, Boolean * ok);

TreeNode * parse_good_return(TreeNode * top, Boolean * ok);

#endif
