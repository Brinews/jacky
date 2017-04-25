#include <iostream>
#include "ArrayBST.h"

using namespace std;

int main()
{
    int size;
    char cmd;
    int num;
    cin >> size;

    ArrayBST<int> bst(size);

    while (cin >> cmd) {
        if (cmd == 'I') {
            cin >> num;
            bst.insert(num);
        } else if (cmd == 'F') {
            cin >> num;
            if (bst.search(num) != NULL) {
                cout << "Found" << endl;
            } else {
                cout << "Not Found" << endl;
            }
        } else if (cmd == 'R') {
            cin >> num;
            bst.remove(num);
        } else if (cmd == 'O') {
            //cout << bst << endl; /* level order */
            /*
             *  differenet output order
             */
            /*
            bst.preorder(cout);
            cout << endl;
            */
            bst.inorder(cout);
            cout << endl;
            //bst.debug(cout);
        }
    }

    return 0;
}
