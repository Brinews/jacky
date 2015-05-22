#include <cstdlib>
#include <iostream>

#include "minesweeper.h"
#include "bonus.h"

using namespace std;

void displayMine(bonus& play);
void playTime(bonus& play);
void viewStatistics(bonus &play);
int parseInt(const string &buf);
void selectPosition(bonus &play, int pos[]);

string prompt = "Game Options:\n-----------\n1. Start Game\n2. View Stats\n3. Quit program\nOptions:[1/2/3] >>>";

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

string digitToStr(int a)
{
    string s = "";

    while (a > 0) {
        int left = a%10;
        a /= 10;
        string t = "";
        t += (char) ('0'+left);
        s = t + s;
    }

    if (s == "") s = "0";

    return s;
}

int parseInt(const string &buf)
{
    int ret = 0;

    for (int i = 0; i < buf.length(); i++) {
        if (buf[i] < '0' || buf[i] > '9') return -1;
        ret = ret*10 + (buf[i] - '0');
    }

    return ret;
}

/*
 * contruct option string
 */
string options() 
{
	string s = "Mine Scale:(column x rows : numOfMines)\n";
	char buf[16];

	for (int i = 0; i < sizeof(scales)/sizeof(struct minescale); i++) {
		s += digitToStr(i);
		s += ". ";
		s += digitToStr(scales[i].col);
		s += "x";
		s += digitToStr(scales[i].row);
		s += ":";
		s += digitToStr(scales[i].numOfMines);
		s += "; ";
	}

	return s;
}

void selectPosition(bonus &play, int pos[2])
{
    int x,y;
    string buf;
    do {
        printf("Please input location x[0, %d]:", play.getRowNum()-1);
        cin >> buf;
        x = parseInt(buf);
        printf("Please input location y[0, %d]:", play.getColNum()-1);
        cin >> buf;
        y = parseInt(buf);
    } while (x < 0 || x > play.getRowNum()-1 || 
            y < 0 || y > play.getColNum()-1);


    printf("Your select position(%d,%d).\n", x, y);

    pos[0] = x;
    pos[1] = y;
}

/*
 * 
 */
int main() {

    string buf;
    string fileName;
	int opt;
	int level; /* 9x9:10, 16x16:40, 16x30:99 */
	bonus *mine = NULL;
    int pos[2];
    
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
            cin >> buf;
			//cin >> opt;
		} while (buf[0] < '1' || buf[0] > '3');

        opt = buf[0] - '0';
		
		switch (opt) {
			case 1:
				if (NULL != mine) {
					delete mine;
				}

                do {
                    printf("Start from load-file or new game?(Y/N)");
                    cin >> buf;
                } while (buf[0] != 'Y' && buf[0] != 'N');

                if (buf[0] == 'Y') {
                    mine = new bonus();
                    cout << "Input Load-file Name:";
                    cin >> fileName;
                    mine->loadGame(fileName);
                }
                else {

                    do {
                        cout << options();
                        cout << "\n[0/1/2/3]>>>";
                        cin >> buf;
                        level = parseInt(buf);
                    } while (level < 0 || level > sizeof(scales)/
                            sizeof(struct minescale));

                    mine = new bonus(scales[level].col, 
                            scales[level].row, 
                            scales[level].numOfMines);

                    /* first dig */
                    displayMine(*mine);
                    selectPosition(*mine, pos);
                    mine->selectFirstDig(pos[0], pos[1]);
                }

                playTime(*mine);

                break;
            case 2:
                if (mine != NULL) {
                    viewStatistics(*mine);
                }
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
    int pos[2];
	string buf;
	int endFlag = 0;
	string fileName;

	while (play.endGame() == 0) {
		
		displayMine(play);

		//play.debug();

        selectPosition(play, pos);
        x = pos[0];
        y = pos[1];
		
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
				else {
                    displayMine(play);
                    selectPosition(play, pos);
                    play.selectFirstDig(pos[0], pos[1]);
                    /*
                    play.initialMineField(rand()%play.getRowNum(),
						rand()%play.getColNum());
                     */
                }
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
