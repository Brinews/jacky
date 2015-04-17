#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <stdlib.h>
#include <ctime>
#include <string>

using namespace std;

#ifndef MINESWEEPER_H_
#define MINESWEEPER_H_


class minesweeper {
private:
	int colNum;                                     //number of columns on game board (X))
	int rowNum;                                     //number of rows on game board(Y)
	int minesNum;                                   //number of mines placed on game board
        int end;                                       //end game condition tracker (-1 loss, 0 ongoing, 1 win)
        vector<vector<int> > mineField;       //actual representation of game board (-1 mine, 0 no surrounding, # > 0 number of surrounding mines)
        vector<vector<bool> > bitField;       //representation of tiles to be hidden or shown (true hidden, false shown). 
        
        void calculateSurrounding(int row, int col);    //Updates board tiles by adding 1 to tiles surrounding mine except when adjacent tile is mine
        void onlyMines();                               //Checks end game status (int end). Should check entire playing field, no internal counter
        int randomPick(int num);                        //Randomly generates number between 0 and num
        void unmask(int row, int col);                  //Actual function to reveal blank tile. Makes recursive calls for 0 value tiles

        
public:
	
	minesweeper();                                  //Default constructor
	virtual ~minesweeper();                         //Destructor
        minesweeper(int col, int row, int numOfMines);  //Constructor given board dimensions and number of mines
        
	int getColNum();                                //Return # of columns for game board
	int getRowNum();                                //Return # of rows for game board
	int getMinesNum();                              //Return total # of mines on game board
        int endGame();                                  //Return current end game status
	void initialMineField(int fpX, int fpY);        //Initialize game board given first revealed tile
        void initialMineField(string path);             //Initialize game board given file path of initial mine placement w/o checking for 1st turn game over
		void initialMineField(std::vector<std::vector<int> > input);
        bool isRevealed(int x, int y);                  //Checks if tile has been already revealed
	void revealLocation(int x, int y);              //Reveals selected tile. Selected tile should return true on subsequent isRevealed calls, the left click action
        void markLocation(int x, int y);                  // mark a cell as potential mine, the right click action
        int valueOf(int x, int y);                      //Returns tile value (# of surrounding mines if not mine)

		void debug() {
			for (int i = 0; i < mineField.size(); i++) {
				for (int j = 0; j < mineField[i].size(); j++) {
					printf("%3d", mineField[i][j]);
				}
				printf("\n");
			}
		}

		void saveMine(string path);
};


#endif /* MINESWEEPER_H_ */
