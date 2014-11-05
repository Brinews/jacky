#include <stdio.h>
#include <stdlib.h>

void printArray(char board[21][21], int size);
int printMove(char board[21][21], int size, int select);
int findLongest(char board[21][21], int size, int row, int col);
int winnercheck(char board[21][21], int size);
void chooseplayer(char board[21][21], int size);
void measureSquare(char board[21][21], int size);

int error_outofrange(char board[21][21], int size, int x, int y);
int error_occupied(char board[21][21], int size, int x, int y);
int error_nonvalidplace(char board[21][21], int size, int x, int y);

/* basic judement : 1 for true, 0 for false */
int error_outofrange(char board[21][21], int size, int x, int y)
{
	if (x >= size || y >= size) return 1;
	if (x < 0 || y < 0) return 1;

	return 0;
}

int error_occupied(char board[21][21], int size, int x, int y)
{
	if (board[x][y] != 'U') return 1;

	return 0;
}

int error_nonvalidplace(char board[21][21], int size, int x, int y)
{
	if (board[x][y] != 'B'
			&& board[x][y] != 'W') 
		return 1;

	return 0;
}

int main(int argc, char** argv) {

    int size, i, j, blockedrow, blockedcol ;
    char board[21][21];

    printf("Enter board dimensions (n), n>=6 and n<=21: ");
    scanf("%d", &size);

	/* 1. init */

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            board[i][j] = 'U';
        }
    }

    printArray(board, size);

	/* 2. place red blocks */

	blockedrow = blockedcol = 0;
    while (blockedrow != -1 && blockedcol != -1) {

        printf("Enter position (ROW COL) of blocked square; (-1 -1) to finish: ");
        scanf("%d%d", &blockedrow, &blockedcol);

        if (blockedrow == -1 && blockedcol == -1) {
			break;
        } 
		else 
		if (error_outofrange(board, size, blockedrow, blockedcol)) {
            printf("Out of range row or column\n");
        } 
		else 
		if (error_occupied(board, size, blockedrow, blockedcol)) {
                printf("Position already occupied, try again\n");
		}
		else
		{
			board[blockedrow][blockedcol] = 'R';
			printArray(board,size);
        }
    }
    
	/* 3. player move */

	chooseplayer(board, size);

	/* 4. measure square */

	measureSquare(board, size);

    return (EXIT_SUCCESS);
}

void chooseplayer(char board[21][21], int size) {
    int j, player, ret;

    for (j = 2;; j++) {
        player = j % 2; /* black:0, red:1 */

		ret = printMove(board, size, player);

		if (ret == 0) 
		{
			printf("\n");
			break;
		}
		else
			printArray(board, size);
    }

	winnercheck(board, size);
}

void printArray(char board[21][21], int size) {
    int i, j;
    for (i = 0; i <= size - 1; i++) {
        for (j = 0; j <= size - 1; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
}

int printMove(char board[21][21], int size, int select) {

	char *player[2] = {"Black","White"};
	char color[2] = {'B', 'W'};

    int colum, row, i, blackmoverow, blackmovecol,determine;

	while (1) {
		printf("Enter %s Move (ROW COL); (-1 -1) to finish: ",
				player[select]);
		scanf("%d%d", &blackmoverow, &blackmovecol);

		if (blackmoverow == -1 && blackmovecol == -1) return 0;
		else 
		if (error_outofrange(board, size, blackmoverow, blackmovecol))
		{
			printf("Out of range row or column\n");
			continue;
		}
		else
		if (error_occupied(board, size, blackmoverow, blackmovecol))
		{
			printf("That square is already occupied or blocked\n");
			continue;
		}
		else
		{
			board[blackmoverow][blackmovecol] = color[select];
			break;
		}
	}

	return 1;
}

int winnercheck(char board[21][21], int size) {

	int i, j;
	int num = 0;

	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
		{
			num = findLongest(board, size, i, j);

			if (num >= 6)
			{
				if (board[i][j] = 'W')
					printf("White wins\n");
				else if (board[i][j] == 'B')
					printf("Black wins\n");

				return 1;
			}
		}

	printf("No winner so far\n");
	return 0;
}

int findLongest(char board[21][21], int size, int row, int col) {
    int num1, num2, num3, num4, num5, num6, num7, num8, 
		totalNum1, totalNum2, totalNum3, totalNum4, longestNum;
    int i;

    char c = board[row][col];

	num1 = num2 = num3 = num4 = 0;
	num5 = num6 = num7 = num8 = 0;

    if (c != 'B' && c != 'W')
        return 0;
    else {
        for (i = 1;; i++) {	/* left */
            if (row-i >= 0 
					&& board[row][col] == board[row - i][col])
                num1++;
            else
                break;
        }

        for (i = 1;; i++) { /* left & down */
            if (row-i >= 0 && col+i < size &&
					board[row][col] == board[row - i][col + i])
                num2++;
            else
                break;
        }

        for (i = 1;; i++) { /* down */
            if (col+i < size && 
					board[row][col] == board[row][col + i])
                num3++;
            else
                break;
        }

        for (i = 1;; i++) { /* right & down */
            if (row+i < size && col+i < size &&
					board[row][col] == board[row + i][col + i])
                num4++;
            else
                break;
        }
        for (i = 1;; i++) { /* right */
            if (row+i < size &&
					board[row][col] == board[row + i][col])
                num5++;
            else
                break;
        }

        for (i = 1;; i++) { /* right & up */
            if (row+i<size && col-i>=0 &&
					board[row][col] == board[row + i][col - i])
                num6++;
            else
                break;
        }
        for (i = 1;; i++) { /* up */
            if (col-i>= 0 &&
					board[row][col] == board[row][col - i])
                num7++;
            else
                break;
        }
        for (i = 1;; i++) { /* left & up */
            if (row-i>=0 && col-i>=0 &&
					board[row][col] == board[row - i][col - i])
                num8++;
            else
                break;
        }

        totalNum1 = num1 + num5;
        totalNum2 = num2 + num6;
        totalNum3 = num3 + num7;
        totalNum4 = num4 + num8;

        longestNum = totalNum1;

        if (longestNum < totalNum2)
            longestNum = totalNum2;


        if (longestNum < totalNum3)
            longestNum = totalNum3;

        if (longestNum < totalNum4)
            longestNum = totalNum4;

        return longestNum + 1; /* add the center one */
    }
}

void measureSquare(char board[21][21], int size)
{
	int row, col;

	while (1) {
		printf("Enter position (ROW COL) of square to measure; (-1 -1) to finish: ");
		scanf("%d%d", &row, &col);

		if (row == -1 && col == -1)
		{
			printf("Goodbye\n");
			return;
		}
		else 
		if (error_outofrange(board, size, row, col))
		{
			printf("Out of range row or column\n");
			continue;
		}
		else 
		if (error_nonvalidplace(board, size, row, col))
		{
			printf("That square is neither white nor black\n");
			continue;
		}
		else
			printf("Longest Run of %c at position (%d,%d) is %d\n", 
					board[row][col], row, col, findLongest(board, size, row, col));
	}
}
