#ifndef HUFFTREE_H
#define HUFFTREE_H

#include <queue>
#include <vector>
#include <fstream>
#include <stack>

#include "HuffNode.h"
#include "BitInput.h"
#include "BitOutput.h"

class HuffTree {

    private:
        HuffNode* root;
        vector<HuffNode*> leaves;
        void destroy(HuffNode *node);
        void backtrackTree(HuffNode *node, BitOutput &out) const;

    public:
        explicit HuffTree() : root(0) {
            leaves = vector<HuffNode*>(256, (HuffNode*) 0);
        }

        ~HuffTree();

        void build(const vector<int>& freqs);

        void encode(byte symbol, BitOutput& out) const;
        int decode(BitInput& in) const;
        
        /* binary tree operations */

        HuffNode *getRoot() { return root; }

        void InOrderTravese(HuffNode *p);
        void PreOrderTravese(HuffNode *p);
        void PostOrderTravese(HuffNode *p);
        int TreeHeight(HuffNode *p);
        bool IsCompleteBTree(HuffNode *p);

        void SwapChild(HuffNode *p);
        HuffNode *removeNext(HuffNode *p);
        bool DeleteChild(byte symbol);

        void InOrderNonRecur(HuffNode *p);
        void PreOrderNonRecur(HuffNode *p);
        void PostOrderNonRecur(HuffNode *p);

};

class HuffNodeCmp {
    public:
        bool operator()(HuffNode*& lhs, HuffNode*& rhs) const {
            return *lhs < *rhs;
        }
};

#endif
