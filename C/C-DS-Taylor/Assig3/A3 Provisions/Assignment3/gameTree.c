/*
 *	GameTree ADT
 *	author <<Your names here>>
 *	version September 2015
 *	
 *	This file holds the GameTree ADT which is a
 *	general game tree.  The GameTree is built using
 *	TNode ADTs.  A GameTree variable consists of a
 *	"root" field which refers to a TNode variable
 *	which has a "data" field and "child" and
 *	"sibling" references, and a "level" value.
 *
 * YOU NEED TO COMPLETE THIS FILE. 
*/


//#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "tNode.h"
#include "gameTree.h"
#include "assig_three215.h"


struct gameTree_int
{
	tNode root;			// the node at the top of the tree
};

/*
	*	GameTree
	*	initialiser function.
	*	Pre-condition: none
	*	Post-condition: if the parameter value (e) is true then the 
	*					GameTree variable's "root" field is set to NULL
	*					otherwise the GameTree variable's "root" field
	*					refers to a new TNode variable containing the
	*					parameter value (o) of level with parameter 
	*					value (l) with a NULL child, and a NULL sibling
	*	Informally: creates either an empty tree or a leaf node as
	*				required
*/
void init_gameTree(gameTree *tp,bool e,void *o,int l)
{
	trace("GameTree: initialiser starts");
	
	(*tp) = (gameTree) malloc(sizeof(struct gameTree_int));

	if (e)
		(*tp)->root = NULL;
	else
		init_TNode(&((*tp)->root), o, l);
		
	trace("GameTree: initialiser ends");
}
	
	
/*
	*	isEmptyGT
	*	Emptiness test.
	*	Pre-condition: none
	*	Post-condition: true is returned if the GameTree variable is
	*					empty, false is returned otherwise
	*	Informally: indicate if the GameTree contains no nodes
	*
	*	return boolean whether or not the game tree is empty
*/
bool isEmptyGT(gameTree t)
{
	trace("isEmptyGT: isEmptyGT starts and ends");
		
	return t == NULL || t->root == NULL;
}


/*
	*	getData
	*	Get function for "root" field's data value.
	*	Pre-condition: none
	*	Post-condition: the value of the GameTree variable's data
	*					field is returned
	*	Informally: return the value within the root node
	*
	*	return (void *) the data item of the root node
*/
void *getData(gameTree t)
{
	trace("getData: getData starts");
		
	if (isEmptyGT(t))
	{
		fprintf(stderr,"getData: empty game tree");
		exit(1);
	}

	trace("getData: getData ends");
	return getTNData(t->root);
}
	
	
/*
	*	getLevel
	*	Get function for "root" field's level value.
	*	Pre-condition: none
	*	Post-condition: the value of the GameTree variable's root
	*					field's level is returned
	*	Informally: return the level value within the root node
	*
	*	return int the level number of the root node
*/
int getLevel(gameTree t)
{
	trace("getLevel: getLevel starts");

	if (isEmptyGT(t))
	{
		fprintf(stderr,"getData: empty game tree");
		exit(1);
	}	

	trace("getLevel: getLevel ends");
	return getTNLevel(t->root);
}	
	

/*
	*	getChild
	*	Get function for "root" field's child value.
	*	Pre-condition: none
	*	Post-condition: the value of the GameTree variable's child
	*					field is returned in a newly
	*					constructed GameTree variable
	*	Informally: return the GameTree variable's child
	*
	*	return GameTree the eldest child of the current node
*/
gameTree getChild(gameTree t) 
{		
	trace("getChild: getChild starts");
		
	if (isEmptyGT(t))
	{
		fprintf(stderr,"getData: empty game tree");
		exit(1);
	}	

	trace("getChild: getChild ends");

	return getTNChild(t->root);
}


/*
	* 	getSibling
	*	Get function for "root" field's sibling value.
	*	Pre-condition: none
	*	Post-condition: the value of the GameTree variable's sibling
	*					field is returned in a newly constructed 
	*					GameTree variable
	*	Informally: return the GameTree variable's sibling
	*
	*	return GameTree the next sibling of the current node
*/
gameTree getSibling(gameTree t) 
{
	trace("getSibling: getSibling starts");
		
	if (isEmptyGT(t))
	{
		fprintf(stderr,"getData: empty game tree");
		exit(1);
	}	

	trace("getSibling: getSibling ends");

	return getTNSibling(t->root);
}


