#include "BitInput.h"

char bitVal(char b, int n) {
  return (b >> n) & 1;
}

int BitInput::readBit() {
  if (bufi == 8) {
    fill();
  }

  if (in.eof()) {
    return -1;
  }

  int bit = bitVal(buf, bufi);
  bufi++;
  return bit;
}

int BitInput::readByte() {
  int byte = in.get();
  if (in.eof()) {
    return -1;
  }
  return byte;
}

int BitInput::readInt() {
  int integer;
  in.read((char*) &integer, sizeof(integer));
  if (in.eof()) {
    return -1;
  }
  return integer;
}

void BitInput::fill() {
  buf = in.get();
  bufi = 0;
}
