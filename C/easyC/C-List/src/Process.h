#ifndef _PROCESS_H
#define _PROCESS_H

#include <ostream>
using namespace std;

class Process
{
protected:
	int _beginAddress;
	int _endAddress;
	bool _hole;
	int _processID;

public:
	Process();
	~Process();
	Process(int processID, int begin, int end, bool hole);
	Process(const Process& p);

	Process& operator = (const Process& p);
	bool operator == (const Process& p);

	int infoBeginAddress();
	int infoEndAddress();
	bool infoHole();
	int infoProcessID();

	void setBeginAddress(int begin);
	void setEndAddress(int end);
	void setHole(bool hole);
	void setProcessID(int id);

    friend ostream& operator<<(ostream &out, Process &p);
};

#endif
