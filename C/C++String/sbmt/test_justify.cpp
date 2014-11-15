#include "string.hpp"

#include <iostream>
#include <fstream>

using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::ostream;
using std::ofstream;

/* do justify */
void doJustify(ifstream &fin, ostream &fout, int width, int end = 0)
{
	String line = "";
	String word = "";

	/* read file */
	while (fin >> word) {
		if (line.length() + word.length() + 1 <= width) {
			if (line == "") line = word;
			else 
				line = line + " " + word;
		}
		else {
			line = line.justify(width);
			for (int i = 0; i < end - width; i++) 
				fout << " ";
			fout << line << std::endl;
			line = word;
		}
	}

	/* last line */
	for (int i = 0; i < end - width; i++) 
		fout << " ";
	fout << line << std::endl;
}

int main(int argc, char **argv)
{
	if (argc < 4) {
		cout << "Usage : " << argv[0] 
			<< " <start column> <end column> <src file> [dest file]\n";
		return -1;
	}

	/* open files */
	ifstream fin(argv[3]);
	assert(!fin.fail());

	ostream *fout;
	if (argc == 5) {
		fout = new ofstream(argv[4]);
		assert(!fout->fail());
	}
	else
		fout = &cout;

	/* basic check */
	int start = atoi(argv[1]);
	int end = atoi(argv[2]);
	assert(start < end && start > 0 && end <= 90);

	int width = end - start + 1;

	/* do justify */
	doJustify(fin, *fout, width, end);

	return 0;
}
