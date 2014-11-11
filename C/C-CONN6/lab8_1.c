/* 
 * File:   lab8.c
 * Author: mac
 *
 * Created on November 8, 2014, 6:30 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/*
 * 
 */
void printBoard(char board[21][21], int n) {//print the board
    int i;
    int j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++)
            printf("%c", board[i][j]);
        printf("\n");
    }
}

void blockedSquare(char board[21][21], int n) {//print the blocked square
    int redRow;
    int redCol;

    do {
        printf("Enter position (ROW COL) of blocked square; (-1 -1) to finish: ");
        scanf("%d%d", &redRow, &redCol);
        if (redRow != -1 && redCol != -1) {
            if (redRow >= n || redCol >= n 
					|| redRow < 0 || redCol < 0)
                printf("Out of range row or column\n");
            else {
                if (board[redRow][redCol] != 'U')
                    printf("Position already occupied, try again\n");
                else {
                    board[redRow][redCol] = 'R';
                    printBoard(board, n);
                }
            }
        }
    } while (redRow != -1 && redCol != -1);
}

int checkHorizontal(char board[21][21], int n, int row, int col) {
    int i;
    int horizontal = 1;

    bool end1 = false;
    bool end2 = false;
    bool hor1 = false;
    bool hor2 = false;
    for (i = 1; !end1 || !end2; i++) {//check horizontal line
        if (col+i < n && board[row][col] == board[row][col + i] && !hor2)//check from left to right
            horizontal++;
        else {
            end2 = true;
            hor2 = true;
        }
        if (col-i >= 0 && board[row][col] == board[row][col - i] && !hor1)//check from right to left
            horizontal++;
        else {
            end1 = true;
            hor1 = true;
        }
    }

    return horizontal;
}

int checkVertical(char board[21][21], int n, int row, int col) {
    int j;
    int vertical = 1;

    bool end1 = false;
    bool end2 = false;
    bool ver1 = false;
    bool ver2 = false;
    for (j = 1; !end1 || !end2; j++) {//check vertical line
        if (row+j < n && board[row][col] == board[row + j][col] && !ver2)//check from upper to lower
            vertical++;
        else {
            end2 = true;
            ver2 = true;
        }

        if (row-j>=0 && board[row][col] == board[row - j][col] && !ver1)//check from lower to upper
            vertical++;
        else {
            end1 = true;
            ver1 = true;
        }
    }
    return vertical;
}

int checkLeftdiagonal(char board[21][21], int n, int row, int col) {
    int k;
    int leftDiagonal = 1;

    bool end1 = false;
    bool end2 = false;
    bool leftDig1 = false;
    bool leftDig2 = false;
    for (k = 1; !end1 || !end2; k++) {//check left diagonal
        if (row+k<n && col+k<n &&
				board[row][col] == board[row + k][col + k] && !leftDig2)//check from upper left to lower right
            leftDiagonal++;
        else {
            end2 = true;
            leftDig2 = true;
        }

        if (row-k>=0 && col-k>=0
				&& board[row][col] == board[row - k][col - k] && !leftDig1)//check from lower right to upper left
            leftDiagonal++;
        else {
            end1 = true;
            leftDig1 = true;
        }
    }

    return leftDiagonal;
}

int checkRightdiagonal(char board[21][21], int n, int row, int col) {
    int m;
    int rightDiagonal = 1;

    bool end1 = false;
    bool end2 = false;
    bool rightDig1 = false;
    bool rightDig2 = false;
    for (m = 1; !end1 || !end2; m++) {//check right diagonal
        if (row+m<n && col-m>=0 &&
				board[row][col] == board[row + m][col - m] && !rightDig2)//check from upper right to lower left
            rightDiagonal++;
        else {
            end2 = true;
            rightDig2 = true;
        }

        if (row-m>=0 && col+m<n &&
				board[row][col] == board[row - m][col + m] && !rightDig1)//check from lower left to upper right
            rightDiagonal++;
        else {
            end1 = true;
            rightDig1 = true;
        }
    }
    return rightDiagonal;
}

int findLongest(char board[21][21], int n, int row, int col) {
    int horizontal;
    int vertical;
    int leftDiagonal;
    int rightDiagonal;
    int Longest;

    horizontal = checkHorizontal(board, n, row, col);
    vertical = checkVertical(board, n, row, col);
    leftDiagonal = checkLeftdiagonal(board, n, row, col);
    rightDiagonal = checkRightdiagonal(board, n, row, col);

    Longest = fmax(fmax(horizontal, vertical), fmax(leftDiagonal, rightDiagonal));

    return Longest;
}

bool blackMove(char board[21][21], int n) {//print the steps of black
    int blackRow;
    int blackCol;

    printf("Enter Black Move (ROW COL): ");
    scanf("%d%d", &blackRow, &blackCol);

    if (blackRow >= n || blackCol >= n
			|| blackRow < 0 || blackCol < 0) {
        printf("Out of range row or column\n");
        return blackMove(board, n);
    } else {
        if (board[blackRow][blackCol] != 'U') {
            printf("That square is already occupied or blocked\n");
            return blackMove(board, n);
        } else {
            board[blackRow][blackCol] = 'B';
            printBoard(board, n);

            return true;
        }
    }
}

