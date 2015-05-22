#include "minesweeper.h"
#include <fstream>

using namespace std;

minesweeper::minesweeper() {
	// TODO - default two dimension array is 9 X 9 with 10 mines
    //
	colNum = 9;
	rowNum = 9;
	minesNum = 10;
	end = 0;

	//srand(time(NULL)); // setting random seed
	//initialMineField(randomPick(rowNum), randomPick(colNum));

    boardInit();
}

minesweeper::minesweeper(int col, int row, int numOfMines) {
	// TODO Auto-generated constructor stub
	// TODO - two dimension gameboard size col x num with numOfMines mines

	colNum = col;
	rowNum = row;
	minesNum = numOfMines;
	end = 0;

	//srand(time(NULL));
	//initialMineField(randomPick(rowNum), randomPick(colNum));

    boardInit();
}

void minesweeper::boardInit()
{
    mineField.clear();
	bitField.clear();

	for (int i = 0; i < rowNum; i++) {
		vector<int> minei;
		vector<bool> biti;

		for (int j = 0; j < colNum; j++) {
			minei.push_back(0);
			biti.push_back(true);
		}

		mineField.push_back(minei);
		bitField.push_back(biti);
	}
}

void minesweeper::selectFirstDig(int x, int y)
{
    initialMineField(x, y);
}

minesweeper::~minesweeper() {
	// TODO Auto-generated destructor stub
}

/**
 * Returns int representing number of rows of current playing field.
 *
 * @return      Number of rows on playing field
 * */
int minesweeper::getRowNum()
{
	return rowNum;
}

/**
 * Returns int representing number of columns of current playing field.
 *
 * @return      Number of rows on playing field
 * */
int minesweeper::getColNum()
{
	return colNum;
}
/**
 * Returns int representing number of mines on current playing field.
 * Includes both hidden and revealed mines.
 *
 * @return      Number of rows on playing field
 * */
int minesweeper::getMinesNum()
{
	return minesNum;
}

/**
 * Returns int randomly generated between 0 and num
 *
 * @param   num Upper limit of randomly generated number
 * @return      Number of rows on playing field
 * @see         initialMineField()
 * */
int minesweeper::randomPick(int num)
{
	return rand()%num;
}

/**
 * another load-method
 * file-format:
 *   rowNum colNum mineNum
 *   minePosX minePosY
 * @param path string
 */
void minesweeper::initialMineField(string path)
{
	int x, y;
	int data;
	bool state;

	ifstream fin(path.c_str());

	fin >> rowNum >> colNum >> minesNum;

    // 0. base board initialization
    boardInit();
		
	// 1. place mines
	for (int i = 0; i < minesNum; i++) {
		int x, y;

        fin >> x >> y;

		mineField[x][y] = -1;
	}

	// 2. update game board representation
	for (int i = 0; i < rowNum; i++)
		for (int j = 0; j < colNum; j++)
			calculateSurrounding(i, j);

	fin.close();

    end = 0;
}

void minesweeper::initialMineField(string path, int flag)
{
	int x, y;
	int data;
	bool state;

	ifstream fin(path.c_str());

	fin >> rowNum >> colNum >> minesNum;

    // 0. base board initialization
    boardInit();
	
	//1. load board
	for (int i = 0; i < rowNum; i++) {
		for (int j = 0; j < colNum; j++) {
			fin >> data;
			mineField[i][j] = data;
		}
	}

	//2. load mask
	for (int i = 0; i < rowNum; i++) {
		for (int j = 0; j < colNum; j++) {
			fin >> state;
			bitField[i][j] = state;
		}
	}

	fin.close();

	end = 0;
}

