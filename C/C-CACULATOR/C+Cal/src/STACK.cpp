#include "common.h"
    
/*
 * class StackEle functions
 * An interface
 */
StackEle::StackEle()
{
    is_op = false;
}

StackEle::StackEle(bool op)
{
    is_op = op;
}

bool StackEle::is_Op()
{
    return is_op;
}

void StackEle::set_Op(bool op)
{
    is_op = op;
}

StackEle::~StackEle()
{
}

/*
 * DataStackEle class
 * data element
 */
DataStackEle::DataStackEle()
{
    is_op = false;
    data = 0.0;
}

DataStackEle::DataStackEle(double d)
{
    is_op = false;
    data = d;
}

DataStackEle::~DataStackEle()
{
}

double DataStackEle::getData()
{
    return data;
}

void DataStackEle::setData(double d)
{
    data = d;
}

/*
 * operator element class
 * OpStackEle class
 */
OpStackEle::OpStackEle()
{
    is_op = true;
}

OpStackEle::OpStackEle(OpType ot)
{
    is_op = true;
    type = ot;
}

OpType OpStackEle::getType()
{
    return type;
}

void OpStackEle::setType(OpType ot)
{
    type = ot;
}

OpStackEle::~OpStackEle()
{
}

/*
 * stack class 
 */
Stack::Stack()
{
    root = NULL;
    nCount = 0;
}

Stack::~Stack()
{
    StackNode *sn;
    while (root) {
        sn = root->next;
        delete root;
        root = sn;
    }
    nCount = 0;
}

Stack::StackNode* Stack::position(int pos)
{
    StackNode *ret = root;
    while (pos-- && ret != NULL) {
        ret = ret->next;
    }

    return ret;
}

int Stack::count()
{
    return nCount;
}

void Stack::push(StackEle *se)
{
    StackNode *sn = new StackNode();
    sn->ele = se;
    sn->next = root;
    root = sn;

    nCount++;
}

StackEle* Stack::pop()
{
    if (empty()) return NULL;

    StackEle *ret = root->ele;
    StackNode *next = root->next;

    delete root;
    root = next;
    nCount--;

    return ret;
}

StackEle* Stack::top()
{
    if (root == NULL) return NULL;

    return root->ele;
}

bool Stack::empty()
{
    return nCount == 0;
}
