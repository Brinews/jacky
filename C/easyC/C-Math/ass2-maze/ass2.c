/* 
 * Solution to COMP20005 Assignment 2, May 2016.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define MAX_ROWS 100		/* maximum number of maze row */
#define MAX_COLS 100		/* maximum number of maze col */
#define DIR_NUM	 4			/* total directions */
#define NO_COST	 -1			/* no cost */

#define PASS	'.' 		/* passable cell */
#define WALL	'#'			/* wall cell */
#define SPACE	' '

#define REACHABLE	'+'		/* cell can be reachable */
#define NONREACH	'-'		/* cell not reachable */

#define VISITED		1
#define NONVISITED 	0

#define PRINTNUM	1
#define PRINTCHAR	0

#define HDR1	"Stage 1"
#define HDR2	"Stage 2"
#define HDR3	"Stage 3"
#define HDR4	"Stage 4"
#define LINES	"======="
#define NEWLINE	"\n"

typedef struct maze_cell* cell_t;
typedef struct maze_struct* maze_t;
typedef struct maze_copy* copy_t;

/*
 * maze cell info
 */
struct maze_cell {
	char type; 		/* '.': no wall, '#' : wall */
	char doubled; 	/* '+':reachable, '-':non-reachable */
	char path;		/* '.', ' ', or 'N' */
	int cost;		/* reachable cost value */
	int visited;	/* 1 for visited */
	int print_flag;	/* 1 for print number */
	int from_which_dir;	/* from which direction to the reachable cell */
};

/*
 * maze info
 */
struct maze_struct {
	int maze_row;
	int maze_col;
	struct maze_cell cells[MAX_ROWS][MAX_COLS];
};

/*
 * used for find path
 */
struct maze_copy {
	int m_row;
	int m_col;
	struct maze_cell cells[MAX_ROWS][MAX_COLS/2];
};

/* function prototypes */
void read_maze(maze_t root);				/* read maze from stdin */
void print_stage1_maze(maze_t root);		/* print out maze */
void print_stage2_maze(maze_t root);		/* print out maze */
void find_reachable_cells(maze_t root);		/* mark all reachable cell */
/* judge the cell around(x,y) is reachable or not */
int  around_cell_reachable(maze_t root, int x, int y);
int  maze_has_solution(maze_t root);		/* judge the maze solution */
void update_maze_cost(maze_t root, copy_t copy);	/* update maze cost cycle */
void maze_cycle_cost(copy_t root, int x, int y);
void print_stage3_maze(maze_t root, copy_t copy);	/* print out maze */

/* function definition */
void read_maze(maze_t root)
{
	int i;
	char buf[MAX_COLS];

	root->maze_row = 0;
	root->maze_col = 0;

	/* read all maze line */
	while (scanf("%s", buf) > 0) {

		/* one maze row */
		for (i = 0; i < strlen(buf); i++) {
			root->cells[root->maze_row][i].type = buf[i];
			root->cells[root->maze_row][i].doubled = NONREACH;
			root->cells[root->maze_row][i].cost = NO_COST;
			root->cells[root->maze_row][i].path = NONREACH;
		}

		root->maze_col = strlen(buf);
		root->maze_row ++; /* accmulate row number */
	}
}

void print_stage1_maze(maze_t root)
{
	int i, j;

	printf(HDR1); 	/* print format header */
	printf(NEWLINE);

	printf(LINES);
	printf(NEWLINE);

	printf("maze has %d rows and %d columns", 
			root->maze_row, root->maze_col);
	printf(NEWLINE);

	for (i = 0; i < root->maze_row; i++) {
		for (j = 0; j < root->maze_col; j++) {
			printf("%c", root->cells[i][j].type);
		}
		printf(NEWLINE);
	}
}

int around_cell_reachable(maze_t root, int x, int y)
{
	/* directions */
	int dir_x[] = {-1, 1, 0, 0};
	int dir_y[] = {0, 0, -1, 1};

	int i;
	int pos_x, pos_y;

	for (i = 0; i < DIR_NUM; i++) {
		pos_x = x + dir_x[i];
		pos_y = y + dir_y[i];

		if (pos_x < 0 || pos_x >= root->maze_row
			|| pos_y < 0 || pos_y >= root->maze_col)
			continue;

		if (root->cells[pos_x][pos_y].type == PASS
				&& root->cells[pos_x][pos_y].doubled == REACHABLE)
			return 1;
	}

	return 0;
}

