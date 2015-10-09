/*
 *	GameState ADT
 *	author <<Your names here>>
 *	version September 2015
 *	
 *	This file holds the GameState ADT which is the
 *	data stored in game tree nodes.  The GameState
 *	variables consist of two fields: count and
 *	worth.  Count refers to the number of matches
 *	left in the game at that point while worth
 *	refers to how optimistic the computer-player
 *	is that it is going to win (with values
 *	ranging from certain loss (-9) to certain
 *	success (+9)).
 *
 * YOU NEED TO COMPLETE THIS FILE. 
*/


//#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "assig_three215.h"
#include "gameState.h"


// internals of the state of the game
struct gameState_int {
	int count;	// the number of matches still to be removed
	int worth;	// how optimistic the position looks for the computer
};


/*
	*	GameState
	*	Initialiser function
	*	Pre-condition: none
	*	Post-condition: the gameState variable holds the parameter value
	*					(c) within its "count" field, and the parameter
	*					value (w) within its "worth" field
	*	Informally: intialises the fields of the newly created gameState
	*				variable to hold the given parameters
	*
	*	param (int) c the count of matches to store in the count field of the variable
	*	param (int) w the worth of the move to store in the worth field of the variable
*/
void init_gameState(gameState *sp,int c,int w)
{
	trace("GameState: initialiser starts");
	
	(*sp) = (gameState) malloc(sizeof(struct gameState_int));
	(*sp)->count = c;
	(*sp)->worth = w;
		
	trace("GameState: initialiser ends");
}


/*
	*	getCount
	*	Get function for "count" field.
	*	Pre-condition: s is not NULL
	*	Post-condition: the gameState variable's count field is
	*				returned
	*	Informally: examine the gameState variable's "count"
	*				field returning its value
	*
	*	return int the match count stored in the gameState variable
*/
int getCount(gameState s)
{
	trace("getCount: getCount starts and finishes");

	return s->count;
}


/*
	*	getWorth
	*	Get function for "worth" field.
	*	Pre-condition: s is not NULL
	*	Post-condition: the gameState variable's worth field is
	*				returned
	*	Informally: examine the gameState variable's "worth"
	*				field returning its value
	*
	*	return int the worth stored in the gameState variable
*/
int getWorth(gameState s)
{
	trace("getWorth: getWorth starts and finishes");

	return s->worth;
}


/*
	*	setCount
	*	Set function for a gameState variable's count field.
	*	Pre-condition: gameState variable isn't NULL
	*	Post-condition: the gameState variable's count field is
	*					altered to hold the given (c) value
	*	Informally: store the given value in the count field of
	*				the gameState variable
	*
	*	param int c value to install in the variable
*/
void setCount(gameState s,int c)
{
	trace("setCount: setCount starts");
		
	s->count = c;
		
	trace("setCount: setCount ends");
}


/*
	*	setWorth
	*	Set function for a gameState variable's worth field.
	*	Pre-condition: gameState variable isn't NULL
	*	Post-condition: the gameState variable's worth field is
	*					altered to hold the given (w) value
	*	Informally: store the given value in the worth field of
	*				the gameState variable
	*
	*	param int w value to install in the variable
*/
void setWorth(gameState s,int w)
{
	trace("setCount: setWorth starts");
		
	s->worth = w;
		
	trace("setCount: setWorth ends");
}


/*
	*	evaluateState
	*	Evaluate the worth of a gameState value
	*	Pre-condition: gameState variable isn't NULL and p is
	*				   true if the current player is the
	*				   computer player and false otherwise
	*	Post-condition: the gameState variable is evaluated
	*					and the calculated worth is returned
	*	Informally: determine whether the game is going well
	*				or poorly for the computer player
	*
	*	param bool p whether or not the current player is the
	*				 the computer player
*/
int evaluateState(gameState s,bool p)
{
	int r;	// function result

	trace("evaluateState: evaluateState starts");

	if (s->count == 0)	// game is over
	{
		r=9;
	}
	else
	{
		if (s->count == 1)	// game will be lost on next turn
		{
			r=-5;
		}
		else
		{
			if (s->count <= MAX_MOVES+1)	// game could be claimed on next turn
			{
				r=-3;
			}
			else	// too early to tell what the outcome could be
			{
				r=0;
			}
		}
	}

	if (!p)	// not the computer's turn so invert the scores
	{
		r=-r;
	}

	trace("evaluateState: evaluateState ends");

	return r;
}


/*
	*	gameOver
	*	Determine whether the game is over or not
	*	Pre-condition: gameState variable isn't NULL
	*	Post-condition: the gameState variable is evaluated
	*					and true has been returned if the
	*					game was over; false was returned
	*					otherwise
	*	Informally: determine whether the game is over or not
*/
bool gameOver(gameState s)
{
	trace("gameOver: gameOver starts and ends");

	return s->count == 0;
}