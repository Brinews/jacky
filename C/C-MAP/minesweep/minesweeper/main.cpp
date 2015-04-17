#include <cstdlib>
#include <iostream>

#include "minesweeper.h"
#include "bonus.h"

using namespace std;

void displayMine(bonus& play);
void playTime(bonus& play);
void viewStatistics(bonus &play);

char *prompt = "Game Options:\n-----------\n1. Start Game\n2. View Stats\n3. Quit program\n>>>";

struct minescale {
	int col;
	int row;
	int numOfMines;
} scales[4] = {
	{4, 4, 4},
	{9, 9, 10},
	{16, 16, 40},
	{16, 30, 99}
};

/*
 * contruct option string
 */
string options() 
{
	string s = "Mine Scale:(column x rows : numOfMines)\n";
	char buf[16];

	for (int i = 0; i < sizeof(scales)/sizeof(struct minescale); i++) {
		s += itoa(i, buf, 10);
		s += ". ";
		s += itoa(scales[i].col, buf, 10);
		s += "x";
		s += itoa(scales[i].row, buf, 10);
		s += ":";
		s += itoa(scales[i].numOfMines, buf, 10);
		s += "; ";
	}

	return s;
}
/*
 * 
 */
int main() {

	int opt;
	int level; /* 9x9:10, 16x16:40, 16x30:99 */
	bonus *mine = NULL;

    
     /* This will be your main GUI loop. You should query the user for actions such 
     * as starting a game, viewing statistics, or quitting the program. You should
     * handle user input either succinctly or by making a call to a helper method
     * to perform the desired operations.
     */
    while (1) {

        
	//TODO - Ask the user with options to perform such as Starting a game, Viewing
	// statistics, Quitting the program,  and any 
        // other functionality you feel is necessary.
        // Each iteration here is one round of the game.
	
        // somewhere you need to create a minesweeper game object first if the 
        // user decides to start a new round of game, e.g., 
        // minesweeper play();

		do {
			cout << prompt;
			cin >> opt;
		} while (opt < 1 || opt > 3);
		
		switch (opt) {
			case 1:
				if (NULL != mine) {
					delete mine;
				}

				do {
					cout << options();
					cout << "\n>>>";
					cin >> level;
				} while (level < 0 || 
						level > sizeof(scales)/sizeof(struct minescale));

				mine = new bonus(scales[level].col, 
						scales[level].row, scales[level].numOfMines);

				playTime(*mine);

				break;
			case 2:
				viewStatistics(*mine);
				break;
		}

		if (opt == 3) break; //exit loop
    }

	delete mine;

    return 0;
}

/* This should be your game UI function. If a user decides to play a game of minesweeper,
* UI interaction with a minesweeper object should be done here. For example, the repeated
* printing of the board state and handling of user game action such as left/right click
* on a particular cell should be done here
*/
void playTime(bonus& play) {
//TODO - Begin the minesweeper game function; should interact with minesweeper class/object here

	int x, y;
	string buf;
	int endFlag = 0;
	string fileName;

	while (play.endGame() == 0) {
		
		displayMine(play);

		play.debug();

		do {
			printf("Please input location x[0, %d]:", play.getRowNum());
			cin >> x;
			printf("Please input location y[0, %d]:", play.getColNum());
			cin >> y;
		} while (x < 0 || x > play.getRowNum() || 
				y < 0 || y > play.getColNum());

		printf("(D - Dig, S - Sign, R - Restart, W - Save Game, E - Exit Game)\n");
		printf("Please input:");
		cin >> buf;

		switch (buf[0]) {
			case 'D':
				play.revealLocation(x, y);
				break;
			case 'S':
				play.markLocation(x, y);
				break;
			case 'R':
				play.initialMineField(rand()%play.getRowNum(),
						rand()%play.getColNum());
				break;
			case 'W':
				cout << "Input Save File Name:";
				cin >> fileName;
				play.saveGame(fileName);
				break;
			case 'E':
				endFlag = 1;
				break;
			default:
				break;
		}

		if (endFlag == 1) break;

		// game end
		if (play.endGame() != 0) {
			// 0. set statistics
			if (play.endGame() == 1) {
				play.setWins();
				printf("YOU WIN.\n\n");
			}
			else if (play.endGame() == -1) {
				play.setLoss();
				printf("YOU LOST.\n\n");
			}

			play.setWinStreak();
			play.setLongestWinStreak();

			char yOrN;

			// 1. save statistics
			do {
				cout << "Save Statistics?(Y/N)" << endl;
				cin >> yOrN;
			} while (yOrN != 'Y' && yOrN != 'N');

			if (yOrN == 'Y') {
				cout << "Input to be saved file name:" << endl;
				cin >> fileName;
				play.saveStatistics(fileName);
			}

			// 2. play again and load from file
			do {
				cout << "Play Again?(Y/N)" << endl;
				cin >> yOrN;
			} while (yOrN != 'Y' && yOrN != 'N');

			if (yOrN == 'Y') {
				do {
					printf("Load From File?(Y/N)");
					cin >> yOrN;
				} while (yOrN != 'Y' && yOrN != 'N');

				if (yOrN == 'Y') {
					cout << "Input File Name:";
					cin >> fileName;
					play.loadGame(fileName);
				}
				else 
					play.initialMineField(rand()%play.getRowNum(),
						rand()%play.getColNum());
			}
		}
	}

}

/* display the statistics of the game. This is a bonus point function
 */
void viewStatistics(bonus &play) {
    cout << "Statistics List:\n" << endl;
	cout << "Wins: " << play.getWins() << endl;
	cout << "Loss: " << play.getLoss() << endl;
	cout << "Ratio: " << play.getRatio() << endl;
	cout << "WinStreak: " << play.getWinStreak() << endl;
	cout << "LongestWinStreak: " << play.getLongestWinStreak() << endl;
}

void displayMine(bonus &play)
{
	int i, j;

	printf("Mine Board State Marks:\n");
	printf("(!:marked,#:untack,*:mine,n:numOfMine)\n");

	for (i = 0; i < play.getRowNum() + 2; i++) {
		for (j = 0; j < play.getColNum() + 1; j++) {
			if (i == 0) {
				if (j == 0) printf("   |");
				else printf("%3d", j-1);
			}

			if (i == 1) {
				if (j == 0) printf("___|");
				else printf("____");
			}

			if (i > 1) {
				if (j == 0) printf("%3d|", i-2);
				else {
					int r = play.valueOf(i-2, j-1);

					if (r >= 0) /* revealed */
						printf("%3d", r);
					else if (r == -2) /* marked */ 
						printf("  !");
					else if (r == -3) /* untacked */
						printf("  #");
					else if (r == -1)
						printf("  *"); /* mine */
				}
			}
		}
		/* extra Y */
		if (i == 1) printf("Y");
		printf("\n");
	}

	printf("    X\n");
}
