/*
 * GameState
 * Specification for the GameState ADT
 * Author Julian Dermoudy
 * Version September 2015
 *
 * This file is complete.
*/

#include <stdbool.h>

struct gameState_int;
typedef struct gameState_int *gameState;

void init_gameState(gameState *sp,int c,int w);
void setCount(gameState s,int c);
void setWorth(gameState s,int w);
int getCount(gameState s);
int getWorth(gameState s);
int evaluateState(gameState s,bool p);
bool gameOver(gameState s);
