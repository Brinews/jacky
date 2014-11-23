#include <stdio.h>
#include <stdlib.h>

void printArray(char board[21][21], int size);
int playerMove(char board[21][21], int size, int select);
int selectMove(char board[21][21], int size, int select);
int findLongest(char board[21][21], int size, int row, int col);
int winnercheck(char board[21][21], int size);
int drawcheck(char board[21][21], int size);
void chooseplayer(char board[21][21], int size, int isComputerBlack);

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

	char selectColor[10];
	printf("Computer playing B or W?: ");
	scanf("%s", selectColor);


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
    
	/* 3. player vs computer */
	chooseplayer(board, size, selectColor[0] == 'B');

    return (EXIT_SUCCESS);
}

void chooseplayer(char board[21][21], int size, int isComputerBlack) {
    int ret;

    for (;;) {
		if (drawcheck(board, size)) break;
		if (winnercheck(board, size)) break;

        /* black:0, red:1 */
		if (isComputerBlack) {
			selectMove(board, size, 0);
			printArray(board, size);

			if (drawcheck(board, size)) break;
			if (winnercheck(board, size)) break;

			ret = playerMove(board, size, 1);
			if (ret == 0) {printf("\n"); break;}
			printArray(board, size);

		} else {
			ret = playerMove(board, size, 0);
			if (ret == 0) {printf("\n"); break;}
			printArray(board, size);

			if (drawcheck(board, size)) break;
			if (winnercheck(board, size)) break;

			selectMove(board, size, 1);
			printArray(board, size);
		}
    }
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

int selectMove(char board[21][21], int size, int wb)
{
	char color[2] = {'B', 'W'};
	int i, j;
	int score = -1, maxScore = 0;
	int row, col;

	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			/* undef position */
			if (board[i][j] == 'U') {
				/* calculate when 'B' */
				board[i][j] = 'B';
				score = findLongest(board, size, i, j);
				/* calculate when 'W' */
				board[i][j] = 'W';
				score += findLongest(board, size, i, j);

				if (score > maxScore) {
					maxScore = score;
					row = i;
					col = j;
				}

				board[i][j] = 'U';
			}
		}
	}

	/* pos on (row, col) */
	board[row][col] = color[wb];
	printf("Computer moves %c at %d %d\n", color[wb], row, col);

	return 0;
}

int playerMove(char board[21][21], int size, int select) {

	char *player[2] = {"Black","White"};
	char color[2] = {'B', 'W'};

    int colum, row, i, blackmoverow, blackmovecol,determine;

	while (1) {
		printf("Enter %s Move (ROW COL): ",
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

int drawcheck(char board[21][21], int size)
{
	int undef = 0;
	int i, j;

	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			if (board[i][j] == 'U')
				undef ++;
		}
	}

	if (undef == 0) {
		printf("\nDraw!\n");
		return 1;
	}

	return 0;
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
					printf("\nWhite player wins.\n");
				else if (board[i][j] == 'B')
					printf("\nBlack player wins.\n");

				return 1;
			}
		}

	//printf("No winner so far\n");
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
