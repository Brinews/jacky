#include <iostream>
#include "grid.h"
using namespace std;

Grid::Grid() {
}

Grid::~Grid() { 
	if (td != NULL) delete td;
	if (gd != NULL) delete gd;
    clearGrid();
}

void Grid::clearGrid() {
	if (theGrid.size() > 0) {
		for (unsigned int i = 0; i < theGrid.size(); i++) {
			theGrid[i].clear();
		}
		theGrid.clear();
	}
}

bool Grid::isWon() const {
  // all light out or not ?
  for (int i = 0; i < gridSize; i++) {
	  for (int j = 0; j < gridSize; j++) {
		  if (theGrid[i][j].getState()) return false;
	  }
  }
  return true;
}

void Grid::init(int n) {
	gridSize = n;
	td = new TextDisplay(n);
	gd = new GraphicDisplay(n);

	clearGrid();

    // push elements
	theGrid.resize(n);
	for (int i = 0; i < n; i++) {
		theGrid[i].resize(n);
		for (int j = 0; j < n; j++) {
            Cell c;
            theGrid[i].push_back(c);
        }
    }

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			theGrid[i][j].setCoords(i, j);

            // add observers
			theGrid[i][j].attach(td);
			theGrid[i][j].attach(gd);

			if (i > 0)
				theGrid[i][j].attach(&(theGrid[i-1][j]));
			if (i < n-1)
				theGrid[i][j].attach(&(theGrid[i+1][j]));
			if (j > 0)
				theGrid[i][j].attach(&(theGrid[i][j-1]));
			if (j < n-1)
				theGrid[i][j].attach(&(theGrid[i][j+1]));
		}
	}
}

void Grid::turnOn(int r, int c) {
  theGrid[r][c].setOn();
}

void Grid::toggle(int r, int c) {
  theGrid[r][c].toggle();
}

void Grid::display(Xwindow *win, int width, int height) {
  gd->display(win, width, height);
}

ostream &operator<<(ostream &out, const Grid &g) {
    // output the grid text
	out << *(g.td);
	return out;
}
