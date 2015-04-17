#include "bonus.h"

#include <fstream>

using namespace std;

bonus::bonus():wins(0), loss(0), ratio(0.0),winStreak(0), 
	longestWinStreak(0), minesweeper()
{
}

bonus::bonus(int col, int row, int numOfMines)
	:wins(0), loss(0), ratio(0.0),winStreak(0), longestWinStreak(0)
	,minesweeper(col, row, numOfMines)
{
}

int bonus::saveGame(string path)
{
	minesweeper::saveMine(path);
}

int bonus::loadGame(string path)
{
	minesweeper::initialMineField(path);
}

int bonus::saveStatistics(string path)
{
	ofstream fout(path.c_str());

	fout << wins << endl;
	fout << loss << endl;
	fout << winStreak << endl;
	fout << longestWinStreak << endl;

	fout.close();
}

int bonus::loadStatistics(string path)
{
	ifstream fin(path.c_str());

	fin >> wins;
	fin >> loss;
	fin >> winStreak;
	fin >> longestWinStreak;

	fin.close();
}

int bonus::getWins()
{
	return wins;
}

void bonus::setWins()
{
	wins++;
}

int bonus::getLoss()
{
	return loss;
}

void bonus::setLoss()
{
	loss++;
}

double bonus::getRatio()
{
	calcRatio();

	return ratio;
}

int bonus::getWinStreak()
{
	return winStreak;
}

void bonus::setWinStreak()
{
	int e = endGame();
	if (e == 1) winStreak++;
	if (e == -1) winStreak = 0;
}

int bonus::getLongestWinStreak()
{
	return longestWinStreak;
}

void bonus::setLongestWinStreak()
{
	if (winStreak > longestWinStreak)
		longestWinStreak = winStreak;
}

void bonus::calcRatio()
{
	if (wins == 0) ratio = 0;
	else ratio = 1.0*wins/(wins+loss);
}

/*
void bonus::initialMineField(int fpX, int fpY)
{
	minesweeper::initialMineField(fpX, fpY);
}
*/
