#include "HuffNode.h"

bool HuffNode::operator< (const HuffNode& other) {

  if (count != other.count) {
    return count > other.count;
  }

  return symbol > other.symbol;
}

ostream& operator<< (ostream& os, const HuffNode& n) {
    os << "[" << n.count << "," << (int) (n.symbol) << "]";
    return os;
}
