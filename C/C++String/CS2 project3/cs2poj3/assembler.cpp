#include "string.hpp"
#include "stack.hpp"
#include "stack.cpp"

#include <fstream>

String infix2Assembler(String infix);

#define BUFLEN 1024
#define LEN	256

using namespace std;

String integer2String(int n);

/**
 * Converts Infix to Postfix
 */
String infix2Postfix(String infix) {

    std::stringstream strstream;
    Stack<String> st;

    String token, postfix_str;

    int prior[LEN];
    memset(prior, 0, sizeof(prior));

    prior[(int) '('] = prior[(int) ')'] = 1;
    prior[(int) '+'] = prior[(int) '-'] = 2;
    prior[(int) '*'] = prior[(int) '/'] = 3;

    strstream << infix;

    // Convert infix to postfix_str
    while (strstream >> token && token != ";") {
        if (token == "(") {
            st.push(token);

        } else if (token == "+" || token == "-" 
				|| token == "*" || token == "/") {
            while (!st.isEmpty() && 
				prior[(int) token[0]] <= prior[(int) st.peek()[0]]) {
                postfix_str = postfix_str + " " + st.pop();
            }
            st.push(token);

        } else if (token == ")") {
            while (!st.isEmpty() && st.peek() != "(") {
                postfix_str = postfix_str + " " + st.pop();
            }
            if (!st.isEmpty())
                st.pop();

        } else {
            postfix_str = postfix_str + " " + token;
        }
    }
    // Append reminding elements
    while (!st.isEmpty()) {
        postfix_str = postfix_str + " " + st.pop();
	}

    // leading spaces
    while (postfix_str.length() > 0 && postfix_str[0] == ' ')
        postfix_str = postfix_str.substr(1, postfix_str.length() - 1);

    // trailing spaces
    while (postfix_str.length() > 0 && postfix_str[postfix_str.length() - 1] == ' ')
        postfix_str = postfix_str.substr(0, postfix_str.length() - 1);

    return postfix_str;
}

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



/**
 * Converts the postfix --> assembly.
 */
int main(int argc, char **argv) 
{

    if (argc != 2 && argc != 3) {
        cout << "Usage: " << argv[0] << " postfix-file [asm-file]" 
			 << endl;
        return 0;
    }

    ifstream fin(argv[1]);
    if (!fin) {
        cout << argv[1] << " not found" << endl;
        return 0;
    }

	/* for out */
    ofstream fout;
    if (argc == 3) {
        fout.open(argv[2]);
    }

    char line[BUFLEN];

    while (fin.getline(line, BUFLEN)) {

        String infix = line;
        if (infix.length() > 0) {
            String asmstr = infix2Assembler(infix);
            if (argc == 3) {
                fout << asmstr << endl;
            } else {
                cout << asmstr << endl;
            }
        }
    }

    fin.close();

    if (argc == 3)
        fout.close();

    return 0;
}

