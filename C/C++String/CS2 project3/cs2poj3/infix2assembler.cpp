#include "infix2assembler.hpp"
#include "infix2postfix.hpp"
#include <sstream>

using namespace std;

String integer2String(int n);

/**
 * Converts an infix expression into myasm.
 */
String infix2Assembler(String infix) {
    String myasm;

	/* get postfix str */
    String postfix = infix2Postfix(infix);

    myasm = myasm + "Infix Expression: " + infix + "\n";
    myasm = myasm + "Postfix Expression: " + postfix + "\n";
    myasm = myasm + "\n";

    stringstream ss;

	/* put postfix str to stream */
    ss << postfix;

    String token;
    Stack<String> mystack;

    int temp = 1;

    while (ss >> token) {
        if (token == "+" || token == '-' 
				|| token == "*" || token == "/") {
            String right = mystack.pop();
            String left = mystack.pop();

            myasm = myasm + "   LDR  " + left + "\n";

            if (token == "+")
                myasm = myasm + "   ADD  ";
            else if (token == "-")
                myasm = myasm + "   SUB  ";
            else if (token == "*")
                myasm = myasm + "   MUL  ";
            else
                myasm = myasm + "   DIV  ";

            myasm = myasm + right + "\n";

            token = "TMP" + integer2String(temp++);

            myasm = myasm + "   STR  " + token + "\n";
        }
        mystack.push(token);
    }

    return myasm;
}

String integer2String(int n) {
    stringstream ss;

	/* read integer */
    ss << n;
    String output;
	/* write string */
    ss >> output;

    return output;
}


