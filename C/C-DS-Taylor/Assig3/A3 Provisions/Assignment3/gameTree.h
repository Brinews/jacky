/*
 *	GameTree ADT
 *	Specification for the GameTree ADT
 *	author Julian Dermoudy
 *	version September 2015
 *	
 *	This file is complete.
*/

#include <stdbool.h>
#include "assig_three215.h"
#include "stack.h"

struct gameTree_int;
typedef struct gameTree_int *gameTree;

void init_gameTree(gameTree *gp,bool e,void *o,int l);
bool isEmptyGT(gameTree g);
void setData(gameTree g,void *o);
void *getData(gameTree g);
void setLevel(gameTree g,int l);
int getLevel(gameTree g);
void setChild(gameTree g,gameTree c);
gameTree getChild(gameTree g);
void setSibling(gameTree g,gameTree s);
gameTree getSibling(gameTree g);
void generateLevelDF(gameTree g,stack s);
void buildGameDF(gameTree g,stack s,int l);
void update(gameTree g);
double chooseBest(gameTree g);
int findBest(gameTree g,double w);
void findMove(gameTree g,int c);
char *toStringGT(gameTree g);