void find_reachable_cells(maze_t root)
{
	int i,j;

	/* the entry row should all be reachable */
	i = 0;
	for (j = 0; j < root->maze_col; j++) {
		if (root->cells[i][j].type == PASS) {
			root->cells[i][j].doubled = REACHABLE;
		}
	}

	/* mark the left cells line by line */
	for (i = 1; i < root->maze_row; i++) {
		for (j = 0; j < root->maze_col; j++) {
			if (root->cells[i][j].type == PASS) {
				if (around_cell_reachable(root, i, j)) {
					root->cells[i][j].doubled = REACHABLE;
				} else
					root->cells[i][j].doubled = NONREACH;
			}
		}

		for (j = root->maze_col-1; j >= 0; j--) {
			if (root->cells[i][j].type == PASS) {
				if (around_cell_reachable(root, i, j)) {
					root->cells[i][j].doubled = REACHABLE;
				} else
					root->cells[i][j].doubled = NONREACH;
			}
		}
	}
}

/*
 * if the exit cell is reachable then has solution
 */
int maze_has_solution(maze_t root)
{
	int i, j;
	i = root->maze_row-1;

	for (j = 0; j < root->maze_col; j++) {
		if (root->cells[i][j].type == PASS
				&& root->cells[i][j].doubled == REACHABLE)
			return 1;
	}

	return 0;
}

/*
 * print doubled-marked maze
 */
void print_stage2_maze(maze_t root)
{
	int i, j;

	printf(HDR2); 	/* print format header */
	printf(NEWLINE);

	printf(LINES);
	printf(NEWLINE);

	if (maze_has_solution(root))
		printf("maze has a solution");
	else 
		printf("maze does not have a solution");

	printf(NEWLINE);

	for (i = 0; i < root->maze_row; i++) {
		for (j = 0; j < root->maze_col; j++) {
			if (root->cells[i][j].type == WALL)
				printf("%c", root->cells[i][j].type);
			else {
				printf("%c", root->cells[i][j].doubled);
			}
		}
		printf(NEWLINE);
	}
}

/*
 * start position: (x, y)
 * search around util all the reachable cells
 */
void maze_cycle_cost(copy_t root, int x, int y)
{
	/* directions */
	int dir_x[] = {-1, 1, 0, 0};
	int dir_y[] = {0, 0, -1, 1};

	int queue_x[MAX_ROWS]; /* queue structure */
	int queue_y[MAX_COLS];

	int q_head = 0, q_tail = 0;
	int cost = 0;
	int px, py; /* position */
	int i;

	/* enqueue */
	queue_x[q_tail] = x;
	queue_y[q_tail] = y;
	q_tail++;

	while (q_head < q_tail) {
		/* dequeue */
		px = queue_x[q_head];
		py = queue_y[q_head];

		root->cells[px][py].visited = VISITED;
		cost = root->cells[px][py].cost;

		for (i = 0; i < DIR_NUM; i++) {
			x = px + dir_x[i];
			y = py + dir_y[i];

			if (x < 0 || x >= root->m_row
					|| y < 0 || y >= root->m_col) {
				continue; /* out of border */
			}

			if (root->cells[x][y].type == PASS
					&& root->cells[x][y].doubled == REACHABLE
					&& root->cells[x][y].visited == NONVISITED) {
				/* enqueue to visit */
				queue_x[q_tail] = x;
				queue_y[q_tail] = y;
				q_tail++;

				/* update cell cost */
				if (root->cells[x][y].cost == NO_COST
						|| root->cells[x][y].cost > cost+1) {
					/* record the path */
					root->cells[x][y].from_which_dir = i;
					root->cells[x][y].cost = cost+1;
				}
			}
		}

		q_head++;
	}
}

void update_maze_cost(maze_t root, copy_t copy)
{
	int i, j;
	int m, n;

	copy->m_row = root->maze_row;
	copy->m_col = root->maze_col/2;

	for (i = 0; i < copy->m_row; i++) {
		for (j = 0; j < copy->m_col; j++) {
			copy->cells[i][j] = root->cells[i][j*2];
		}
	}

	/* init first row's cost */
	i = 0;
	for (j = 0; j < copy->m_col; j++) {
		if (copy->cells[i][j].type == PASS) {
			copy->cells[i][j].cost = 0;
			copy->cells[i][j].visited = VISITED;
		}
	}

	i = 0;
	for (j = 0; j < copy->m_col; j++) {
		if (copy->cells[i][j].type == PASS) {
			/* clear the visited flag */
			for (m = 1; m < copy->m_row; m++) {
				for (n = 0; n < copy->m_col; n++) {
					if (copy->cells[m][n].type == PASS)
						copy->cells[m][n].visited = NONVISITED;
				}
			}

			/* update the cost from (0, j) */
			maze_cycle_cost(copy, i, j);
		}
	}
}

