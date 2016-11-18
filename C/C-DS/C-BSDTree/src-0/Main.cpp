/*
 * Date: 2016-11-15
 */
#include <sstream>

#include "DBentry.h"
#include "TreeNode.h"
#include "TreeDB.h"

using namespace std;

void DoInsert(TreeDB &tree, string input1, int value, string input2)
{
    DBentry *tmp;
    bool inserted = false;

    if (input2 == "active"){
        tmp = new DBentry(input1, value, true);
        inserted = tree.insert(tmp);

        if (inserted == false) {
            delete tmp; // insert failed
        }
    } else {
        tmp = new DBentry(input1, value, false);
        inserted = tree.insert(tmp);
    }

    if (inserted == true)
        cout << "Success" << endl;
    else if (inserted == false)
        cout << "Error: entry already exists" << endl;
}

void DoUpdate(TreeDB &tree, string input1, string input2)
{
    DBentry *tmp;
    tmp = tree.find(input1);
    if (input2 == "active" && tmp != NULL){
        tmp->setActive(true);
    } else if (input2 == "inactive" && tmp != NULL)
        tmp->setActive(false);

    if (tmp == NULL)
        cout << "Error: entry does not exist" << endl;
    else
        cout << "Success" << endl;

}

int main(){
    TreeDB tree;
    string strline;
    int value;
    DBentry *tmp;

    cout.setf(ios::fixed); 

    while (!cin.eof()) {
        string cmd, input1, input2;

        cout << "> ";

        getline(cin, strline);
        stringstream strin(strline);
        strin >> cmd; // parse commands

        if (cmd == "insert") {
            strin >> input1 >> value >> input2;
            DoInsert(tree, input1, value, input2);

        } else if (cmd == "remove"){
            strin >> input1;
            bool removed = tree.remove(input1);

            if (removed == true)
                cout << "Success" << endl;
            else
                cout << "Error: entry does not exist" << endl;

        } else if (cmd == "find"){
            strin >> input1;
            tmp = tree.find(input1);
            if (tmp == NULL)
                cout << "Error: entry does not exist" << endl;
            else{
                cout << *tmp;
            }

        } else if (cmd == "printall"){
            cout << tree;

        } else if (cmd == "printprobes"){
            strin >> input1;
            tmp = tree.find(input1);

            if (tmp != NULL){
                tree.printProbes();
            } else
                cout << "Error: entry does not exist" << endl;

        } else if (cmd == "removeall"){
            tree.clear();
            cout << "Success" << endl;

        } else if (cmd == "countactive"){
            tree.countActive();

        } else if (cmd == "updatestatus"){
            strin >> input1 >> input2;
            DoUpdate(tree, input1, input2);
        }

        strin.clear();
        strin.ignore(1000,'\n');
    }

    return 0;
}
