#ifndef AVLTREE_H
#define AVLTREE_H

#include "Data.h"
#include <queue>

template <typename T>
class AVLTree {
private:
	struct AVLNode {
		AVLNode* leftChild;
		AVLNode* rightChild;
		T* data;
		int duplicates; // used if there are duplicate values in the tree
			// instead of changing rotation rules
		int height;
		AVLNode () :	// default constructor
			leftChild {nullptr},
			rightChild {nullptr},
			data {nullptr},
			duplicates {0},
			height {0} {};
		~AVLNode () = default;
		AVLNode (T& value) :
			leftChild {nullptr},
			rightChild {nullptr},
			duplicates {0},
			height {0} {
				data = new T{value};
			};
		AVLNode (T&& value):
			leftChild {nullptr},
			rightChild {nullptr},
			duplicates {0},
			height {0} {
				data = new T{value};
			}
		AVLNode (T& value, AVLNode* left, AVLNode* right) :
			leftChild {left},
			rightChild {right},
			duplicates {0},
			height {0} {
				data = new T{value};
			};
		AVLNode (T&& value, AVLNode* left, AVLNode* right) :
			leftChild {left},
			rightChild {right},
			duplicates {0},
			height {0} {
				data = new T{value};
			}
	};

	AVLNode* root;


	// accessors -------------------------------------------------------------

	// will return the height of a given AVLNode*. Look at the definition for
	// height. -1 if the tree is empty, or max height of children + 1.
	// Must use recursion, since it counts leaves-up and we start traversals
	// at root.
	int getHeight(AVLNode* node) const {
		// CODE HERE
        if (node == nullptr) return -1;
        return 1 + std::max(getHeight(node->leftChild), getHeight(node->rightChild));
		//return 0; // PLACEHOLDER FOR COMPILATION
	}

	// returns the depth from the current subtree (node is subroot)
	// must use recursion.
	int getDepthAux(const T& value, AVLNode* node) const {
		// CODE HERE
        if (node == nullptr) return -1;
        if (node != nullptr && *node->data == value) return 0;

        if (getDepthAux(value, node->leftChild) >= 0) {
            return 1+getDepthAux(value, node->leftChild);
        } else {
            return 1+getDepthAux(value, node->rightChild);
        }

		//return 0; // PLACEHOLDER
	}

	// driver function for getDepthAux(T&,AVLNode*), which does the recursion.
	// getDepth(T&,AVLNode*) does an extra check for node not found in tree.
	int getDepth(const T& value, AVLNode* node) const {
		if (!findNode(value, node)){
			return -1;  // return -1 if node does not exist in tree
		} else {
			return getDepthAux(value, node);
		}
	}

	// returns the AVLNode* that points to the node containing the
	// parameter value in its data member.
	// the node parameter is the root of the current subtree.
	// Must use recursion.
	AVLNode* findNode(const T& value, AVLNode* node) const {
		// CODE HERE
        if (node == nullptr) return nullptr;
        if (*node->data == value) return node;

        AVLNode *ret = findNode(value, node->leftChild);

        if (ret != nullptr) return ret;
        else return findNode(value, node->rightChild);

		//return nullptr; // PLACEHOLDER
	}

	// returns the AVLNode* that points to the node containing the
	// parameter value in its data member.
	AVLNode* findNode(const T& value) const {
		return findNode(value, root);
	}

	// method to clone a subtree and return it.
	AVLNode* clone (AVLNode* node) const {
		if (!node){
			return nullptr;
		} else {
			AVLNode* temp = new AVLNode (*node->data,
								clone(node->leftChild),
								clone(node->rightChild));
			temp->duplicates = node->duplicates;
			temp->height = getHeight(node);
			return temp;
		}
	}

	// Possibly several functions to be used by printing traversal functions
	// (below). These functions may need to know what the last leaf in a
	// subtree is to print nicely (by my standards, anyway).
		// CODE HERE
    void printNode(AVLNode *node) const {
        std::cout << *node->data << ", ";
    }

	// should print the tree in a preorder traversal
	void printPreorder(AVLNode* node) const {
		// CODE HERE
        if (node != nullptr) {
            printNode(node);
            printPreorder(node->leftChild);
            printPreorder(node->rightChild);
        }
	}

	// should print the tree in an inorder traversal
	void printInorder(AVLNode* node) const {
		// CODE HERE
        if (node != nullptr) {
            printInorder(node->leftChild);
            printNode(node);
            printInorder(node->rightChild);
        }
	}
	// should print the tree in a postorder traversal
	void printPostorder(AVLNode* node) const {
		// CODE HERE
        if (node != nullptr) {
            printPostorder(node->leftChild);
            printPostorder(node->rightChild);
            printNode(node);
        }
	}

