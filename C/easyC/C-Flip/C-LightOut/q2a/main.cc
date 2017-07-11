#include <iostream>
#include <string>
#include "grid.h"
using namespace std;

int main() {
  cin.exceptions(ios::eofbit|ios::failbit|ios::badbit);
  string cmd;
  Grid g;
  int moves = 0;

  // You will need to make changes to this code.

  try {
    while (true) {
      cin >> cmd;
      if (cmd == "new") {
        int n;
        cin >> n;
        g.init(n);
      }
      else if (cmd == "init") {
        int x = 0, y = 0;
        while (true) {
            cin >> x >> y;
            if (x < 0 || y < 0) break;
            g.turnOn(x, y);
        }
        cout << g;
      }
      else if (cmd == "game") {
        cin >> moves;
        cout << moves << " moves left" << endl;
      }
      else if (cmd == "switch") {
        int r = 0, c = 0;
        cin >> r >> c;
        g.toggle(r, c);
        cout << g;
        moves--;
        cout << moves << " moves left" << endl;
        if (moves <= 0) {
            cout << "Lost" << endl;
            return 0;
        }
        if (g.isWon()) {
            cout << "Won" << endl;
            return 0;
        }
      }
    }
  }
  catch (ios::failure &) {
  }

  return 0;
}
