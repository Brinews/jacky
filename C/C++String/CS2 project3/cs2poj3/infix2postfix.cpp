#include "infix2postfix.hpp"
#include <sstream>
#include <cstring>

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
