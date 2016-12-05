#ifndef BITINPUT_H
#define BITINPUT_H

#include <iostream>
#include <bitset>

using namespace std;

class BitInput {

private:
  istream& in;
  char buf;
  int bufi;

public:
  BitInput(istream& s) : in(s), buf(0), bufi(8) { }

  int readBit();
  int readByte();
  int readInt();

  void fill();
};

#endif
