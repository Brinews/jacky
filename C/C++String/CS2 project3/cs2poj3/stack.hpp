#ifndef _STACK_H
#define _STACK_H

#include "string.hpp"
#include <iostream>

template <typename T>
class Stack {
private:
	struct Node {
		T value;
		Node *next;
		Node(T d, Node *n) {
			value = d;
			next = n;
		}
	};

	int size;
	Node *top;

public:
				Stack			();
				Stack			(const Stack<T> &s);

	Stack<T>&	operator=		(Stack<T> s);
	T			pop				();
	void		push			(T v);
	T			peek			()						const;
	bool		isEmpty			()						const;
	void		swap			(Stack<T> &s);
	unsigned int getSize		()						const;
	String		toString		()						const;

				~Stack();

	friend
	std::ostream& operator<< (std::ostream&, Stack<T> &s);
};

#endif
