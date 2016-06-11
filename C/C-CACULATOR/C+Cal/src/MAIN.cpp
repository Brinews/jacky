#include "common.h"
#include <iostream>

using namespace std;

int main()
{
    Stack raw_stack, inf_stack, pst_stack;

    string line;
    while (getline(cin, line)) {
        if (!token_parse(line, raw_stack)) {
			cout << "输入格式不对" << endl;
			continue;
		}

        // 栈翻转
        while (!raw_stack.empty()) inf_stack.push(raw_stack.pop());

        infix2rpn_parse(inf_stack, pst_stack);

		while (!pst_stack.empty()) raw_stack.push(pst_stack.pop());

        cout << "Result=" << rpn_eval(raw_stack) << endl;
    }

    return 0;
}
