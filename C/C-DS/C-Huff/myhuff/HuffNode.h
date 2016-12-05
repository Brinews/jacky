#ifndef HUFFNODE_H
#define HUFFNODE_H

#include <iostream>

using namespace std;

typedef unsigned char byte;

class HuffNode {
public:
  int count;
  byte symbol;
  HuffNode* c0;
  HuffNode* c1;
  HuffNode* p;

  HuffNode(int count, byte symbol, HuffNode* c0 = 0, 
          HuffNode* c1 = 0, HuffNode* p = 0)
      : count(count), symbol(symbol), c0(c0), c1(c1), p(p) {
  }

  bool operator< (const HuffNode& other);
};

ostream& operator<< (ostream&, const HuffNode&);

#endif
