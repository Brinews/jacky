/****************************************************
File: analyze.h
Semantic analyzer interface for TINY compiler
Compiler Construction: Principles and Practice
Kenneth C. Louden
Programming by Zhiyao Liang
MUST FIT FALL 2014
***************************************************/

#ifndef _ANALYZE_H_
#define _ANALYZE_H_

#include "symbtab.h"

// the symbol table on the top level.
extern SymbTab * symbolTable;




/* Function buildSymtab constructs the symbol 
 * table by preorder traversal of the syntax tree
 */
void build_symb_tab(TreeNode *);

/* Procedure typeCheck performs type checking 
 * by a postorder syntax tree traversal
 */
void type_check(TreeNode *);

#endif
