#include <iostream>
#include <string>
#include <cassert>
#include <vector>
// local includes
#include "AVLTree.h"
#include "Data.h"

using namespace std;

void testDoubleData(){
	// testing getData
	DoubleData d1 {};
	DoubleData d2 {5};
	assert (d1.getData() == 0); // default value
	assert (d2.getData() == 5); // testing 1-arg constructor
	DoubleData d3 {d2};
	assert (d3.getData() == 5); // testing copy constructor
	DoubleData d4 {*(new DoubleData(3))};
	assert (d4.getData() == 3); // testing move constructor

	// testing setData
	double y = 7.5;
	d1.setData(y);
	assert (d1.getData() == 7.5);
	d1.setData(8.5);
	assert (d1.getData() == 8.5);

	// testing compare, should be a.compare(b) = a-b
	assert (d1.compare(d2) == 3.5);
}


void testAVLTree(){
	AVLTree<DoubleData> a {};
	std::vector<DoubleData*> v {5, new DoubleData{}};

	assert (a.getHeight() == 0 || a.getHeight() == -1);
	double counter = 0;
	std::cout << "testing insertion and single rotation cases: " << std::endl;
	for (auto& x : v){
		x = new DoubleData(counter);
		a.insert(*x);
		/* These test all the single rotations */
		if (counter == 1) {assert(a.getHeight() == 1);}
		if (counter == 2) {assert(a.getHeight() == 1);}
		if (counter == 3) {assert(a.getHeight() == 2);}
		if (counter == 4) {assert(a.getHeight() == 2);}
		counter++;
	}
	std::cout << std::endl;

	std::cout << "testing double rotation cases: " << std::endl;
	a.insert(DoubleData(2.5));
	assert(a.getHeight() == 2);
	a.insert(DoubleData(3.5));
	a.insert(DoubleData(1.5));
	a.insert(DoubleData(1.2));
	a.insert(DoubleData(1.7));
	a.insert(DoubleData(-1));
	a.insert(DoubleData(0.5));
	a.insert(DoubleData(1.3));
	a.insert(DoubleData(1.1));
	a.insert(DoubleData(1.6));
	a.insert(DoubleData(-2));
	a.insert(DoubleData(1.05));
	assert(a.getHeight() == 4);
	std::cout << std::endl;

	std::cout<<"testing traversals:"<<std::endl;
	a.printPreorder();
	a.printInorder();
	a.printPostorder();
	a.printLevelOrder();
	std::cout << std::endl;

	std::cout << "removing 2.5:" << std::endl;
	a.remove(DoubleData(2.5));
	a.printLevelOrder();
	std::cout << std::endl;

	std::cout << "removing 1.5:" << std::endl;
	a.remove(DoubleData(1.5));
	a.printPostorder();
	std::cout << std::endl;

	// // testing copy constructor
	AVLTree<DoubleData> b {a};
	cout << "should be same:" << endl;
	a.printPostorder();
	b.printPostorder();
	std::cout << std::endl;

	a.empty();
	cout << "should be empty now:" << endl;
	assert (a.getHeight() == -1 || 	// a null tree can have height -1 or 0,
			a.getHeight() == 0); 	// which is subject to preference and usage
	a.printPostorder();
	std::cout << "end of test"<< std::endl;
}



int main (int argc, char* argv []){
	testDoubleData();
	testAVLTree();
	std::cout << "end of main"<< std::endl;
}
