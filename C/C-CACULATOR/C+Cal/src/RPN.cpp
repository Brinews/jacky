#include "common.h"

/**
 * 中缀转后缀
 * infix expression to postfix expression
 */
void infix2rpn_parse(Stack &inf_stack, Stack &pst_stack)
{
    /* 辅助栈 */
    Stack op_stack;
    OpStackEle *op;

    while (!inf_stack.empty()) {
        // fetch the top element in inf_stack
        StackEle *ele = inf_stack.pop () ; 

        // oprands , directly push into pst_stack
        if (!ele->is_Op()) // 操作数
            pst_stack.push(ele); 
        else {// 操作符
            switch (((OpStackEle *)ele)->getType()) {
                case op_add:
                case op_sub:
                    op = (OpStackEle *)op_stack.top();
                    while (op != NULL && (op->getType() == op_add ||
                                op->getType() == op_sub || 
                                op->getType() == op_mul ||
                                op->getType() == op_div)) {
                        pst_stack.push(op_stack.pop());
                        op = (OpStackEle *)op_stack.top();
                    } // such loop executes at most two iterations
                    op_stack.push(ele);
                    break ;
                case op_mul:
                case op_div: //需要调整运算先后顺序
                    op = (OpStackEle *)op_stack.top();
                    if (op != NULL && (op->getType() == op_mul 
                            || op->getType() == op_div)) {
                        pst_stack.push(op);
                        op_stack.pop();
                    } // why this time , we do not need a loop ?
                    op_stack.push(ele);
                    break ;
                case l_paren:
                    op_stack.push(ele);
                    break ;
                case r_paren: //括号内的运算优先
                    op = (OpStackEle *)op_stack.top();
                    while (op != NULL && op->getType() != l_paren) {
                        pst_stack.push(op);
						op_stack.pop();
                        op = (OpStackEle *)op_stack.top();
                    } // note : '( ' would not be pushed into pst_stack

					/* 释放(的空间 */
					op = (OpStackEle *) op_stack.pop();
                    delete op;

                    delete ele;
                    break ;
            } // end of switch
        } // end of while
    }

    while (!op_stack.empty())
        pst_stack.push(op_stack.pop());
}

/**
 * 后缀表达式求值
 */
double rpn_eval(Stack &rev_stack)
{
    DataStackEle *dse;
    Stack data_stack;
    double val;

    while (!rev_stack.empty()) {
        StackEle *sele = rev_stack.pop();
        if (!sele->is_Op()) // oprands , directly push into data_stack
            data_stack.push(sele);
        else // operators 操作符
        {
            OpStackEle *ele = (OpStackEle *) sele;

            // 取出两个数，然后进行计算 
            dse = (DataStackEle *) data_stack.pop();
            double data_2 = dse->getData(); 
            delete dse;
            dse = (DataStackEle *) data_stack.pop();
            double data_1 = dse->getData();
            delete dse;

            if (ele->getType() == op_add) {
                val = data_1 + data_2;
            } else if (ele->getType() == op_sub) {
                val = data_1 - data_2;
            } else if (ele->getType() == op_mul) {
                val = data_1 * data_2;
            } else if (ele->getType() == op_div) {
                val = data_1 / data_2;
            }

            data_stack.push(new DataStackEle(val));

            delete ele; //释放内存空间
        }
    }

    // 取出最后计算结果
    dse = (DataStackEle *) data_stack.pop();
    val = dse->getData();
    delete dse;

    return  val;
}
