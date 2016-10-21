#include <time.h>
#include <stdlib.h>
#include "ai.h"
#include "utils.h"
#include "priority_queue.h"

struct heap h;

void initialize_ai(){
	heap_init(&h);
}

/**
 * array equals or not
 */
static int not_equal(uint8_t src[SIZE][SIZE], uint8_t dst[SIZE][SIZE])
{
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (dst[i][j] != src[i][j]) return 1;
        }
    }
    return 0;
}


/*
 * copy board by cells
 */
static void copy_board(uint8_t dst[SIZE][SIZE], uint8_t src[SIZE][SIZE])
{
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            dst[i][j] = src[i][j];
        }
    }
}

/*
 * move by action
 * change state of node
 */
static node_t* apply_action(node_t *node, move_t action)
{
    node_t *newnode = (node_t *) malloc(sizeof(node_t));
    uint32_t score = 0;

    copy_board(newnode->board, node->board);

    /* make move */
    execute_move_t(newnode->board, &score, action);

    /* update node */
    newnode->priority = node->priority + score; /* priority should by score? */
    newnode->depth = node->depth+1;
    newnode->move = action;
    newnode->parent = node;

    return newnode;
}

static void propagate_back_score(int firstActionScore[4], node_t *newnode, propagation_t mode)
{
    move_t move;
    int score = newnode->priority;
	int nodeNum = 1;

    while (newnode->parent != NULL) {
        move = newnode->move;
        newnode = newnode->parent;
		nodeNum++;
    }

	/* average mode */
	if (mode == avg) {
		score /= nodeNum;
	}

	/* update four direction's score */
    if ( firstActionScore[move] < score) {
        firstActionScore[move] = score;
    }
}

void free_memory(node_t **node_array, int size)
{
    int i;
    for (i = 0; i < size; i++) {
        free(node_array[i]);
    }

    free(node_array);
}

/**
 * Find best action by building all possible paths up to depth max_depth
 * and back propagate using either max or avg
 */

move_t get_next_move( uint8_t board[SIZE][SIZE], int max_depth, propagation_t propagation ){
	move_t best_action = rand() % 4;

    int i;
    int dir;
    int best_score = 0;// second_best_score = 0, second_best_action = 0;
    move_t fourdirs[4] = {left, right, up, down};
    int firstActionScore[4] = {0, 0, 0, 0};

    /* explored node array */
    int arraySize = 4;
    int arrayIndex = 0;
    node_t **explored = (node_t **) malloc(sizeof(node_t*) * arraySize);

	//FILL IN THE GRAPH ALGORITHM
    node_t *start = (node_t *) malloc(sizeof(node_t));
    start->priority = 0;
    start->depth = 0;
    start->num_childs = 4;
    start->move = 0;
    copy_board(start->board, board);
    start->parent = NULL;

    heap_push(&h, start);

    while (h.count > 0) {
        node_t *node = heap_delete(&h);
        expandNum++;

        if (arrayIndex >= arraySize) {
            arraySize *= 2;
            explored = realloc(explored, sizeof(node_t*)*arraySize);
        }
        /* add to explored array */
        explored[arrayIndex] = node;
        arrayIndex++;

        if (node->depth < max_depth) {

            for (dir = 0; dir < 4; dir++) {
                move_t nextmove = fourdirs[dir];
                node_t *newnode = apply_action(node, nextmove);
                generateNum++;

                if (not_equal(newnode->board, node->board)) {
					addRandom(newnode->board);
                    heap_push(&h, newnode);
					if (newnode->depth == max_depth) {
						propagate_back_score(firstActionScore, newnode, propagation);
					}
                } else {
                    free(newnode);
                }
            }
        }
    }

    free_memory(explored, arrayIndex);

	best_score = firstActionScore[0];
    for (i = 0; i < 4; i++) {
        if (firstActionScore[i] > best_score
                || (firstActionScore[i] == best_score && rand()%2 == 0)) {
            best_action = i;
            best_score = firstActionScore[i];
        }
    }

	/*
    if (propagation == avg) {
        second_best_score = 0;
        for (i = 0; i < 4; i++) {
            if (i != best_action 
                    && firstActionScore[i] > second_best_score) {
                second_best_action = i;
                second_best_score = firstActionScore[i];
            }
        }
        best_action = second_best_action;
    }
	*/
	
	return best_action;
}