void print_stage3_maze(maze_t root, copy_t copy)
{
	int i, j;
	int cost = MAX_ROWS*MAX_COLS;
	int toprint_cost;

	printf(HDR3); 	/* print format header */
	printf(NEWLINE);

	printf(LINES);
	printf(NEWLINE);

	i = copy->m_row-1;
	for (j = 0; j < copy->m_col; j++) {
		if (copy->cells[i][j].type == PASS
				&& copy->cells[i][j].doubled == REACHABLE
				&& cost > copy->cells[i][j].cost) {
			cost = copy->cells[i][j].cost;
		}
	}

	if (maze_has_solution(root))
		printf("maze has a solution with cost %d", cost);
	else 
		printf("maze does not have a solution");

	printf(NEWLINE);

	for (i = 0; i < copy->m_row; i++) {
		for (j = 0; j < copy->m_col; j++) {
			if (copy->cells[i][j].type == WALL)
				printf("%c%c", copy->cells[i][j].type,
						copy->cells[i][j].type);
			else {
				if (copy->cells[i][j].doubled == NONREACH)
					printf("%c%c", copy->cells[i][j].doubled,
							copy->cells[i][j].doubled);
				else {
					if (copy->cells[i][j].cost%2 == 0) {
						toprint_cost = copy->cells[i][j].cost%100;
						printf("%02d", toprint_cost);
					} else {
						printf("%c%c", copy->cells[i][j].doubled,
								copy->cells[i][j].doubled);
					}
				}
			}
		}
		printf(NEWLINE);
	}
}

void update_maze_path(maze_t root, copy_t copy)
{
	int i, j;
	int cost = MAX_ROWS*MAX_COLS;
	int px, py; /* exit position */
	int toprint_cost;

	int dir;
	int dir_x[] = {1,-1,0,0};
	int dir_y[] = {0,0,1,-1};

	i = copy->m_row-1;
	px = i;
	for (j = 0; j < copy->m_col; j++) {
		if (copy->cells[i][j].type == PASS
				&& copy->cells[i][j].doubled == REACHABLE
				&& cost > copy->cells[i][j].cost) {
			cost = copy->cells[i][j].cost;
			py = j;
		}
	}

	/* back-track */
	while (px != 0) {
		toprint_cost = copy->cells[px][py].cost%100;
		if (toprint_cost % 2 == 0) {
			root->cells[px][2*py].path = toprint_cost/10+'0';
			root->cells[px][2*py+1].path = toprint_cost%10+'0';
		} else {
			root->cells[px][2*py].path = '.';
			root->cells[px][2*py+1].path = '.';
		}

		dir = copy->cells[px][py].from_which_dir;

		px = px + dir_x[dir];
		py = py + dir_y[dir];
	}

	toprint_cost = copy->cells[px][py].cost%100;
	root->cells[px][2*py].path = toprint_cost/10+'0';
	root->cells[px][2*py+1].path = toprint_cost%10+'0';
}

/*
 * print path-marked maze
 */
void print_stage4_maze(maze_t root)
{
	int i, j;

	printf(HDR4); 	/* print format header */
	printf(NEWLINE);

	printf(LINES);
	printf(NEWLINE);

	printf("maze solution");
	printf(NEWLINE);

	for (i = 0; i < root->maze_row; i++) {
		for (j = 0; j < root->maze_col; j++) {
			if (root->cells[i][j].type == WALL)
				printf("%c", root->cells[i][j].type);
			else {
				if (root->cells[i][j].doubled == NONREACH)
					printf("%c", root->cells[i][j].doubled);
				else if (root->cells[i][j].path != NONREACH) {
					printf("%c", root->cells[i][j].path);
				} else
					printf("%c", SPACE);
			}
		}
		printf(NEWLINE);
	}
}

int main(int argc, char **argv)
{
	struct maze_struct my_maze;
	struct maze_copy m_copy;

	read_maze(&my_maze);
	print_stage1_maze(&my_maze);

	find_reachable_cells(&my_maze);
	print_stage2_maze(&my_maze);

	update_maze_cost(&my_maze, &m_copy);
	print_stage3_maze(&my_maze, &m_copy);

	if (maze_has_solution(&my_maze)) {
		update_maze_path(&my_maze, &m_copy);
		print_stage4_maze(&my_maze);
	}

	return 0;
}

