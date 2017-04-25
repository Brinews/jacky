/*
 * ArrayBTNode
 */
template<class DT>
ArrayBTNode<DT>::ArrayBTNode()
{
    _info = NULL;
    _left = -1;
    _right = -1;
}

template<class DT>
ArrayBTNode<DT>::ArrayBTNode(DT _i, int _l, int _r)
{
    _info = new DT(_i);
    _left = _l;
    _right = _r;
}

template<class DT>
ArrayBTNode<DT>::~ArrayBTNode()
{
    delete _info;
    _info = NULL;
}

template<class DT>
bool ArrayBTNode<DT>::isNull()
{
    return _info == NULL;
}

template<class DT>
DT * ArrayBTNode<DT>::getInfo()
{
    return _info;
}

template<class DT>
int ArrayBTNode<DT>::getLeft()
{
    return _left;
}

template<class DT>
int ArrayBTNode<DT>::getRight()
{
    return _right;
}

template<class DT>
void ArrayBTNode<DT>::setLeft(int l)
{
    _left = l;
}

template<class DT>
void ArrayBTNode<DT>::setRight(int r)
{
    _right = r;
}


template<class DT>
bool ArrayBTNode<DT>::operator==(const ArrayBTNode<DT> &lhs)
{
    return *_info == *(lhs._info);
}

template<class DT>
bool ArrayBTNode<DT>::operator!=(const ArrayBTNode<DT> &lhs)
{
    return *_info != *(lhs._info);
}

template<class DT>
bool ArrayBTNode<DT>::operator<(const ArrayBTNode<DT> &lhs)
{
    return *_info < *(lhs._info);
}

template<class DT>
bool ArrayBTNode<DT>::operator<=(const ArrayBTNode<DT> &lhs)
{
    return *_info <= *(lhs._info);
}

template<class DT>
bool ArrayBTNode<DT>::operator>(const ArrayBTNode<DT> &lhs)
{
    return *_info > *(lhs._info);
}

template<class DT>
bool ArrayBTNode<DT>::operator>=(const ArrayBTNode<DT> &lhs)
{
    return *_info >= *(lhs._info);
}

template<class BT>
ostream& operator<<(ostream &out, ArrayBTNode<BT> &bt)
{
    out << *bt._info ;
    return out;
}

/*
 * ArrayBT
 */
template<class DT>
ArrayBT<DT>::ArrayBT()
{
    _size = 1000;
    this->_root = -1;
    this->_noNodes = 0;

    myBinaryTree.resize(_size, NULL);

    for (int i = 0; i < _size; i++) {
        this->freeLocations.push(_size-i-1);
    }
}

template<class DT>
ArrayBT<DT>::ArrayBT(int size)
{
    _size = size;
    this->_root = -1;
    this->_noNodes = 0;

    myBinaryTree.resize(_size, NULL);

    for (int i = 0; i < _size; i++) {
        this->freeLocations.push(_size-i-1);
    }
}

template<class DT>
ArrayBT<DT>::~ArrayBT()
{
    for (int i = 0; i < _size; i++) {
        ArrayBTNode<DT> * node = this->myBinaryTree.at(i);
        if (node != NULL) {
            delete node;
        }
    }
}

template<class DT>
bool ArrayBT<DT>::isEmpty()
{
    return this->_noNodes == 0;
}

template<class DT>
int ArrayBT<DT>::Size()
{
    return this->_noNodes;
}

template<class DT>
ArrayBTNode<DT>* ArrayBT<DT>::rootData()
{
    if (this->_root == -1) return NULL;
    return this->myBinaryTree.at(this->_root);
}

template<class DT>
ArrayBTNode<DT>* ArrayBT<DT>::left(ArrayBTNode<DT> *r)
{
    if (r->getLeft() != -1) return this->myBinaryTree.at(r->getLeft());
    return NULL;
}

template<class DT>
ArrayBTNode<DT>* ArrayBT<DT>::right(ArrayBTNode<DT> *r)
{
    if (r->getRight() != -1) return this->myBinaryTree.at(r->getRight());
    return NULL;
}

/*
 * ArrayBST
 */
template<class DT>
ArrayBST<DT>::ArrayBST() : ArrayBT<DT>()
{
}

template<class DT>
ArrayBST<DT>::ArrayBST(int size) : ArrayBT<DT>(size)
{
}

template<class DT>
ArrayBST<DT>::~ArrayBST()
{
}

template<class DT>
void ArrayBST<DT>::debug(ostream &out)
{
    for (int i = 0; i < this->_size; i++) {
        if (this->myBinaryTree[i] != NULL) {
            out << *(this->myBinaryTree[i]) << endl;
        }
    }
}