/*
	*	setData
	*	Set function for "root" field's data field.
	*	Pre-condition: none
	*	Post-condition: the TNode variable's data field is altered to
	*					hold the given (o) value
	*	Informally: store the given value in the root node of the
	*				GameTree variable
	*
	*	param (void *) o to install as data for root node
*/
void setData(gameTree t,void *o)
{
	trace("setData: setData starts");
		
	setTNData(t->root, o);
		
	trace("setData: setData ends");
}
		
	
/*
	*	setLevel
	*	Set function for "root" field's level field.
	*	Pre-condition: none
	*	Post-condition: the TNode variable's level field is altered
	*					to hold the given (l) value
	*	Informally: assign the given value as the level of the
	*				GameTree variable
	*
	*	param int l level number for root of current game tree
*/
void setLevel(gameTree t,int l)
{
	trace("setLevel: setLevel starts");
		
	setTNLevel(t->root, l);
		
	trace("setLevel: setLevel ends");
}
	
	
/*
	*	setChild
	*	Set function for "root" field's child field.
	*	Pre-condition: none
	*	Post-condition: the TNode variable's child field is altered
	*					to hold the given (c) value
	*	Informally: assign the given value as the child of the
	*				GameTree variable
	*
	*	param gameTree c GameTree to be set as eldest child of current game tree
*/
void setChild(gameTree t,gameTree c)
{
	trace("setChild: setChild starts");
		
	setTNChild(t->root, c);
		
	trace("setChild: setChild ends");
}
	
	
/*
	*	setSibling
	*	Set function for "root" field's sibling field.
	*	Pre-condition: none
	*	Post-condition: the TNode variable's sibling field is altered
	*					to hold the given (s) value
	*	Informally: assign the given value as the sibling of the
	*				GameTree variable
	*
	*	param gameTree s GameTree to be set as next sibling of current game tree
*/
void setSibling(gameTree t,gameTree s)
{
	trace("setSibling: setSibling starts");

	setTNSibling(t->root, s);

	trace("setSibling: setSibling ends");
}
	
	
/*
	*	generateLevelDF
	*	Generate the next level of the tree
	*	Pre-condition: the given tree is defined and the given stack
	*				   is defined, i.e. these variables are not NULL
	*	Post-condition: an additional level of possible moves has
	*					been added to the given game tree and each
	*					tree node of the new level also has been
	*					pushed onto the stack.  Each move is for
	*					the human player if the level number of the
	*					level is even, and for the computer player
	*					otherwise.  Each state in the new level has
	*					a value calculated from the opponent's
	*					perspective
	*	Informally: generate the next level of the game tree
	*
	*	param stack k the stack of reachable but unexpanded game trees
*/
void generateLevelDF(gameTree t,stack k)
{
	gameTree ct;
	gameState gs;
	
	int level = getLevel(t) + 1;
	int moves = 1;
	int remind = getCount((gameState)getData(t));

	trace("generateLevelDF: generateLevelDF starts");

	if (moves > remind) return;

	init_gameState(&gs, remind-moves, 0);
	setWorth(gs, evaluateState(gs, (level-1)%2));

	init_gameTree(&ct, false, gs, level);

	setChild(t, ct); /* add  child node */
	push(k, ct);

	for (moves = moves + 1; moves <= MAX_MOVES && moves <= remind; moves++) {
		gameTree st;

		init_gameState(&gs, remind - moves, 0);
		setWorth(gs, evaluateState(gs, (level-1)%2));

		init_gameTree(&st, false, gs, level);

		setSibling(ct, st); /* add sibling node */
		ct = st;

		push(k, st);
	}
		
	trace("generateLevelDF: generateLevelDF ends");
}
	
	
/*
	*	buildGameDF
	*	Generate the game tree in a depth-first manner
	*	Pre-condition: the given game tree is defined, the given stack
	*				   is defined, i.e. these are not NULL, and the 
	*				   given int value represents the desired depth of
	*				   the tree
	*	Post-condition: if the given tree is empty then it is replaced
	*					with a new tree at level 0 comprising a new
	*					game with the maximum number of matches and 0
	*					worth (and this new tree is pushed onto the
	*					stack).  If the given tree isn't empty and it
	*					is not already deep enough, if there are
	*					children, then these are traversed and all
	*					siblings are pushed onto the stack.  If (when)
	*					there are no children, an additional level of
	*					possible moves is added to the given game tree
	*					by calling generateLevelDF(), and each tree
	*					node of the new level also is pushed onto the
	*					stack.  Finally, the next tree to be considered
	*					is determined by removing the top of the stack
	*					and the process continues until the stack is
	*					empty
	*	Informally: generate the game tree from the current point
	*				in a depth-first manner until it is "d" levels
	*				deep
	*
	*	param stack k Stack of reachable but unexpanded game trees
	*	param int d desired depth (number of moves ahead) that game tree should be built to
*/
void buildGameDF(gameTree t, stack k, int d)
{
	trace("buildGameDF: buildGameDF starts");
		
	if (isEmptyGT(t)) {
		gameState s;
		init_gameState(&s, MAX_MATCHES, 0);
		init_gameTree(&t, false, s, 0);
	} else if (!isEmptyGT(getChild(t))) {
		gameTree c = getChild(t);
		while (! isEmptyGT(c)) {
			push(k, c);
			c = getSibling(c);
		}
	} else {
		generateLevelDF(t, k);
	}

	while (! isEmptyS(k)) {
		gameTree gt = (gameTree) top(k);
		pop(k);

		if (getLevel(gt) < d) {

			if (isEmptyGT(getChild(gt))) {
				generateLevelDF(gt, k);
			} else {
				gameTree ct = getChild(gt);
				while(!isEmptyGT(ct)) {
					push(k, ct);
					ct = getSibling(ct);
				}
			}
		}
	}

	trace("buildGameDF: buildGameDF ends");
}


