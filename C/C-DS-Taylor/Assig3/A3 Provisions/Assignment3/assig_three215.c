/*
	*	Harness File
	*	Author Julian Dermoudy
	*	Version September 2015
	*	
	*	This file drives the solution.  
	*
	*	The file is complete.
*/

//#include <stdbool.h>
#include <stdio.h>
#include "assig_three215.h"
#include "gameTree.h"
#include "gameState.h"
#include "stack.h"

/*
	*	trace
	*	Provide trace output.
	*	Pre-condition: none
	*	Post-condition: if trace output is desired then the given string
	*					parameter is shown on the console
	*	Informally: show the given message for tracing purposes
	*
	*	param (char *) s the string to be displayed as the trace message
*/
void trace(char *s)
{
	if (TRACING)
	{
		printf("%s\n",s);
	}
}


/*
	*	intro
	*	Provide informtation about the game.
	*	Pre-condition: none
	*	Post-condition: introductory information has been displayed
	*	Informally: introduce the game
*/
void intro()
{
	trace("intro: intro starts");

	printf("The Game of Nim\n");
	printf("===============\n");
	printf("Welcome to the game of Nim.  There are %d matches and just you and me.\n",MAX_MATCHES);
	printf("We take it in turns choosing to remove 1-%d matches.  You lose if you\n",MAX_MOVES);
	printf("take the last one, you win if I do.\n\n");

	trace("intro: intro starts");
}


/*
	*	user_move
	*	Get the human user's next move
	*	Pre-condition: m is a positive number
	*	Post-condition: the intended number of matches to remove
	*					has been returned
	*	Informally: deermine the user's move
	*
	*	param int s the number of matches remaining in the game
*/int user_move(int m)
{
	int r;	// user's choice (return value)
	int n;	// maximum number of matches available this turn

	trace("user_move: user_move starts");

	n=(m<MAX_MOVES)?m:MAX_MOVES;	// how many matches can be removed
	printf("How many would you like to take (1-%d)? ",n);
	scanf("%d",&r);
	while ((r<1) || (r>n))	// too few or too many
	{
		trace("user_move: too few or too many chosen");
		printf("Between 1 and %d please!\n\n",n);
		printf("How many would you like to take (1-%d)? ",n);
		scanf("%d",&r);
	}

	trace("user_move: user_move ends");

	return r;
}


/*
	*	main
	*	Program entry point
	*	Pre-condition: none
	*	Post-condition: the game has been played
	*	Informally: play the game
	*
	*	return int 0 if the game plays properly, 1 if an error is encountered
*/
int main(int argc, char *argv[])
{
	gameTree g;		// gametree of current game
	gameState s;	// leaf node gametree for starting the game
	stack k;		// stack of moves known about but unexpanded
	char r[100];	// raw user input
	int c;			// count of matches available at start of turn
	int n;			// computer's choice of number of matches to take
	double v;		// whole number indicates worth of move while fraction indicates frequency of this worth
	bool w;			// has the computer player won?
	char msg[100];	// string for building up trace() messages

	trace("main: main starts");

	// tell user about the program
	intro();

	// initialise stack of unexplored moves
	init_stack(&k);

	// found out who's going first and make user's move if they are
	printf("Would you like to go first (Y/N)? ");
	scanf("%s",r);

	if ((r[0]=='Y')||(r[0]=='y'))
	{
		trace("main: user is going first");
		n=user_move(MAX_MATCHES);
		init_gameState(&s,MAX_MATCHES-n,0);
	}
	else
	{
		trace("main: computer is going first");
		init_gameState(&s,MAX_MATCHES,0);
	}
	init_gameTree(&g,false,s,0);

	// build initial game tree to depth of LOOK_AHEAD
	sprintf(msg,"main: There are %d matches",getCount(s));
	trace(msg);
	buildGameDF(g,k,LOOK_AHEAD);

	// play the game until it's over
	do
	{
		// first the computer...
		w=true;
		c=getCount((gameState)(getData(g)));
		printf("There are %d matches remaining.\n",c);

		v=chooseBest(g);
		sprintf(msg,"I choose to take a move worth %f.\n",v);
		trace(msg);

		n=findBest(g,v);
		printf("I choose to take %d matches.\n",n);

		buildGameDF(g,k,LOOK_AHEAD);
		c=getCount((gameState)(getData(g)));
		printf("There are now %d matches remaining.\n\n",c);

		// then the user...
		if (! gameOver((gameState)(getData(g))))
		{
			w=false;
			n=user_move(c);
			findMove(g,c-n);
			buildGameDF(g,k,LOOK_AHEAD);
		}

		// one move gone, remove it from the game tree and repeat until game is over
		update(g);
	} while (! gameOver((gameState)(getData(g))));

	trace("main: game is over");

	// tell us who won
	if (w)
	{
		printf("Congratulations!  You won.\n\n");
	}
	else
	{
		printf("Commiserations! Loser.\n\n");
	}
}
