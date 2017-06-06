

#include <iostream>
using namespace std;


//
// 
// The implementation for the treeNode subclass follows here!
//
// 
//                T R E E   N O D E
//
//
//


    /* Fill in this part */

template<class T>
binTree<T>::treeNode::treeNode(T d) 
{
    left = NULL;
    right = NULL;
    parent = NULL;

    data = d;
}

template <class T>
T binTree<T>::treeNode::getData()
{
    return data;
}


//
// 
// The implementation for the binTree subclass follows here!
//
// 
//                B I N    T R E E
//
//
//

template<class T>
binTree<T>::iterator::iterator(treeNode *root) : nextNode(root)
{
}

template<class T>
void binTree<T>::iterator::increment()
{
    // default pre-order
    if (nextNode->left != NULL) {
        nextNode = nextNode->left;
    } else if (iterator::nextNode->right != NULL) {
        iterator::nextNode = iterator::nextNode->right;
    } else {
        treeNode *par = iterator::nextNode->parent;
        treeNode *chi = iterator::nextNode;

        while (par != NULL &&
                (par->right == chi || par->right == NULL)) {
            chi = par;
            par = par->parent;
        }

        if (par == NULL) iterator::nextNode = NULL;
        else             iterator::nextNode = par->right;
    }

}

template<class T>
bool binTree<T>::iterator::operator==(const iterator &i2) const
{
    return nextNode == i2.nextNode;
}

template<class T>
bool binTree<T>::iterator::operator!=(const iterator &i2) const
{
    return !operator==(i2);
}

template<class T>
T& binTree<T>::iterator::operator*()
{
    return nextNode->data;
}

template<class T>
const T& binTree<T>::iterator::operator*() const
{
    return nextNode->data;
}


template<class T>
T* binTree<T>::iterator::operator->()
{
    return &nextNode->data;
}

template<class T>
typename binTree<T>::iterator& binTree<T>::iterator::operator++ ()
{
    increment();
    return *this;
}

template<class T>
typename binTree<T>::iterator binTree<T>::iterator::operator++ (int p)
{
    iterator ret = *this;
    operator++();
    return ret;
}

template<class T>
binTree<T>::preorder_iterator::preorder_iterator(treeNode *root) : iterator(root)
{
}

template<class T>
void binTree<T>::preorder_iterator::increment() {

    if (iterator::nextNode->left != NULL) {
        iterator::nextNode = iterator::nextNode->left;
    } else if (iterator::nextNode->right != NULL) {
        iterator::nextNode = iterator::nextNode->right;
    } else {
        treeNode *par = iterator::nextNode->parent;
        treeNode *chi = iterator::nextNode;

        while (par != NULL &&
                (par->right == chi || par->right == NULL)) {
            chi = par;
            par = par->parent;
        }

        if (par == NULL) iterator::nextNode = NULL;
        else             iterator::nextNode = par->right;
    }
}


template<class T>
binTree<T>::inorder_iterator::inorder_iterator(treeNode *root) : iterator(root)
{
}

template<class T>
void binTree<T>::inorder_iterator::increment() {

    if (iterator::nextNode == NULL) {
        return;
    }

    if (iterator::nextNode->right != NULL)
    // slide down the left subtree of right child
    {
        iterator::nextNode = iterator::nextNode->right;
        while (iterator::nextNode->left != NULL) {
            iterator::nextNode = iterator::nextNode->left;
        }
    }
    else {
        // back up to first ancestor not already visited
        // as long as we are parent's right child, 
        // then parent has been visited
        bool wasRightChild;
        do {
            wasRightChild = (iterator::nextNode != NULL 
                    && iterator::nextNode->parent != NULL
                    && iterator::nextNode == 
                        iterator::nextNode->parent->right);
            iterator::nextNode = iterator::nextNode->parent;
        } while (wasRightChild);
    }
}

template<class T>
binTree<T>::postorder_iterator::postorder_iterator(treeNode *root) : iterator(root)
{
}

template<class T>
void binTree<T>::postorder_iterator::increment() {
    if (iterator::nextNode == NULL) {
        return;
    }

    // back up to parent
    bool wasLeftChild = (iterator::nextNode != NULL 
            && iterator::nextNode->parent != NULL
            && iterator::nextNode ==
                iterator::nextNode->parent->left);

    iterator::nextNode = iterator::nextNode->parent;

    // if the child was not the right child, go to the right child
    if (wasLeftChild && iterator::nextNode != NULL
            && iterator::nextNode->right != NULL) {
        iterator::nextNode = iterator::nextNode->right;
        // and descend with left priority
        while (1) {
            if (iterator::nextNode != NULL 
                    && iterator::nextNode->left != NULL) {
                iterator::nextNode = iterator::nextNode->left;
            } else if (iterator::nextNode != NULL 
                    && iterator::nextNode->right != NULL) {
                iterator::nextNode = iterator::nextNode->right;
            } else {
                break;
            }
        }
    }
}


// One possible implementation of the standard constructor of the binTree ...
template <class T>
	binTree<T>::binTree():root(0) {}

template <class T>
    binTree<T>::binTree(const binTree<T> &obj) {
        //*this = obj; // need deep copy
        this->root = clone(obj.root, NULL);
    }

