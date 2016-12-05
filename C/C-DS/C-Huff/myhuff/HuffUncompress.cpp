#include <iostream>
#include <fstream>
#include <cerrno>
#include <cstring>
#include "HuffTree.h"

using namespace std;

int main(int argc, char** argv) {

  if (argc != 3) {
    cout << "Usage: HuffUncompress infile outfile" << endl;
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

  int frequency;
  long freqCnt = 0;
  int unichar = 0;
  // read file header
  for (int i = 0; i < freqs.size(); i++) {
    frequency = bin.readInt();
    if (inFile.eof()) {
      cout << "Error reading header in input file." << endl;
      return -1;
    }
    freqs[i] = frequency;
    if (frequency > 0) {
      freqCnt += frequency;
      unichar++;
    }
  }

  // build tree
  tree.build(freqs);

  // write uncompress file
  while (freqCnt > 0) {
    int symbol = tree.decode(bin); // huffman decode

    if (symbol == -1) {
      cout << "Error decoding input file." << endl;
      return -1;
    } else if (inFile.eof()) {
      break;
    }

    bout.writeByte(symbol);

    freqCnt--;
  }

  cout << "Uncompress...OK\n";

  // close
  inFile.close();
  outFile.close();

  return 0;
}
