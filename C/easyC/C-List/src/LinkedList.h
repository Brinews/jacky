#ifndef _HEADER_H
#define _HEADER_H

#include <ostream>

using namespace std;

class Exception{};
class LinkedListBounds : public Exception {};
class LinkedListMemory : public Exception {};
class LinkedListNotFound : public Exception {};
class LinkedListAttachToEmpty : public Exception {};

template <class DT>
class LinkedList
{
protected:
	DT* _info;
	LinkedList* _next;

public:
	LinkedList();
	~LinkedList();
    LinkedList(const DT& dt, LinkedList<DT> *next);
	LinkedList(const LinkedList<DT>& ll);

	void copy(const LinkedList<DT>& ll);
	void operator=(const LinkedList<DT>& ll);

	bool isEmpty();
	DT& info();
	int size();
	DT& infoAt(int position);

	void add(const DT& object);
	void insertAt(const DT& newObj, int position);
	LinkedList<DT>* setNext(LinkedList<DT>* next);

	DT& find(const DT& key);

	DT remove();
	DT removeAt(int position);

    template<class U>
    friend ostream& operator<<(ostream& out, LinkedList<U>& ll);
};

#include "LinkedList.cpp"

#endif
