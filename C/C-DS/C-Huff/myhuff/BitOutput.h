#ifndef BITOUTPUT_H
#define BITOUTPUT_H

#include <iostream>
#include <bitset>

using namespace std;

class BitOutput {

private:
  ostream& out;
  char buf;
  int bufi;

public:
  BitOutput(ostream& s) : out(s), buf(0), bufi(0) { }

  void writeBit(int bit);
  void writeByte(int b);
  void writeInt(int i);

  void flush();
};

#endif
