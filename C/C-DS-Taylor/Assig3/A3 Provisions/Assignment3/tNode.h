/*
 *	Tree Node ADT
 *  Specification for the Tree Node ADT
 *	author Julian Dermoudy
 *	version September 2015
 *	
 *	This file is complete.
*/


//#include <stdbool.h>
#include "gameState.h"

struct tNode_int;
typedef struct tNode_int *tNode;

void init_TNode(tNode *tp,void *o,int l);
void setTNData(tNode t,void *o);
void *getTNData(tNode t);
void setTNLevel(tNode t,int l);
int getTNLevel(tNode t);
void setTNChild(tNode t,tNode n);
tNode getTNChild(tNode t);
void setTNSibling(tNode t,tNode n);
tNode getTNSibling(tNode t);
