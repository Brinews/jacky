#include <iostream>
#include <utility>

#include "graphicdisplay.h"
#include "cell.h"
#include "info.h"
using namespace std;

GraphicDisplay::GraphicDisplay(int n): gridSize(n)  {
	theDisplay.resize(n);
	for (int i = 0; i < n; i++) {
		theDisplay[i].resize(n);
        for (int j = 0; j < n; j++) {
            theDisplay[i][j] = '_';
        }
	}
}

void GraphicDisplay::notify(Subject &whoNotified) {
	Info inf = whoNotified.getInfo();
	if (inf.state)
		theDisplay[inf.row][inf.col] = 'X';
	else
		theDisplay[inf.row][inf.col] = '_';
}

GraphicDisplay::~GraphicDisplay() {
	if (theDisplay.size() > 0) {
		for (unsigned int i = 0; i < theDisplay.size(); i++)
			theDisplay[i].clear();

		theDisplay.clear();
	}
}

SubscriptionType GraphicDisplay::subType() const {
  return SubscriptionType::All;
}

void GraphicDisplay::display(Xwindow *xwin, int width, int height) {
    int w, h;

    w = width / gridSize;
    h = height / gridSize;

    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            if (theDisplay[i][j] == '_')
                xwin->fillRectangle(j*w, i*h, w, h, Xwindow::Black);
            else
                xwin->fillRectangle(j*w, i*h, w, h, Xwindow::White);
        }
    }
}