/*
	*	adjustLevel
	*	Adjust the level numbers of the tree
	*	Pre-condition: The game tree is not NULL
	*	Post-condition: all level numbers in the tree are reduced
	*					by one
	*	Informally: decrement the level number of all nodes in
	*				the game tree after the layer above has been
	*				removed
*/
void adjustLevel(gameTree t)
{
	char msg[100];	// string for building up trace() messages

	trace("adjustLevel: adjustLevel starts");
		
  	if (! isEmptyGT(t))
	{
    	adjustLevel(getChild(t));	// recursively adjust all in the sub-tree
    	adjustLevel(getSibling(t));	// recursively adjust all in the adjacent tree
		sprintf(msg,"adjustLevel: setting level to %d",(getLevel(t)-1));
    	trace(msg);
    	setLevel(t,getLevel(t)-1);	// make the change to the level number
  	}
  		
	trace("adjustLevel: adjustLevel ends");
}


/*
	*	chooseBest
	*	Find the number of matches for the best move in the tree
	*	Pre-condition: the given game tree is not NULL
	*	Post-condition: the best move that can be made is determined
	*					and information about it (its worth and
	*					frequency) is returned
	*	Informally: examine the next alternate moves and return
	*				the worth of the best one
	*
	*	return double signed floating point number where signed
	*				  whole number is the worth of the best
	*				  move available and floating point part is
	*				  a tenth of the number of moves of this
	*				  worth available
*/
double chooseBest(gameTree t)
{
  	int w;			// worth of moves in the game tree
  	int f;			// frequency of moves found in the game tree
  	int v;			// worth of move which is most valuable
	double a;		// combination of f and v and result of function
	gameTree c;		// loop counter for sibling traversals
	int m;			// number of matches remaining in the game
	char msg[100];	// string for building up trace() messages
  
	trace("chooseBest: chooseBest starts");
	
	// initialise variables using eldest child
	f=1;
	c=getChild(t);
  	v=getWorth((gameState)getData(c));
	m=getCount((gameState)getData(c));
	sprintf(msg,"chooseBest: starting at %d with a worth of %d",m,v);
  	trace(msg);
  		
  	while (! isEmptyGT(getSibling(c)))	// traverse siblings looking for more important (and as important) nodes
  	{
    	c=getSibling(c);
  		w=getWorth((gameState)getData(c));
		sprintf(msg,"chooseBest: considering a worth of %d",w);
    	trace(msg);
    		
    	if (w == v)	// as important as current best worth
    	{
    		f++;
			sprintf(msg,"chooseBest: found another, frequency is now %d",f);
	    	trace(msg);
    	}
    		
   		if (getLevel(c) % 2 == 1)
   		{ 
       		if (w > v)	// positive values; new one is bigger
       		{
				sprintf(msg,"chooseBest: found a better worth for the computer of %d",v);
	    		trace(msg);
          		v=w;
          		f=1;
 				m=getCount((gameState)getData(c));
 				sprintf(msg,"chooseBest: which occurs with the removal of %d",m);
	    		trace(msg);
        	}
      	}
      	else
      	{
        	if (w < v) // negative values; new one is bigger
        	{
				sprintf(msg,"chooseBest: found a better worth for the player of %d",v);
	    		trace(msg);
          		v=w;
          		f=1;
				m=getCount((gameState)getData(c));
 				sprintf(msg,"chooseBest: which occurs with the removal of %d",m);
	    		trace(msg);
          	}
      	}
  	}
	sprintf(msg,"chooseBest: best worth is %d and a frequency of %d",v,f);
  	trace(msg);
  	
	// combine worth (v) and frequency (f) into function's result
	a=f/10.0;
	a=a+abs(v);
	if (v<0)
	{
		a=-a;
	}
	sprintf(msg,"chooseBest: chooseBest ends with %f",a);
	trace(msg);
  		
  	return a;
}