void minesweeper::saveMine(string path)
{
	ofstream fout(path.c_str());

	fout << rowNum << " " << colNum << " " << minesNum << endl;
	
	//1. save board
	for (int i = 0; i < rowNum; i++) {
		for (int j = 0; j < colNum; j++) {
			fout << mineField[i][j] << " ";
		}
		fout << endl;
	}

	//2. save mask
	for (int i = 0; i < rowNum; i++) {
		for (int j = 0; j < colNum; j++) {
			fout << bitField[i][j] << " ";
		}
		fout << endl;
	}

	fout.close();
}

/**
 * Generates mine field after accepting player's first turn.
 * Does not have to avoid first turn game over. 
 *
 * @param   fpX X-coordinate of first pick
 * @param   fpY Y-coordinate of first pick
 * @see     calculateSurrounding()
 * @see     randomPick()
 * */
void minesweeper::initialMineField(int fpX, int fpY) {
	//Generate our map and mask
	//TODO - generate all mines randomly
	//surrounding tile values should be updated to reflect presence of adjacent mine
	//
	// 0. base board initialization
	boardInit();
	
	// 1. place mines randomly 
	for (int i = 0; i < minesNum; i++) {
		int x, y;

		do {
			x = randomPick(rowNum);
			y = randomPick(colNum);
		} while (x == fpX && y == fpY);

		mineField[x][y] = -1;
	}

	// 2. update game board representation
	for (int i = 0; i < rowNum; i++)
		for (int j = 0; j < colNum; j++)
			calculateSurrounding(i, j);

	// 3. reveal the first turn
	bitField[fpX][fpY] = false;

	end = 0;
}

/**
 * Generates mine field based on 2D vector. This option
 * does not have to check the user's first turn for 1-turn game over.
 *
 * @param   path to load MineField from
 * */
void minesweeper::initialMineField(std::vector<std::vector<int> > input) { 
	//TODO - load mine field from 2D vector
	//Is not concerned with user's first tile
	
	rowNum = input.size();
	colNum = input[0].size();
	
	bitField.clear();
	mineField.clear();
	
	for (int i = 0; i < input.size(); i++) {
		vector<int> imine;
		vector<bool> bmine;

		for (int j = 0; j < input[i].size(); j++) {
			imine.push_back(input[i][j]);
			bmine.push_back(true);
		}

		bitField.push_back(bmine);
		mineField.push_back(imine);
	}

	end = 0;
}

/**
 * Generates numbers for surrounding tiles of mines. The only
 * tiles with numbers are those surrounding mines; these tiles are
 * updated as mines are generated.

 *
 * @param   row Row number of generated mine
 * @param   col Column number of generated mine
 * @see     initialMineField()
 * */
void minesweeper::calculateSurrounding(int row, int col) {
	//TODO - should update surrounding tiles to reflect
	//presence of adjacent mine

	int i, j;
	int sum = 0;

	if (mineField[row][col] == -1) return;

	// eight direction
	int p_offset[8][2] = { {0,1}, {1, 1}, {1, 0}, {1, -1},
						   {0,-1}, {-1,-1}, {-1,0}, {-1, 1}};

	for (i = 0; i < 8; i++) {
		int px = row + p_offset[i][0];
		int py = col + p_offset[i][1];

		if (px >=0 && px < rowNum 
				&& py >= 0 && py < colNum) {
			if (mineField[px][py] == -1)
				sum += 1;
		}
	}

	mineField[row][col] = sum;
}

/**
 * Updates bitField with user selection of revealed location.
 * Reveals current location and immediately checks mineField
 * for mine. Recursively calls helper function unmask for blank
 * tiles. 
 *
 * @param   row Row number of user selected tile
 * @param   col Column number of user selected tile
 * @see         unmask();
 * */
void minesweeper::revealLocation(int x, int y) {
	//TODO - update the bitField to reflect the user's
	//tile selection. Should rely on unmask to do actual
	//reveal operation.
	
	if (mineField[x][y] == 0) unmask(x, y);

	if (mineField[x][y] == -1) end = -1; //kick mine, end game

	if (mineField[x][y] > 0 && mineField[x][y] <= 8) {
		bitField[x][y] = false;
	}
}