	// mutators ------------------------------------------------------------

	// inserts a new value into the given subtree with node as the root.
	// If the value already exists, just incrememnt the duplicates counter.
	// Else, create memory for it and place pointers appropriately.
	// Must use recursion.
	void insert(T& value, AVLNode* & node){
		// CODE HERE
        if (node == nullptr)
            node = new AVLNode(value);
        else if (value.compare(*node->data) < 0) {
            insert(value, node->leftChild);
            if (getHeight(node->leftChild) - getHeight(node->rightChild) == 2) {
                if (value.compare(*node->leftChild->data) < 0)
                    rotateLeft(node);
                else
                    rotateDoubleLeft(node);
            } 
        }
        else if (value.compare(*node->data) > 0) {
            insert(value, node->rightChild);
            if (getHeight(node->rightChild)-getHeight(node->leftChild) == 2) {
                if (value.compare(*(node->rightChild->data)) > 0)
                    rotateRight(node);
                else
                    rotateDoubleRight(node);
            }
        }
        else {
            node->duplicates++;
        }
	}

	// will balance the tree with node as the offending root, like
	// alpha in our class discussions. Should call onf of the rotate functions.
	// Don't forget to set the height at the end!
	void balance(AVLNode* & node){
		// CODE HERE
        if (node == nullptr) return;

        int bal = getHeight(node->rightChild) - getHeight(node->leftChild);
        if (bal == -2) {
            if (getHeight(node->leftChild->leftChild) >=
                    getHeight(node->leftChild->rightChild))
                rotateRight(node);
            else
                rotateDoubleRight(node);
        } else if (bal == 2) {
            if (getHeight(node->rightChild->rightChild) >=
                    getHeight(node->rightChild->leftChild))
                rotateLeft(node);
            else
                rotateDoubleLeft(node);
        } else {
            return;
        }

        balance(node->leftChild);
        balance(node->rightChild);
	}

	// Rotate binary tree node with left child, i.e. a single rotation
	// for case 1. Update the heights, and set new root.
	void rotateLeft(AVLNode*& node){
		// CODE HERE
        AVLNode *rn = node->leftChild;
        node->leftChild = rn->rightChild;
        rn->rightChild = node;
        node->height = std::max( getHeight( node->leftChild ), 
                                    getHeight( node->rightChild ) ) + 1;
        rn->height = std::max( getHeight( rn->leftChild ), node->height ) + 1;
        node = rn;
	}

	// Rotate binary tree node with right child, i.e. a single rotation
	// for case 4. Update the heights, and set new root.
	void rotateRight(AVLNode*& node){
		// CODE HERE
        AVLNode *rn = node->rightChild;
        node->rightChild = rn->leftChild;
        rn->leftChild = node;
        node->height = std::max( getHeight( node->leftChild ), 
                                getHeight( node->rightChild ) ) + 1;
        rn->height = std::max( getHeight( rn->rightChild ), node->height ) + 1;
        node = rn;
	}

	// Double rotate binary tree node: first left child with its right
	// child, then subroot with its new left child (was grandchild previously).
	// I.e. rotation case 2. Update the heights, and set new root.
	void rotateDoubleLeft(AVLNode*& node){
		// CODE HERE
        rotateRight(node->leftChild);
        rotateLeft(node);
	}

	// Double rotate binary tree node: first left child with its right
	// child, then subroot with its new left child (was grandchild previously).
	// I.e. rotation case 2. Update the heights, and set new root.
	void rotateDoubleRight(AVLNode*& node){
		// CODE HERE
        rotateLeft(node->rightChild);
        rotateRight(node);
	}

