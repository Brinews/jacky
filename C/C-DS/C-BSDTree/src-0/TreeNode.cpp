//function and and class definition for TreeNode class
#include "TreeNode.h"

TreeNode::TreeNode(DBentry* _entryPtr) : 
    entryPtr(_entryPtr), left(NULL), right(NULL)
{
}

TreeNode::~TreeNode()
{
    delete entryPtr; // free memory
}

/*
 * setter
 */
void TreeNode::setLeft(TreeNode* newLeft)
{
    left = newLeft;
}

void TreeNode::setRight(TreeNode* newRight)
{
    right = newRight;
}

void TreeNode::setEntry(DBentry *p)
{
    entryPtr = p;
}

/*
 * getter
 */
TreeNode* TreeNode::getLeft() const
{
    return left;
}

TreeNode* TreeNode::getRight() const
{
    return right;
}

DBentry* TreeNode::getEntry() const
{
    return entryPtr;
}

/*
 * helper
 */
DBentry* TreeNode::maximum()
{
    if (right == NULL) 
        return entryPtr;
    else
        return right->maximum();
}

bool TreeNode::remove(string name, TreeNode*& cur)
{
    if (name < entryPtr->getName()){ 
        if (left != NULL) return left->remove(name, left);
    }
    if (name > entryPtr->getName()){
        if (right != NULL) return right->remove(name, right);
    }

    if (cur->getEntry()->getName() == name){
        if ((left == NULL) && (right == NULL)){
            cur = NULL;
            delete this;
            return true;
        } else if ((left == NULL) && (right != NULL)){
            cur = right;
            right = NULL;
            delete this;
            return true;
        } else if ((left != NULL) && (right == NULL)){
            cur = left;
            left = NULL;
            delete this;
            return true;
        } else {
            DBentry* tmp = left->maximum();
            left->remove(tmp->getName(), left);
            entryPtr = tmp;
            return true;
        }
    }
    else
        return false;
}
