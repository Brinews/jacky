/***
 * Student Number :
 * Login ID :
 * Author :
 */ 

#include <stdio.h>
#include <queue>
#include <iostream>

using namespace std;

/***
 * ADTs: two priority_queue, one for list one, the other for two
 *
 * Algorithm: 
 *		foreach element A in queue Q1:
 *			while Q2.top > A:
 *				Q2.pop
 *			If Q2.top == A :
 *				continue;
 *			If Q2.top < A :
 *				output A(not found in Q2)
 *				found = true;
 *		If found = false
 *			output None;
 */

void find_nor_sets(priority_queue<int> Q1, priority_queue<int> Q2);
void read_two_sets(priority_queue<int> &Q1, priority_queue<int> &Q2);

void find_nor_sets(priority_queue<int> Q1, priority_queue<int> Q2)
{
	int A, oldA = 0;
	bool found = false;

	while (!Q1.empty()) {
		A = Q1.top();
		Q1.pop();

		//cout << "debug Q1:" << A << endl;

		while (!Q2.empty() && Q2.top() > A) Q2.pop();

		if (!Q2.empty() && Q2.top() == A) continue;

		if (Q2.empty() || Q2.top() < A) {

			if (A != oldA) {
				cout << A << endl;
				oldA = A;
			}

			found = true;
			return;
		}
	}

	if (found == false) {
		printf("None\n");
	}
}

/***
 * 0 for delimiter
 */
void read_two_sets(priority_queue<int> &Q1, priority_queue<int> &Q2)
{
	int element;

	/* read list one */
	do {
		cin >> element;
		if (element == 0) break;
		Q1.push(element);
	} while (!cin.eof());

	/* read list two */
	while (cin >> element)
		Q2.push(element);
}

int main(int argc, char **argv)
{
	priority_queue<int> Q1, Q2;

	cout << "StuNo: Loin ID: Name: " << endl;
	read_two_sets(Q1, Q2);

	/* find x, where x in Q1 but not in Q2 */
	find_nor_sets(Q1, Q2);

	return 0;
}
