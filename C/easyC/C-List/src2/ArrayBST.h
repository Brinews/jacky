#ifndef _ARRAYBST_H
#define _ARRAYBST_H

#include <vector>
#include <stack>
#include <ostream>
#include <iostream>

using namespace std;

template <class DT>
class ArrayBTNode {
    public:
        DT* _info;
        int _left; // the index position where left ArrayBTNode will be avaiable
        int _right; 

        ArrayBTNode();
        ArrayBTNode(DT _i, int _l, int _r);
        ~ArrayBTNode();
        bool isNull();
        int getLeft();
        int getRight();
        DT *getInfo();

        void setLeft(int l);
        void setRight(int r);

        bool operator==(const ArrayBTNode<DT> &lhs);
        bool operator!=(const ArrayBTNode<DT> &lhs);
        bool operator<(const ArrayBTNode<DT> &lhs);
        bool operator>(const ArrayBTNode<DT> &lhs);
        bool operator>=(const ArrayBTNode<DT> &lhs);
        bool operator<=(const ArrayBTNode<DT> &lhs);

        template<class BT>
        friend ostream& operator<<(ostream &out, ArrayBTNode<BT>& bt);
};

template <class DT>
class ArrayBT {
    public:
        vector<ArrayBTNode<DT>* > myBinaryTree;
        int _root; // index position of the root
        int _noNodes; // the number of nodes in the binary tree
        int _size; // the maxinum size of the vector
        stack<int> freeLocations; // STL stack

        ArrayBT();
        ArrayBT(int size);
        virtual ~ArrayBT();

        virtual bool isEmpty() ;
        virtual int Size();
        virtual ArrayBTNode<DT>* rootData();
        virtual ArrayBTNode<DT>* left(ArrayBTNode<DT> *r);
        virtual ArrayBTNode<DT>* right(ArrayBTNode<DT> *r);

};

template <class DT>
class ArrayBST : virtual public ArrayBT<DT> 
{
    public: 
        ArrayBST();
        ArrayBST(int size);
        virtual ~ArrayBST();

        // insert, remove, inorder and preorder traversals
        // other methods that should be part of any class
        void insert(DT dt);
        void remove(DT dt);

        ArrayBTNode<DT>* search(DT dt);

        void debug(ostream &out);

        void inorder_traverse(ostream &out, ArrayBTNode<DT> *r);
        void preorder_traverse(ostream &out, ArrayBTNode<DT> *r);

        void inorder(ostream& out);
        void preorder(ostream& out);

        template<class BT>
        friend ostream& operator<<(ostream &out, ArrayBST<BT>& bt);
};


#include "ArrayBST.cpp"

#endif
