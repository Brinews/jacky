/****************************************************
 File: parse_print.h
 The parser for the C-Minus compiler
 Textbook: Compiler Construction: Principles and Practice
 Book author:  Kenneth C. Louden
 Programming by Zhiyao Liang
 MUST FIT 2014 Fall
****************************************************/

#ifndef _PARSE_PRINT_H_
#define _PARSE_PRINT_H_

#include "parse.h"
/* TreeNode  is defined in parse.h.
 * If parse.h is not here, but some file includes parse_print.h before including
 * parse.h, then some strange error message appears.  */

void print_tree( TreeNode * );

void print_token_type(TokenType );

void print_expr_type(ExprType );

#endif