/*
	*	update
	*	Propogate the best known worth around the game tree
	*	Pre-condition: the game tree is not NULL
	*	Post-condition: the states of all nodes in the tree have
	*					up-to-date values
	*	Informally: walk over the tree spreading knowledge of the best
	*				possible outcome known so far to all ancestor nodes
	*				assuming both the computer and human will make the
	*				best move available to each when it is their turn
*/
void update(gameTree t)
{
  	double w;		// worth and frequency combination of move which is most valuable
  	int v;			// worth of move which is most valuable
	gameTree c;		// loop counter for sibling traversals
	gameState s;	// game state of parent node to be updated
	char msg[100];	// string for building up trace() messages

	trace("update: update starts");
		
  	if (! isEmptyGT(t))	// nodes exist to check
  	{
    	if (! isEmptyGT(getChild(t)))	// child nodes exist to check
    	{
      		c=getChild(t);
      		while (! isEmptyGT(c))	// move to child and update entire level
      		{
        		update(c);
        		c=getSibling(c);
      		}
      		trace("update: siblings traversed");

      		w=chooseBest(t);
			v=(int)w;
      		s=(gameState)getData(t);	
      		setWorth(s,v);	// update parent
      		setData(t,s);
			sprintf(msg,"update: worth of %d set",v);
      		trace(msg);
    	}
  	}
  		
	trace("update: update ends");
}


/*
	*	findBest
	*	Find the best move in the tree for the computer
	*	Pre-condition: moves exist in the child level of the
	*				   current tree with a worth of the int part
	*				   of the given double parameter (w)
	*	Post-condition: the tree is adjusted so that the current
	*					state is overriden by the game tree with
	*					the best next move as its root (with ties
	*					broken randomly) and all level numbers
	*					are decremented accordingly
	*	Informally: computer has a turn!
	*
	*	param double w whole number indicates worth of best move 
	*				   while fraction indicates frequency of this worth
*/
int findBest(gameTree t,double w)
{
	gameTree c;		// loop counter for sibling traversals
	gameTree e;		// empty game tree
	int m;			// number of matches remaining in the game
	int n;			// frequency of moves with worth of v
	int k;			// random move number between 1 and n
	int r;			// number of matches to be removed next turn (function result)
  	int v;			// worth of move which is most valuable
  	bool found;		// flag to stop search once move of desired worth is located
	char msg[100];	// string for building up trace() messages
	double w2,w3;	// intermediate variables for deconstructing worth and frequency
    		
	trace("findBest: findBest starts");

	// 'decode' worth and frequency for search
	v=(int)w;
	w=w-v;
	w2=10*w;
	if (w2<0)
	{
		w2=-w2;
	}
	w3=(w2+0.5);
	n=(int)w3;
	k=rand()%n;
	sprintf(msg,"findBest: looking randomly for move %d of %d with a worth of %d",k+1,n,v);
	trace(msg);
			
  	m=getCount((gameState)(getData(t)));

	// find move to be made
	c=getChild(t);
  	found=false;
    while (!found)
    {
		sprintf(msg,"findBest: Move is: %d",m-getCount((gameState)(getData(c))));
      	trace(msg);
    	if (getWorth((gameState)(getData(c))) == v)
      	{
			if (k == 0) // found it
			{
   				trace("findBest: got it");
				r=m-getCount((gameState)(getData(c)));
      			found=true;
			}
			else // right value but not this one
			{
				trace("findBest: got one, but skipping it");
				k--;
 	      		c=getSibling(c);
			}
      	}
      	else // not the right value
      	{
	      	trace("findBest: not this move, trying sibling");

 	      	c=getSibling(c);
     	}
  	}
  		
	// discard other siblings and adjust tree levels
  	trace("findBest: found it; cutting off remaining siblings and adjusting");
	init_gameTree(&e,true,NULL,-1);
    setSibling(c,e);
	t->root=c->root;
    adjustLevel(t);
    	
	sprintf(msg,"findBest: findBest ends with value of %d",r);
	trace(msg);

	return r;
}


