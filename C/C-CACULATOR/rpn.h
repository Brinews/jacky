#ifndef _RPN_H
#define _RPN_H

#include <ctype.h>
#include <stack>
#include <vector>
#include <string>

//template<typename T>
class tokenstream {
public:
	tokenstream() {}
	virtual bool process(const std::string& input,
			std::vector<std::string>& output) = 0;
};

class tokenifier : public tokenstream {
public:
	tokenifier() {}
	virtual bool process(const std::string& input1,
			std::vector<std::string>& output)
	{
		//loop over the characters in input and put the strings in output

		std::string input = input1;
		std::string token;
		char c;

		if (input.at(0) == '\'' || input.at(0) == '"') 
			input = input.substr(1);
		if (input.at(input.size()-1) == '\''
			|| input.at(input.size()-1) == '"') 
			input = input.substr(0, input.size() - 1);

		int len = input.size();

		for (int i = 0; i < len; i++)
		{
			c = input[i];
			token = "";

			if (isalpha(c))
			{
				do {
					token += c;
					i++;
				} while (i < len && isalpha(c=input[i]));

				if (i != len)
					i--;

				if (token.compare("sqrt") != 0 
					&& token.compare("pi") != 0
						&& token.compare( "cos") != 0)
					return false;
			}
			else if (isdigit(c))
			{
				do {
					token += c;
					i++;
				} while (i < len && (isdigit(c=input[i]) || c == '.'));

				if (i != len)
					i--;
			}
			else if (c == '(')
			{
				token = "(";
			}
			else if (c == ')')
			{
				token = ")";
			}
			else if (c == '+')
			{
				token = "+";
			}
			else if (c == '-')
			{
				token = "-";
			}
			else if (c == '*')
			{
				token = "*";
			}
			else if (c == '/')
			{
				token = "/";
			}
			else if (c == '%')
			{
				token = "%";
			}
			else if (c == ' ')
			{
				do {
					i++;
				} while (i < len && (c=input[i]) == ' ');

				if (i != len) i--;
			}
			else
			{
				return false;
			}

			if (token.compare("") != 0)
			{
				if (token.compare("pi") == 0)
				{
					//token = "(22/7)";
					output.push_back("(");
					output.push_back("22");
					output.push_back("/");
					output.push_back("7");
					output.push_back(")");
				}
				else
					output.push_back(token);
			}
		}

		return true;
	}
};

class infix2rpn : public tokenstream {
private:
	tokenstream *tokens;
public:
	infix2rpn(tokenstream *_tokens) : tokens(_tokens) {}
	//tokenifier(tokenstream *_tokens) : tokens(_tokens) {}
	virtual bool process(const std::string& input, 
			std::vector<std::string>& output)
	{
		// call the underlying tokenstream object
		std::vector<std::string> infixtokens;
		if (!tokens->process(input, infixtokens))
			return false;

		return shunting_yard(infixtokens, output);
	}

	bool isoperator(std::string s)
	{
		char str = s.at(0);
		if (str == '+' || str == '-'
				|| str == '*' || str == '/')
			return true;

		return false;
	}

	bool isprior(std::string pstr, std::string qstr)
	{
		char p, q;
		p = pstr.at(0);
		q = qstr.at(0);

		if (p == '*' || p == '/')
			return true;

		if ((p == '+' || p == '-') 
			&& (q == '+' || q == '-'))
			return true;

		return false;
	}

	bool shunting_yard(const std::vector<std::string> &input,
			std::vector<std::string>& output)
	{
		// do the shunting yerd algorithm
		// take tokens from input and put the results in output
		// return true/false to indicate errors

		std::stack<std::string> optor;

		std::string token, top;
		for (unsigned int i = 0; i < input.size(); i++)
		{
			token = input[i];
			char c = token.at(0);

			if (isdigit(c)) /* operand */
			{
				output.push_back(token);
			}
			else if(isalpha(c)) /* function name */
			{
				optor.push(token);
			}
			else if (c == ',')
			{
				while (!optor.empty() && optor.top().compare("(")!=0)
				{
					output.push_back(optor.top());
					optor.pop();
				}

				if (optor.top().compare("(") != 0) return false;
			}
			else if(c == '+' || c == '-' 
					|| c == '*' || c == '/')
			{
				while(!optor.empty() && isoperator(optor.top())
						&& isprior(optor.top(), token))
				{
					output.push_back(optor.top());
					optor.pop();
				}

				optor.push(token);
			}
			else if (c == '(')
			{
				optor.push(token);
			}
			else if (c == ')')
			{
				while (!optor.empty() && optor.top().compare("(") != 0)
				{
					output.push_back(optor.top());
					optor.pop();
				}

				if (optor.top().compare("(") != 0)
				{
					return false;
				}

				optor.pop(); /* pop ( */

				if (!optor.empty() && isalpha(optor.top().at(0)))
				{
					output.push_back(optor.top());
					optor.pop();
				}
			}
		}

		while (!optor.empty())
		{
			if (optor.top().compare("(") == 0) {
				return false;
			}

			output.push_back(optor.top());
			optor.pop();
		}

		return true;
	}

};

template<typename T>
class rpneval : public tokenstream {
private:
	tokenstream *tokens;
public:
	rpneval(tokenstream *_tokens) : tokens(_tokens) {}

	virtual bool process(const std::string &input,
			std::vector<std::string>&output)
	{
		std::vector<std::string> postfixtokens;

		if (!tokens->process(input, postfixtokens))
			return false;

		return eval(postfixtokens, output);
	}

	bool eval(const std::vector<std::string>& input, 
		std::vector<std::string>& output)
	{
		// Evaluate the RPN token stream like before
		// The result is converted to a string and stored in output
		// Return true/false to indicate errors
		
		std::string token;
		char c;
		T a, b;

		std::stack<T> oprnd;

		for (unsigned int i = 0; i < input.size(); i++)
		{
			token = input[i];
			c = token.at(0);

			if (isdigit(c))
			{
				oprnd.push(token);
			}
			else
			{
				if (token == "sqrt")
				{
					a = oprnd.top();
					oprnd.pop();
					a = sqrt(a);
					oprnd.push(a);
				}
				else if (token == "cos")
				{
					a = oprnd.top();
					oprnd.pop();
					a = cos(a);
					oprnd.push(a);
				}
				else if (c == '+')
				{
					a = oprnd.top();
					oprnd.pop();
					b = oprnd.top();
					oprnd.pop();
					a = a + b;
					oprnd.push(a);
				}
				else if (c == '-')
				{
					a = oprnd.top();
					oprnd.pop();
					b = oprnd.top();
					oprnd.pop();
					a = b - a;
					oprnd.push(a);
				}
				else if (c == '*')
				{
					a = oprnd.top();
					oprnd.pop();
					b = oprnd.top();
					oprnd.pop();
					a = a * b;
					oprnd.push(a);
				}
				else if (c == '/')
				{
					a = oprnd.top();
					oprnd.pop();
					b = oprnd.top();
					oprnd.pop();
					a = b / a;
					oprnd.push(a);
				}
			}
		}

		if (oprnd.size() != 1)
		{
			return false;
		}

		a = oprnd.top();
		output.push_back(a.tostring());

		return true;
	}
};

#endif


