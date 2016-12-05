#include "HuffTree.h"

#include <iostream>

using namespace std;

int main()
{
    HuffTree ht;

    vector<int> freq;
    for (int i = 0; i < 48; i++) freq.push_back(0);
    freq.push_back(19);
    freq.push_back(10);
    freq.push_back(9);
    freq.push_back(1);
    freq.push_back(8);
    freq.push_back(15);

    ht.build(freq);
    cout << "InOrder:\n";
    ht.InOrderTravese(ht.getRoot());
    cout << "PreOrder:\n";
    ht.PreOrderTravese(ht.getRoot());
    cout << "PostOrder:\n";
    ht.PostOrderTravese(ht.getRoot());

    cout << "Height = " <<ht.TreeHeight(ht.getRoot()) << endl;
    ht.SwapChild(ht.getRoot());
    cout << "InOrder:\n";
    ht.InOrderTravese(ht.getRoot());
    cout << "InOrder - Non Recur:\n";
    ht.InOrderNonRecur(ht.getRoot());

    return 0;
}
