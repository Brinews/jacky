#include "string.hpp"
#include "stack.hpp"
#include "stack.cpp"

#include <sstream>
#include <cstring>
#include <fstream>

using namespace std;

#define LEN	256

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
 * Reads a set of infix expressions from a file and writes the 
 * infix and postfix expressions to another file. 
 */
int main(int argc, char **argv) {
    if (argc != 2 && argc != 3) {
        cout << "Usage: " << argv[0] << " infile [outfile]" << endl;
        return 0;
    }
    ifstream input(argv[1]);
    if (!input) {
        cout << "The input file is not found" << endl;
        return 0;
    }
    ofstream out;
    if (argc == 3) {
        out.open(argv[2]);
    }
    char line[1024];
    while (input.getline(line, 1024)) {
        String infix = line;
        if (infix.length() > 0) {
            String postfix = infix2Postfix(infix);
            if (argc == 3) {
                out << "Infix Expression: " << infix << endl;
                out << "Postfix Expression: " << postfix << endl;
            } else {
                cout << "Infix Expression: " << infix << endl;
                cout << "Postfix Expression: " << postfix << endl;
            }
        }
    }
    input.close();
    if (argc == 3)
        out.close();
    return 0;
}