/**
 * Allow the user to mark a cell as a potential mine. Marking an already
 * marked cell will remove the mark for that cell
 *
 * @param x column number of the cell selected
 * @param y row number of the cell selected
 */
void minesweeper::markLocation(int x, int y) {
    // TODO - update the display for the selected cell, change it
    // to marked if it's not marked, or to unmarked if it's already marked
	// mark the Mine = mineField + 10
	// unmark the Mine = mineField - 10
	
	if (bitField[x][y] == true) {
		if (mineField[x][y] > 4) mineField[x][y] -= 10;
		else mineField[x][y] += 10;
	}
}

/**
 * Checks end game status (int end). End -1 is loss condition.
 * Also checks onlyMines() for a win condition. onlyMines will
 * update the end game status and endGame should return this value.
 *
 * @return      end game status (-1 loss, 0 ongoing, 1 win)
 * @see         onlyMines();
 * @see		revealLocation();
 * */
int minesweeper::endGame() {
	//TODO - return current end game status.
	//Calling this method should only update int end with
	//win or ongoing condition. revealLocation should
	//update if there is loss condition.
	
	onlyMines();

	return end;
}

/**
 * Checks end game status (int end). End 1 is win condition;
 * end 0 is ongoing. onlyMines will only update end with these
 * two conditions. onlyMines will check the entire playing field
 * (does not rely on numMines or any internal counter). 
 *
 * @see         endGame();
 * */
void minesweeper::onlyMines() {
	//TODO - check for win condition, otherwise ongoing
	
	if (end == -1) return;
	
	end = 1; // assume only left mines
	
	for (int i = 0; i < rowNum; i++) {
		for (int j = 0; j < colNum; j++) {
			if (mineField[i][j] >= 0 && mineField[i][j] <= 8)
				if (bitField[i][j] == true)
					end = 0; // find a hidden non-mine
		}
	}
}

/**
 * Checks if the selected position should be visible
 *
 * @param   row Row number of user selected tile
 * @param   col Column number of user selected tile
 * @see         valueOf();
 * */
bool minesweeper::isRevealed(int x, int y) {
	//TODO - check if a user has revealed a specific tile
	
	return bitField[x][y];
}

/**
 * Checks if the value of selected tile. isRevealed
 * should be called before this for correct gameplay. 
 * 
 * @param   row Row number of user selected tile
 * @param   col Column number of user selected tile
 * @see         isRevealed();
 * */
int minesweeper::valueOf(int x, int y) {
	//TODO - returns the value of specific tile.
	//should only be called if tile isRevealed.
	
	if (!isRevealed(x, y)) return mineField[x][y];

	if (mineField[x][y] > 8) return -2; // marked state

	return -3; // untack state
}

/**
 * Reveals surrounding tiles. Should only be called if
 * user selected tile was not mine or had no surrounding
 * mines. Will recursively call itself to reveal all
 * adjacent blank tiles.
 *
 * @param   row Row number of user selected tile
 * @param   col Column number of user selected tile
 * @see         revealLocation();
 * */
void minesweeper::unmask(int px, int py) {
	//TODO - reveal the tile here.
	//This method should reveal surrounding tiles
	//if the tile revealed had a value of 0
	
	if (px < 0 || px >= rowNum ||
			py < 0 || py >= colNum)
		return;

	if (mineField[px][py] != 0) {
        if (mineField[px][py] != -1) {
            bitField[px][py] = false;
        }
        return;
    }
	if (bitField[px][py] == false) return;

	bitField[px][py] = false;

	/* up, down, left, right */
	int i;
	int p_offset[4][2] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0}};

	for (i = 0; i < 4; i++)
	{
		int nx, ny;

		nx = px + p_offset[i][0];
		ny = py + p_offset[i][1];

		unmask(nx, ny);
	}
}
