#ifndef _COMMON_H
#define _COMMON_H

#include <string>

using namespace std;

class StackEle {
    public :
        StackEle ();
        StackEle ( bool ); // the parameter yields initial value of is_op
        virtual ~ StackEle ();
    protected :
        bool is_op ;
    public :
        bool is_Op (); // 'get ' method of is_op
        void set_Op ( bool ); // 'set ' method of is_op
};

class DataStackEle : public StackEle {
    public :
        DataStackEle ();
        DataStackEle ( double ); // yields initial value of 'data '
        virtual ~ DataStackEle ();
    private :
        double data ; // which contains the current value of the data
    public :
        double getData ();
        void setData ( double );
};

// declaration of operator types
enum OpType {op_add , op_sub , op_mul , op_div , l_paren , r_paren, 
    op_unknown};
class OpStackEle : public StackEle {
    public :
        OpStackEle();
        OpStackEle( OpType ); // constructor yielding initial value of 'type '
        virtual ~OpStackEle();
    private :
        OpType type ;
    public :
        OpType getType ();
        void setType ( OpType );
};

class Stack {
    public :
        Stack ();
        virtual ~ Stack ();
    protected :
        struct StackNode {
            StackEle * ele;
            StackNode * next ;
        };
    private :
        struct StackNode* position (int);
        int count (); // the method count becomes private
        struct StackNode *root;
        int nCount;
    public :
        void push ( StackEle *);
        StackEle * pop ();
        StackEle * top ();
        bool empty ();
};

bool token_parse(const string &expr, Stack &infix);
void infix2rpn_parse(Stack &infix, Stack &rpn);
double rpn_eval(Stack &rpn);

#endif
