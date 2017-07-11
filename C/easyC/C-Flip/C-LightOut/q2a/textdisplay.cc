#include <iostream>
#include <utility>
#include "textdisplay.h"
#include "cell.h"
#include "info.h"
using namespace std;

TextDisplay::TextDisplay(int n): gridSize(n)  {
	theDisplay.resize(n);
	for (int i = 0; i < n; i++) {
		theDisplay[i].resize(n);
        for (int j = 0; j < n; j++) {
            theDisplay[i][j] = '_';
        }
	}
}

void TextDisplay::notify(Subject &whoNotified) {
	Info inf = whoNotified.getInfo();
	if (inf.state)
		theDisplay[inf.row][inf.col] = 'X';
	else
		theDisplay[inf.row][inf.col] = '_';
}

TextDisplay::~TextDisplay() {
	if (theDisplay.size() > 0) {
		for (unsigned int i = 0; i < theDisplay.size(); i++)
			theDisplay[i].clear();

		theDisplay.clear();
	}
}

SubscriptionType TextDisplay::subType() const {
  return SubscriptionType::All;
}

ostream &operator<<(ostream &out, const TextDisplay &td) {
  for (unsigned int i = 0; i < td.theDisplay.size(); i++) {
	  for (unsigned int j = 0; j < td.theDisplay[i].size(); j++) {
		  cout << td.theDisplay[i][j];
	  }
	  cout << endl;
  }

  return out;
}