template <class T>
    typename binTree<T>::treeNode* binTree<T>::clone(treeNode *r, treeNode *par)  {
        if (r == NULL) return NULL;
        treeNode *p = new treeNode(r->data, 
                clone(r->left, r), clone(r->right, r));
        p->parent = par;

        return p;
    }

template <class T>
    binTree<T>::~binTree() {
        destroy(root);
    }

template <class T>
    void binTree<T>::destroy(treeNode *&root) {
        if (root != NULL) {
            destroy(root->left);
            destroy(root->right);
            delete root;
        }

        root = NULL;
    }

template <class T>
    void binTree<T>::insert(const T &val) {
        treeNode *y = NULL;
        treeNode *x = NULL;

        treeNode *insert_node = new treeNode(val);

        x = root;
        while (x != NULL) {
            y = x;
            if (insert_node->data < x->data) {
                x = x->left;
            } else {
                x = x->right;
            }
        }

        insert_node->parent = y;

        if (y == NULL) {
            root = insert_node;
        } else if (insert_node->data < y->data) {
            y->left = insert_node;
        } else {
            y->right = insert_node;
        }
    }

template <class T>
    bool binTree<T>::exists(const T &val) const {
        treeNode *t = root;
        while (t != NULL) {
            if (t->data < val) t = t->right;
            else if (t->data > val) t = t->left;
            else return true;
        }

        return false;
    }

template <class T>
    int binTree<T>::size() const {
        return size(root);
    }

template <class T>
    int binTree<T>::size(treeNode *root) const {
        if (root) return size(root->left)+size(root->right)+1;
        return 0;
    }

template <class T>
    void binTree<T>::preorder(T *array, int n) const {
        preorder(array, 0, root);
    }

template <class T>
    int binTree<T>::preorder(T *array, int n, treeNode *root) const {
        if (root != NULL) {
            array[n++] = root->data; // copy it into array
            n = preorder(array, n, root->left);
            n = preorder(array, n, root->right);
        }
        return n;
    }

template <class T>
    void binTree<T>::inorder(T *array, int n) const {
        inorder(array, 0, root);
    }

template <class T>
    int binTree<T>::inorder(T *array, int n, treeNode *root) const {
        if (root != NULL) {
            n = inorder(array, n, root->left);
            array[n++] = root->data; // copy it into array
            n = inorder(array, n, root->right);
        }
        return n;
    }

template <class T>
    void binTree<T>::postorder(T *array, int n) const {
        postorder(array, 0, root);
    }

template <class T>
    int binTree<T>::postorder(T *array, int n, treeNode *root) const {
        if (root != NULL) {
            n = postorder(array, n, root->left);
            n = postorder(array, n, root->right);
            array[n++] = root->data; // copy it into array
        }
        return n;
    }


// One possible implementation of pre_begin ...
template <class T>
  typename binTree<T>::iterator binTree<T>::begin(){
  	return iterator(root);
  }

template <class T>
  typename binTree<T>::iterator binTree<T>::end(){
  	return iterator();
  }

template <class T>
  typename binTree<T>::preorder_iterator binTree<T>::pre_begin(){
  	return preorder_iterator(root);
  }

template <class T>
  typename binTree<T>::preorder_iterator binTree<T>::pre_end(){
  	return preorder_iterator();
  }


template <class T>
  typename binTree<T>::inorder_iterator binTree<T>::in_begin(){
    treeNode *leftMost = root;
    treeNode *left = root;
    while (leftMost != NULL) {
        left = leftMost;
        leftMost = leftMost->left;
    }

  	return inorder_iterator(left);
  }

template <class T>
  typename binTree<T>::inorder_iterator binTree<T>::in_end(){
  	return inorder_iterator();
  }

template <class T>
  typename binTree<T>::postorder_iterator binTree<T>::post_begin(){
      bool finished = (root == NULL);
      treeNode *p = root;
      // descend with left priority to deepest level
      while (!finished) 
      {
          if (p != NULL && p->left != NULL) {
              p = p->left;
          } else if (p != NULL && p->right != NULL) {
              p = p->right;
          } else {
              finished = 1;
          }
      }

      return postorder_iterator(p);
  }

template <class T>
  typename binTree<T>::postorder_iterator binTree<T>::post_end(){
  	return postorder_iterator();
  }

    /* Fill in this part */



//
// 
// The implementation for the iterator subclasses follow here!
//
// 
//                   I T E R A T O R S 
//
//
//


/*
// this might not be the correct solution yet....
template <class T>
	typename binTree<T>::preorder_iterator & binTree<T>::preorder_iterator::operator++(){


        return *this;
    }
*/


    /* Fill in this part */


template <class T>
  typename binTree<T>::const_preorder_iterator binTree<T>::pre_begin() const {
  	return const_preorder_iterator(root);
  }

template <class T>
  typename binTree<T>::const_preorder_iterator binTree<T>::pre_end() const {
  	return const_preorder_iterator();
  }

template <class T>
  typename binTree<T>::const_inorder_iterator binTree<T>::in_begin() const {
  	return const_inorder_iterator(root);
  }

template <class T>
  typename binTree<T>::const_inorder_iterator binTree<T>::in_end() const {
  	return const_inorder_iterator();
  }

template <class T>
  typename binTree<T>::const_postorder_iterator binTree<T>::post_begin() const {
  	return const_postorder_iterator(root);
  }

template <class T>
  typename binTree<T>::const_postorder_iterator binTree<T>::post_end() const {
  	return const_postorder_iterator();
  }

