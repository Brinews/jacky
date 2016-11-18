//function and and class definition for TreeDB class
#include "TreeDB.h"

TreeDB::TreeDB() : root(NULL), probesCount(0) 
{
}

TreeDB::~TreeDB()
{
    clear();
}

bool TreeDB::insert(DBentry* newEntry)
{
    if (root == NULL){ 
        TreeNode *ins = new TreeNode (newEntry);
        root = ins;
        return true;
    }

    return insertFunc(newEntry, root);
}

bool TreeDB::insertFunc(DBentry* newEntry, TreeNode* cur)
{
    if (newEntry->getName() == cur->getEntry()->getName()){
        return false; // find same key
    }

    if (newEntry->getName() < cur->getEntry()->getName()){ // left
        if (cur->getLeft() == NULL){
            TreeNode* tmp = new TreeNode(newEntry);
            cur->setLeft(tmp);
            return true;
        } else {
            return insertFunc(newEntry, cur->getLeft());
        }
    } else { // right
        if (cur->getRight() == NULL){
            TreeNode* tmp = new TreeNode(newEntry);
            cur->setRight(tmp);
            return true;
        } else {
            return insertFunc(newEntry, cur->getRight());
        }
    }
}

DBentry* TreeDB::find(string name)
{
    if (root == NULL) {
        return NULL;
    }
    return findFunc(name, root, 1);
}

DBentry* TreeDB::findFunc(string name, TreeNode* cur, int probes)
{
    if (cur == NULL)
        return NULL; // no found

    if (name == cur->getEntry()->getName()){ 
        probesCount = probes;
        return cur->getEntry();
    }

    if (name < cur->getEntry()->getName()) {
        return findFunc(name, cur->getLeft(), probes + 1);
    } else {
        return findFunc(name, cur->getRight(), probes + 1);
    }
}

TreeNode *TreeDB::removeNext(TreeNode *p)
{
    TreeNode *toRemove;
    TreeNode *parent = p->getLeft();
    toRemove = parent->getRight();

    if (toRemove == NULL) {
        p->setLeft(parent->getLeft());
        return parent;
    } else {
        while (toRemove->getRight() != NULL) {
            parent = toRemove;
            toRemove = toRemove->getRight();
        }

        parent->setRight(NULL);
        return toRemove;
    }
}

bool TreeDB::remove(string name)
{
    if (root == NULL) //makes sure tree is not empty
        return false;

    return root->remove(name, root);

    /*
    TreeNode *p = root, *parent = NULL;
    while (p != NULL) {
        if (p->getEntry()->getName() == name) break;
        else if (p->getEntry()->getName() < name) {
            parent = p;
            p = p->getLeft();
        } else {
            parent = p;
            p = p->getRight();
        }
    }

    if (p == NULL) return false;

    if (p->getLeft() == NULL && p->getRight() == NULL) {
        if (p == root) {
            root = NULL;
        } else {
            if (parent->getLeft() == p) parent->setLeft(NULL);
            else parent->setRight(NULL);
        }
        delete p;
        p = NULL;
    } else if (p->getLeft() == NULL) {
        if (p == root) {
            root = p->getRight();
        } else if (parent->getLeft() == p) {
            parent->setLeft(p->getRight());
        } else {
            parent->setRight(p->getRight());
        }
        delete p;
        p = NULL;
    } else if (p->getRight() == NULL) {
        if (p == root) {
            root = p->getLeft();
        } else if (parent->getLeft() == p) {
            parent->setLeft(p->getLeft());
        } else {
            parent->setRight(p->getLeft());
        }
        delete p;
        p = NULL;

    } else {
        TreeNode *next = removeNext(p);
        delete p->getEntry();
        p->setEntry(next->getEntry());
    }

    return true;
    */
}

void TreeDB::clear()
{
    if (root != NULL)
        clearFunc(root);
    root = NULL; 
}

void TreeDB::clearFunc(TreeNode* cur){
    if (cur == NULL) return;

    clearFunc(cur->getLeft());
    clearFunc(cur->getRight());
    
    delete cur;

    return;
}

void TreeDB::printProbes() const
{
    cout << probesCount << endl; 
}

void TreeDB::countActive()
{
    activeCount = 0;
    if (root != NULL){ 
        activeFunc(root);
    }
    cout << activeCount << endl;
}

void TreeDB::activeFunc(TreeNode* cur)
{
    if (cur == NULL)
        return;

    if (cur->getEntry()->getActive()){
        activeCount += 1;
    }

    activeFunc(cur->getLeft());
    activeFunc(cur->getRight());
}

void TreeDB::printNode(ostream &out, TreeNode* cur) const
{
    if (cur->getLeft() != NULL)
        printNode(out, cur->getLeft());

    out << *(cur->getEntry());

    if (cur->getRight() != NULL)
        printNode(out, cur->getRight());
}

// You *may* choose to implement the function below to help print the
// tree. You do not have to implement this function if you do not wish to.
ostream& operator<< (ostream& out, const TreeDB& rhs)
{
    if (rhs.getRoot() != NULL) {
        rhs.printNode(out, rhs.getRoot());
    }

    return out;
}