	// removes a given value from the tree. If the Node containing the value
	// has duplicates, decrement the duplicates. Else deallocate the memory and
	// recursively call remove to fix the tree, as discussed in class.
	void remove(T& value, AVLNode*& node){
		// CODE HERE
        if (node == nullptr) return;

        if (value.compare(*node->data) == 0) {
            if (node->duplicates > 0) {
                node->duplicates--;
            } else {
                if (node->leftChild == nullptr 
                        && node->rightChild == nullptr) {
                    delete node->data;
                    delete node;
                    node = nullptr;
                } else if (node->leftChild != nullptr) {
                    AVLNode *father = node;
                    AVLNode *p = node->leftChild;
                    while (p->rightChild != nullptr) {
                        father = p;
                        p = p->rightChild;
                    }
                    delete node->data;
                    node->data = p->data;
                    AVLNode *del = p;
                    if (p->leftChild != nullptr) {
                        del = p->leftChild;
                        p->leftChild = del->leftChild;
                        p->rightChild = del->rightChild;
                        p->data = del->data;
                        p->height = del->height;
                        p->duplicates = del->duplicates;
                    }
                    del->data = nullptr;
                    del->leftChild = nullptr;
                    del->rightChild = nullptr;
                    if (del == p) {
                        if (father == node) father->leftChild = nullptr;
                        else father->rightChild = nullptr;
                    }
                    delete del;
                } else if (node->rightChild != nullptr) {
                    AVLNode *father = node;
                    AVLNode *p = node->rightChild;
                    while (p->leftChild != nullptr) {
                        father = p;
                        p = p->leftChild;
                    }
                    delete node->data;
                    node->data = p->data;
                    AVLNode *del = p;
                    if (p->rightChild != nullptr) {
                        del = p->rightChild;
                        p->leftChild = del->leftChild;
                        p->rightChild = del->rightChild;
                        p->data = del->data;
                        p->height = del->height;
                        p->duplicates = del->duplicates;
                    }
                    del->data = nullptr;
                    del->leftChild = nullptr;
                    del->rightChild = nullptr;

                    if (del == p) {
                        if (father == node) father->rightChild = nullptr;
                        else father->leftChild = nullptr;
                    }
                    delete del;
                }
            }
        } else if (value.compare(*(node->data)) < 0) {
            remove(value, node->leftChild);
        } else if (value.compare(*(node->data)) > 0) {
            remove(value, node->rightChild);
        }
	}

	// private function to recursively empty
	void empty(AVLNode* node){
		// CODE HERE
        if (node != nullptr) {
            empty(node->leftChild);
            empty(node->rightChild);

            delete node->data;
            node->height = 0;
            node->leftChild = nullptr;
            node->rightChild = nullptr;

            delete node;
            node = nullptr;
        }

        root = nullptr;
	}

public:
	AVLTree():
		root {nullptr} {};
	~AVLTree(){
		empty();
	}
	// copy constructor: should copy all of the data from the other tree
	// into this tree.
	AVLTree(const AVLTree<T>& other){
		root = clone(other.root);
	}

	// accessors --------------------------------------------------------

	int getHeight() const {
		return getHeight(root);
	}

	// searches the tree for a value. If it is found, the height
	// is returned. If not, then -1 is returned.
	int getHeight(const T& value) const {
		AVLNode* node = findNode(value);
		return node ? node->height : -1; // ternary operator
	}

	// returns the depth for the whole tree. should be 0 if the
	// tree is nonempty, and -1 if it is empty.
	int getDepth() const {
		if (root){
			return 0;
		} else {
			return -1;
		}
	}
	// returns the depth for a given value.
	// should be -1 if tree is empty, or the number of steps
	// down from root node if not.
	int getDepth(T& value) const {
		if (!root){
			return -1;
		} else {
			return getDepth(value, root);
		}
	}

	// will return the balance factor of a value in the tree.
	// if the value does not exist, return -128 (the lowest value for
	// a 1-byte char). If it does exist, return the balance factor.
	char getBalanceFactor(T& value) const {
		// CODE HERE

		return 0; // PLACEHOLDER FOR COMPILATION
	}

	// driver function to call the private preorder traversal
	void printPreorder(){
		std::cout << "[";
		printPreorder(root);
		std::cout << "]" << std::endl;
	}

	// driver function to call the private preorder traversal
	void printInorder(){
		std::cout << "[";
		printInorder(root);
		std::cout << "]" << std::endl;
	}

	// driver function to call the private preorder traversal
	void printPostorder(){
		std::cout << "[";
		printPostorder(root);
		std::cout << "]" << std::endl;
	}

	// should print the tree in a level-order traversal (NOT driver function)
	void printLevelOrder(){
		// CODE HERE
        std::queue<AVLNode *> Q;
        Q.push(root);

		std::cout << "[";
        while (!Q.empty()) {
            AVLNode *t = Q.front();
            printNode(t);
            Q.pop();
            if (t->leftChild != nullptr) Q.push(t->leftChild);
            if (t->rightChild != nullptr) Q.push(t->rightChild);
        }
		std::cout << "]" << std::endl;
	}

	// mutators -----------------------------------------------------

	// call private balance(1) on tree
	void balance(){
		balance(root);
	}

	// calls private remove function to remove starting at root
	void remove(T& value){
		remove(value, root);
	}
	void remove(T&& value){
		T temp = T{value};
		remove(temp);
	}

	// driver function for emptying the tree, since there is no public access
	// to root of tree (as many other functions do in this file)
	void empty(){
		// CODE HERE
        empty(root);
	}

	// calls private insert function to insert starting at root
	void insert(T& value){
		insert(value, root);
	}
	void insert(T&& value){
		T temp = T{value};
		insert(temp);
	}

};



#endif