template<class DT>
void ArrayBST<DT>::insert(DT dt)
{
    int pos;
    ArrayBTNode<DT> *node = new ArrayBTNode<DT>(dt, -1, -1);

    if (this->freeLocations.empty()) {
        vector<ArrayBTNode<DT>*> nvec;
        nvec.resize(this->_size, NULL);
        for (int i = 0; i < this->_size; i++)
            nvec[i] = this->myBinaryTree[i];

        this->myBinaryTree.resize(this->_size*2, NULL);

        for (int i = 0; i < 2*this->_size; i++) {
            if (i >= this->_size) {
                this->freeLocations.push(2*this->_size-i-1);
            } else {
                this->myBinaryTree[i] = nvec[i];
            }
        }

        this->_size = 2*this->_size;
    }

    pos = this->freeLocations.top();
    this->freeLocations.pop();

    if (this->_root == -1) {

        this->_root = pos;
        this->myBinaryTree[pos] = node;

        return;
    }

    this->myBinaryTree[pos] = node;

    ArrayBTNode<DT> *root = this->myBinaryTree[this->_root];
    ArrayBTNode<DT> *parent;
    int direction = 0; // 0: left, 1: right

    while (root != NULL) {
        parent = root;

        if (*root >= *node) {
            root = this->left(root);
            direction = 0;
        } else {
            root = this->right(root);
            direction = 1;
        }

        //cout << " * " << *(parent->getInfo()) << endl;
    }

    if (direction == 0) {
        parent->setLeft(pos);
    } else {
        parent->setRight(pos);
    }

    this->_noNodes++;
}

template<class DT>
void ArrayBST<DT>::remove(DT dt)
{
    if (this->_root == -1) return;

    int p = this->_root;
    int parent = -1;

    while (p != -1) {
        DT *info = this->myBinaryTree[p]->getInfo();

        if (*info == dt) {
            break;
        } else if (*info > dt) {
            parent = p;
            p = this->myBinaryTree[p]->getLeft();
        } else {
            parent = p;
            p = this->myBinaryTree[p]->getRight();
        }
    }

    if (p == -1) return; /* no found */

    ArrayBTNode<DT> *delete_node = this->myBinaryTree[p];

    ArrayBTNode<DT> *y = NULL; /* to delete node */
    int chg = 0;

    if (delete_node->getLeft() == -1 || delete_node->getRight() == -1) {
        y = delete_node;
        chg = p;
    } else {
        //y = successor(delete_node);
        y = this->right(delete_node);
        chg = delete_node->getRight();

        while (this->left(y) != NULL) {
            chg = y->getLeft();
            y = this->left(y);
        }
    }

    int xpos;

    if (y->getLeft() != -1) {
        xpos = y->getLeft();
    } else {
        xpos = y->getRight();
    }

    if (parent == -1) {
        this->_root = xpos;
    } else {
        if (this->myBinaryTree[parent]->getLeft() == p) {
            this->myBinaryTree[parent]->setLeft(xpos);
        } else {
            this->myBinaryTree[parent]->setRight(xpos);
        }
    }

    if (chg != p) {
        delete_node->_info = y->_info;
    }


    this->_noNodes--;
    delete y;
    this->myBinaryTree[chg] = NULL;
}

/*
 * return NULL means no found
 */
template<class DT>
ArrayBTNode<DT>* ArrayBST<DT>::search(DT dt)
{
    if (this->_root == -1) return NULL;

    ArrayBTNode<DT> *root = this->myBinaryTree[this->_root];

    while (root != NULL) {

        DT info = *(root->getInfo());

        if (info == dt) {
            return root;
        } else if (info > dt) {
            root = this->left(root);
        } else {
            root = this->right(root);
        }
    }

    return NULL;
}

template<class DT>
void ArrayBST<DT>::inorder_traverse(ostream &out, ArrayBTNode<DT> *root)
{
    if (root == NULL || root->isNull()) return;

    inorder_traverse(out, this->left(root));
    out << *root << ",";
    inorder_traverse(out, this->right(root));
}

template<class DT>
void ArrayBST<DT>::inorder(ostream &out)
{
    ArrayBTNode<DT>* root = this->rootData();
    inorder_traverse(out, root);
}

template<class DT>
void ArrayBST<DT>::preorder_traverse(ostream &out, ArrayBTNode<DT> *root)
{
    if (root == NULL || root->isNull()) return;

    out << *root << ",";
    preorder_traverse(out, this->left(root));
    preorder_traverse(out, this->right(root));
}

template<class DT>
void ArrayBST<DT>::preorder(ostream &out)
{
    ArrayBTNode<DT>* root = this->rootData();
    preorder_traverse(out, root);
}

template<class BT>
ostream& operator<<(ostream &out, ArrayBST<BT>& bt)
{
    stack<ArrayBTNode<BT>* > s;
    ArrayBTNode<BT> *root = bt.rootData();
    if (root != NULL) s.push(root);

    while (!s.empty()) {
        ArrayBTNode<BT> *t = s.top();
        s.pop();
        out << *t << ",";
        if (bt.left(t) != NULL) s.push(bt.left(t));
        if (bt.right(t) != NULL) s.push(bt.right(t));
    }

    return out;
}
