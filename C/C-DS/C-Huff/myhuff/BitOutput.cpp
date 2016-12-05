#include "BitOutput.h"

char setBit(char b, int bit, int n) {
  b = b & ~(1 << n);
  return b | (bit << n);
}

void BitOutput::writeBit(int bit) {
  if (bufi == 8) {
    flush();
  }
  buf = setBit(buf, bit, bufi);
  bufi++;
}

void BitOutput::writeByte(int b) {
  out.put(b);
}

void BitOutput::writeInt(int i) {
  out.write((char*)& i, sizeof(i));
}

void BitOutput::flush() {
  out.put(buf);
  out.flush();
  buf = bufi = 0;
}