/*
	*	findMove
	*	Find the nominated move in the tree (located as one child)
	*	Pre-condition: game tree is not NULL and int parameter value
	*				   is the number of matches left in a child node
	*	Post-condition: the tree is adjusted so that the current
	*					state is overriden by the game tree with
	*					the number of matches as specified in the
	*					parameter (m) as its root and all level
	*					numbers are decremented accordingly
	*	Informally: find the move the human wants to make!
	*
	*	param int m the number of matches to be left after the move
*/
void findMove(gameTree t,int m)
{
	gameTree c;		// loop counter for sibling traversals
	gameTree e;		// empty game tree

	trace("findMove: findMove starts");
		
	c=getChild(t);
	if (! isEmptyGT(c))
	{
		// find the move
	    while ((! isEmptyGT(c)) && (getCount((gameState)(getData(c))) != m))
	    {
	      	trace("findMove: not this move, trying sibling");
	      	c=getSibling(c);
      	}
      		
		// cleave the move from the tree
		init_gameTree(&e,true,NULL,-1);
	    if (isEmptyGT(c))	// wasn't there; shouldn't happen!
	    {
	    	trace("findMove: not there, creating empty tree as sentinel");
			t->root=e->root;
	    }
	    else	// got it
	    {
	    	trace("findMove: found it, cutting off remaining siblings and adjusting");
	      	setSibling(c,e);
			t->root=c->root;
	      	adjustLevel(t);
	    }
	}

	trace("findMove: findMove ends");
}
	

/*
	*	rootNodeToString
	*	String conversion for root node value
	*	Pre-condition: game tree variable isn't NULL
	*	Post-condition: a string variable is returned consisting of the
	*				string representation of the value within the
	*				root node, followed by " " or "<>" if the game tree
	*				variable is an empty tree
	*	Informally: produce a string representation of the tree's root
	*				node
	*
	*	return (char *) printable equivalent of root node contents
*/
char *rootNodeToString(gameTree t)
{
	char *s;	// function result

	trace("rootNodeToString: rootNodeToString starts");
	
	s=(char *)malloc(5*sizeof(char));
		
	if (isEmptyGT(t))	// empty game tree
	{
		return "<>";
	}
	else	// non-empty game tree
	{
		sprintf(s,"%d ",getCount((gameState)getData(t)));
	}

	trace("rootNodeToString: rootNodeToString ends");

	return s;
}
	
	
/*
	*	toStringGT
	*	String conversion for tree
	*	Pre-condition: game tree variable isn't NULL
	*	Post-condition: a string variable is returned consisting of the
	*				string representation of all items in the GameTree,
	*				from top to bottom in depth-first order, separated by
	*				" " and contained within "<" and ">"
	*	Informally: produce a string representation of the game tree
	*
	*	return (char *) printable contents of game tree
*/
char *toStringGT(gameTree t)
{
	gameTree c;		// loop counter for sibling traversals
	char *s;		// function result

	trace("toStringGT: toStringGT starts");
		
	if (isEmptyGT(t))	// empty game tree
	{
		trace("toStringGT: toStringGT ends");
		return "<>";
	}
	else	// non-empty game tree
	{
		s=(char *)malloc(200*5*sizeof(char));

		// add current node content to ansewer and process all others (children and then siblings)
		sprintf(s,"%s %s ",s,rootNodeToString(t));
		c=getChild(t);
		if (! isEmptyGT(c))
		{
			sprintf(s,"%s %s ",s,toStringGT(c));
		}
		c=getSibling(t);
		if (! isEmptyGT(c))
		{
			sprintf(s,"%s %s ",s,toStringGT(c));
		}
	}

	trace("toStringGT: toStringGT ends");

	return s;
}