bool whiteMove(char board[21][21], int n) {//print steps of white
    int whiteRow;
    int whiteCol;

    printf("Enter White Move (ROW COL): ");
    scanf("%d%d", &whiteRow, &whiteCol);


    if (whiteRow >= n || whiteCol >= n
			|| whiteRow < 0 || whiteCol < 0) {
        printf("Out of range row or column\n");
        return whiteMove(board, n);
    } else {
        if (board[whiteRow][whiteCol] != 'U') {
            printf("That square is already occupied or blocked\n");
            return whiteMove(board, n);
        } else {
            board[whiteRow][whiteCol] = 'W';
            printBoard(board, n);

            return true;
        }
    }
}

void blackScore(char board[21][21], int n) {
    int i;
    int j;
    int blackLongest;
    int whiteLongest;
    int maxNumber = 0;
    int row = 0;
    int col = 0;
    int score;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (board[i][j] == 'U') {
                board[i][j] = 'B'; //move black to this square
                blackLongest = findLongest(board, n, i, j);

                board[i][j] = 'W'; //move white to this square
                whiteLongest = findLongest(board, n, i, j);

				//printf("score[%d,%d]\n", blackLongest, whiteLongest);

                score = blackLongest + whiteLongest;
                if (score > maxNumber) {
                    maxNumber = score;
                    row = i;
                    col = j;
                }
                board[i][j] = 'U'; //reset to unoccupied
            }
        }
    }

    printf("Computer moves B at %d %d\n", row, col);

    board[row][col] = 'B';

    printBoard(board, n);
}

void whiteScore(char board[21][21], int n) {
    int i;
    int j;
    int blackLongest;
    int whiteLongest;
    int maxNumber = 0;
    int row;
    int col;
    int score;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (board[i][j] == 'U') {
                board[i][j] = 'B'; //move black to this square
                blackLongest = findLongest(board, n, i, j);

                board[i][j] = 'W'; //move white to this square
                whiteLongest = findLongest(board, n, i, j);

                score = blackLongest + whiteLongest;
                if (score > maxNumber) {
                    maxNumber = score;
                    row = i;
                    col = j;
                }
                board[i][j] = 'U'; //reset to unoccupied
            }
        }
    }

    printf("Computer moves W at %d %d\n", row, col);

    board[row][col] = 'W';

    printBoard(board, n);
}

bool checkWinner(char board[21][21], int n) {
    int k;
    int m;
    int maxBlack;
    int maxWhite;
    bool foundWinner = false;
    bool blackWins = false;
    bool whiteWins = false;

    for (k = 0; k < n && !foundWinner; k++) {
        for (m = 0; m < n && !foundWinner; m++) {//find longest line for black squares
            if (board[k][m] == 'B') {
                maxBlack = findLongest(board, n, k, m);
                if (maxBlack >= 6) {
                    printf("\nBlack player wins.\n");
                    foundWinner = true;
                    blackWins = true;
                    return true;
                }
            }
            if (board[k][m] == 'W') {//find longest line for white squares
                maxWhite = findLongest(board, n, k, m);
                if (maxWhite >= 6) {
                    printf("\nWhite player wins.\n");
                    foundWinner = true;
                    whiteWins = true;
                    return true;
                }
            }
        }
    }

	return false;
}

bool checkDraw(char board[21][21], int n) {
    int i;
    int j;
    int sum = 0;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (board[i][j] == 'U')
                sum = sum + 1;
        }
    }

    if (sum == 0) {
        printf("\nDraw!\n");
        return true;
    } else
        return false;
}

void blackPlayer(char board[21][21], int n) {
    bool finished = false;
    bool white = false;
    bool win;
    bool draw;

    while (!finished) {

        blackScore(board, n);
        win = checkWinner(board, n);
        draw = checkDraw(board, n);

        if (draw == true || win == true)
            finished = true;
        else {
            white = whiteMove(board, n);

            win = checkWinner(board, n);
            draw = checkDraw(board, n);

            if (draw == true || win == true)
                finished = true;
        }
    }
}

void whitePlayer(char board[21][21], int n) {
    bool finished = false;
    bool draw;
    bool win;
    bool black;

    while (!finished) {
        black = blackMove(board, n);

        win = checkWinner(board, n);
        draw = checkDraw(board, n);

        if (draw == true || win == true)
            finished = true;
        else {
            whiteScore(board, n);

            win = checkWinner(board, n);
            draw = checkDraw(board, n);

            if (draw == true || win == true)
                finished = true;
        }
    }
}

int main(void) 
{
    int dimensions;
    char board[21][21];

    printf("Enter board dimensions (n), n>=6 and n<=21: ");
    scanf("%d", &dimensions);

    int i;
    int j;
    for (i = 0; i < dimensions; i++) { //print the board when unoccupied
        for (j = 0; j < dimensions; j++)
            board[i][j] = 'U';
    }
    printBoard(board, dimensions);

    char inFirstPlayer[10];
    printf("Computer playing B or W?: ");
    scanf("%s", &inFirstPlayer); /* just read a char */

    blockedSquare(board, dimensions);

    if (inFirstPlayer[0] == 'B')
        blackPlayer(board, dimensions);
    else if (inFirstPlayer[0] == 'W')
        whitePlayer(board, dimensions);

    return (EXIT_SUCCESS);
}


