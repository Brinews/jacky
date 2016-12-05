#include "HuffTree.h"

void HuffTree::destroy(HuffNode* node) {

    if (node == NULL) {
        return;
    }

    destroy(node->c0);
    destroy(node->c1);

    delete node;
}

void HuffTree::backtrackTree(HuffNode* node, BitOutput& out) const {

    if (node == NULL) {
        return;
    }

    backtrackTree(node->p, out);

    if (node->p != NULL) {

        if (node->p->c0 == node) {
            out.writeBit(0);
        } else {
            out.writeBit(1);
        }
    }
}

HuffTree::~HuffTree() {
    destroy(root);
    root = NULL;
}

void HuffTree::build(const vector<int>& freqs) {

    std::priority_queue<HuffNode*,std::vector<HuffNode*>,HuffNodeCmp> forest;

    for (int symbol = 0; symbol < freqs.size(); ++symbol) {
        int count = freqs[symbol];

        if (count != 0) {
            HuffNode* tree = new HuffNode(count, symbol);

            leaves[symbol] = tree;

            forest.push(tree);
        }
    }

    while (forest.size() > 1) {

        HuffNode* c0 = forest.top();
        forest.pop();
        HuffNode* c1 = forest.top();
        forest.pop();

        HuffNode* p = new HuffNode(c0->count + c1->count, c0->symbol, c0, c1);

        c0->p = p;
        c1->p = p;

        forest.push(p);
    }

    if (forest.size() > 0) {
        root = forest.top();
    }
}

void HuffTree::encode(byte symbol, BitOutput& out) const {
    backtrackTree(leaves[symbol], out);
}

int HuffTree::decode(BitInput& in) const {

    if (root == NULL) {
        return -1;
    }

    HuffNode* node = root;

    while (node->c0 != NULL || node->c1 != NULL) {

        int bit = in.readBit();

        if (bit == -1) {
            if (node == root) {
                return -2;
            }
            return bit;
        }
        if (bit == 0) {
            node = node->c0;
        } else if (bit == 1) {
            node = node->c1;
        }
    }
    return node->symbol;
}

void HuffTree::InOrderTravese(HuffNode *p)
{
    if (p == NULL) return;

    InOrderTravese(p->c0);
    cout << p->symbol << endl;
    InOrderTravese(p->c1);
}

void HuffTree::PreOrderTravese(HuffNode *p)
{
    if (p == NULL) return;

    cout << p->symbol << endl;
    PreOrderTravese(p->c0);
    PreOrderTravese(p->c1);
}

void HuffTree::PostOrderTravese(HuffNode *p)
{
    if (p == NULL) return;

    PostOrderTravese(p->c0);
    PostOrderTravese(p->c1);
    cout << p->symbol << endl;
}

int HuffTree::TreeHeight(HuffNode *p)
{
    if (p == NULL) return 0;

    return 1 + std::max(TreeHeight(p->c0), TreeHeight(p->c1));
}

bool HuffTree::IsCompleteBTree(HuffNode *p)
{
    if (p == NULL) return true;
    if (p->c0 == NULL || p->c1 != NULL) return false;
    if (p->c1 == NULL || p->c0 != NULL) return false;

    return IsCompleteBTree(p->c0) && IsCompleteBTree(p->c1);
}

void HuffTree::SwapChild(HuffNode *p)
{
    if (p != NULL) {
        HuffNode *t = p->c0;
        p->c0 = p->c1;
        p->c1 = t;

        SwapChild(p->c0);
        SwapChild(p->c1);
    }
}

void HuffTree::InOrderNonRecur(HuffNode *p)
{
    stack<HuffNode *> s;
    while (p != NULL || !s.empty()) {
        while (p != NULL) {
            s.push(p);
            p = p->c0;
        }
        p = s.top();
        s.pop();
        cout << p->symbol << endl;
        p = p->c1;
    }

}

void HuffTree::PreOrderNonRecur(HuffNode *p)
{
    stack<HuffNode *> s;
    while (p != NULL || !s.empty()) {
        while (p != NULL) {
            cout << p->symbol << endl;
            s.push(p);
            p = p->c0;
        }
        p = s.top();
        s.pop();
        p = p->c1;
    }
}

void HuffTree::PostOrderNonRecur(HuffNode *root)
{
    if (!root) return;
    stack<HuffNode*> s;
    s.push(root);
    HuffNode *prev = NULL;
    while (!s.empty()) {
        HuffNode *curr= s.top();
        if (!prev || prev->c0 == curr|| prev->c1 == curr) {
            if (curr->c0)
                s.push(curr->c0);
            else if (curr->c1)
                s.push(curr->c1);
        } else if (curr->c0 == prev) {
            if (curr->c1)
                s.push(curr->c1);
        } else {
            cout << curr->symbol << endl;
            s.pop();
        }
        prev = curr;
    }
}

bool HuffTree::DeleteChild(byte symbol)
{
    if (root == NULL) //makes sure tree is not empty
        return false;

    HuffNode *p = root, *parent = NULL;
    while (p != NULL) {
        if (p->symbol == symbol) break;
        else if (p->symbol < symbol) {
            parent = p;
            p = p->c1;
        } else {
            parent = p;
            p = p->c0;
        }
    }

    if (p == NULL) return false; /* no found */

    if (p->c0 == NULL && p->c1 == NULL) {
        if (p == root) {
            root = NULL;
        } else {
            if (parent->c0 == p) parent->c0 = NULL;
            else parent->c1 = NULL;
        }
        delete p;
        p = NULL;
    } else if (p->c0 == NULL) {
        if (p == root) {
            root = p->c1;
        } else if (parent->c0 == p) {
            parent->c0 = p->c1;
        } else {
            parent->c1 = p->c1;
        }
        delete p;
        p = NULL;
    } else if (p->c1 == NULL) {
        if (p == root) {
            root = p->c0;
        } else if (parent->c0 == p) {
            parent->c0 = p->c0;
        } else {
            parent->c1 = p->c0;
        }
        delete p;
        p = NULL;

    } else {
        HuffNode *next = removeNext(p);
        p->symbol = next->symbol;
    }

    return true;

}

HuffNode *HuffTree::removeNext(HuffNode *p)
{
    HuffNode *toRemove;
    HuffNode *parent = p->c0;
    toRemove = parent->c1;

    if (toRemove == NULL) {
        p->c0 = parent->c0;
        return parent;
    } else {
        while (toRemove->c1 != NULL) {
            parent = toRemove;
            toRemove = toRemove->c1;
        }

        parent->c1 = NULL;
        return toRemove;
    }
}
