#include "Process.h"

Process::Process()
{
	int _beginAddress = 0;
	int _endAddress = 0;
	bool _hole = true;
	int _processID = 0;
}

Process::~Process()
{
}

Process::Process(const Process& p)
{
	if (this != &p)
	{
		_beginAddress = p._beginAddress;
		_endAddress = p._endAddress;
		_hole = p._hole;
		_processID = p._processID;
	}
}

Process::Process(int processID, int begin, int end, bool hole)
{
	_beginAddress = begin;
	_endAddress = end;
	_hole = hole;
	_processID = processID;
}

Process& Process::operator = (const Process& p)
{
    if (this != &p) {
		_beginAddress = p._beginAddress;
		_endAddress = p._endAddress;
		_hole = p._hole;
		_processID = p._processID;
    }

    return *this;
}

bool Process::operator == (const Process& p)
{
	if (p._processID == _processID)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int Process::infoBeginAddress()
{
	return _beginAddress;
}

int Process::infoEndAddress()
{
	return _endAddress;
}

bool Process::infoHole()
{
	return _hole;
}

int Process::infoProcessID()
{
	return _processID;
}

void Process::setBeginAddress(int begin)
{
	_beginAddress = begin;
}

void Process::setEndAddress(int end)
{
	_endAddress = end;
}

void Process::setHole(bool hole)
{
	_hole = hole;
}

void Process::setProcessID(int id)
{
	_processID = id;
}

ostream& operator<<(ostream &out, Process &p)
{
	if (&p == NULL) return out;

    if (p.infoProcessID() > 0) {
        out << "Pid:" << p.infoProcessID();
    } else {
        out << "Free Space";
    }

    out << ", Mem: [" << p.infoBeginAddress()
         << ", " << p.infoEndAddress() << "]\n";

    return out;
}
