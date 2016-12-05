#include <iostream>
#include <fstream>
#include <cerrno>
#include <cstring>

#include "HuffTree.h"

using namespace std;

int main(int argc, char** argv) {

  if (argc != 3) {
    cout << "Usage: HuffCompress infile outfile" << endl;
    return -1;
  }

  if (strcmp(argv[1], argv[2]) == 0) {
    cout << "infile and outfile should be different files." << endl;
    return -1;
  }

  ifstream inFile;
  ofstream outFile;

  inFile.open(argv[1], std::ifstream::binary);

  if (inFile.fail()) {
    cerr << "Error opening input file: " << strerror(errno) << "." << endl;
    return -1;
  }

  outFile.open(argv[2], std::ofstream::binary);

  if (outFile.fail()) {
    cerr << "Error opening output file: " << strerror(errno) << "." << endl;
    inFile.close();
    return -1;
  }

  BitOutput bout = BitOutput(outFile);
  BitInput bin = BitInput(inFile);

  HuffTree tree = HuffTree();

  vector<int> freqs = vector<int>(256, 0);

  int symbol;

  // read in file
  while (1) {
    symbol = bin.readByte();
    if (inFile.eof()) {
      break;
    }
    freqs[symbol]++;
  }

  // build tree
  tree.build(freqs);

  // write header
  int totalSymbols = 0;
  long totalCount = 0;
  for (int i = 0; i < freqs.size(); i++) {
    if (freqs[i] > 0) {
      totalSymbols++;
      totalCount += freqs[i];
    }
    bout.writeInt(freqs[i]);
  }


  // write compress file
  inFile.clear();
  inFile.seekg(0, ios::beg);
  while (1) {
    symbol = bin.readByte();
    if (inFile.eof()) {
      break;
    }
    tree.encode(symbol, bout);
  }
  bout.flush();

  cout << "Compress...OK\n";
  // close
  inFile.close();
  outFile.close();

  return 0;
}
