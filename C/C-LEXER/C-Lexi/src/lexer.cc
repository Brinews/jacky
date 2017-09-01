/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 */
#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>

#include "lexer.h"
#include "inputbuf.h"

using namespace std;

string reserved[] = { "END_OF_FILE",
    "IF", "WHILE", "DO", "THEN", "PRINT",
    "PLUS", "MINUS", "DIV", "MULT",
    "EQUAL", "COLON", "COMMA", "SEMICOLON",
    "LBRAC", "RBRAC", "LPAREN", "RPAREN",
    "NOTEQUAL", "GREATER", "LESS", "LTEQ", "GTEQ",
    "DOT", "NUM", "ID", "ERROR", "REALNUM", "BASE08NUM", "BASE16NUM" 
		// TODO: Add labels for new token types here (as string)
};

#define KEYWORDS_COUNT 5
string keyword[] = { "IF", "WHILE", "DO", "THEN", "PRINT" };

void Token::Print()
{
    cout << "{" << this->lexeme << " , "
         << reserved[(int) this->token_type] << " , "
         << this->line_no << "}\n";
}

LexicalAnalyzer::LexicalAnalyzer()
{
    this->line_no = 1;
    tmp.lexeme = "";
    tmp.line_no = 1;
    tmp.token_type = ERROR;
}

bool LexicalAnalyzer::SkipSpace()
{
    char c;
    bool space_encountered = false;

    input.GetChar(c);
    line_no += (c == '\n');

    while (!input.EndOfInput() && isspace(c)) {
        space_encountered = true;
        input.GetChar(c);
        line_no += (c == '\n');
    }

    if (!input.EndOfInput()) {
        input.UngetChar(c);
    }
    return space_encountered;
}

bool LexicalAnalyzer::IsKeyword(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return true;
        }
    }
    return false;
}

TokenType LexicalAnalyzer::FindKeywordIndex(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return (TokenType) (i + 1);
        }
    }
    return ERROR;
}

bool LexicalAnalyzer::ZeroReal(string str)
{
	for (int i = 0; i < str.length(); i++) {
		if (str.at(i) != '0' && str.at(i) != '.') return false;
	}

	return true;
}

bool LexicalAnalyzer::Base8Num(string str)
{
	for (int i = 0; i < str.length(); i++) {
		if (str.at(i) > '7') return false;
	}

	return true;
}

Token LexicalAnalyzer::ScanNumber()
{
    char c;

    input.GetChar(c);
    if (isdigit(c)) {
        if (c == '0') {
            tmp.lexeme = "0";
        } else {
            tmp.lexeme = "";
            while (!input.EndOfInput() && isdigit(c)) {
                tmp.lexeme += c;
                input.GetChar(c);
            }
            if (!input.EndOfInput()) {
                input.UngetChar(c);
            }
        }

        // TODO: You can check for REALNUM, BASE08NUM and BASE16NUM here!
		if (!input.EndOfInput()) {
			input.GetChar(c);

			// real-num
			if (c == '.') {
				if (!input.EndOfInput()) {
					input.GetChar(c);
					if (isdigit(c)) {
						tmp.lexeme += '.';
						while (!input.EndOfInput() && isdigit(c)) {
							tmp.lexeme += c;
							input.GetChar(c);
						}
						if (!input.EndOfInput()) {
							input.UngetChar(c);
						}

						// exception: 0.0..0
						if (!ZeroReal(tmp.lexeme)) {
							tmp.token_type = REALNUM;
							tmp.line_no = line_no;
							return tmp;
						} else {
							for (int i = 1; i < tmp.lexeme.length(); i++) {
								input.UngetChar(tmp.lexeme.at(tmp.lexeme.length()-i));
							}
							tmp.lexeme = "0";
						}

					} else {
						input.UngetChar(c);
						input.UngetChar('.');
					}
				} else {
					input.UngetChar(c);
				}
			} else if (c == 'x') {
				// base-num
				if (!input.EndOfInput()) {
					input.GetChar(c);
					if (c == '0') {
						if (!input.EndOfInput()) {
							input.GetChar(c);
							if (c == '8') {
								// base-8
								if (Base8Num(tmp.lexeme)) {
									tmp.lexeme += "x08";
									tmp.token_type = BASE08NUM;
									tmp.line_no = line_no;
									return tmp;
								} else {
									input.UngetChar(c);
									input.UngetChar('0');
									input.UngetChar('x');
								}
							} else {
								input.UngetChar(c);
								input.UngetChar('0');
								input.UngetChar('x');
							}
						} else {
							input.UngetChar(c);
							input.UngetChar('x');
						}
					} else if (c == '1') {
						if (!input.EndOfInput()) {
							input.GetChar(c);
							if (c == '6') {
								// base-16
								tmp.lexeme += "x16";
								tmp.token_type = BASE16NUM;
								tmp.line_no = line_no;
								return tmp;
							} else {
								input.UngetChar(c);
								input.UngetChar('1');
								input.UngetChar('x');
							}
						} else {
							input.UngetChar(c);
							input.UngetChar('x');
						}
					} else {
						input.UngetChar(c);
						input.UngetChar('x');
					}
				} else {
					input.UngetChar(c);
				}
			} else {
				input.UngetChar(c);
			}
		}

		if (!input.EndOfInput() && tmp.lexeme != "0") { // base-16
			string token = "";

			if (Base16Token(token)) {
				tmp.lexeme += token;
				tmp.line_no = line_no;
				tmp.token_type = BASE16NUM;

				return tmp;
			} else {
				//cout << "Token=" << token << endl;
				for (int i = 0; i < token.length(); i++) {
					input.UngetChar(token.at(token.length()-i-1));
				}
			}
		}

        tmp.token_type = NUM;
        tmp.line_no = line_no;
        return tmp;
    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
        tmp.line_no = line_no;
        return tmp;
    }
}

bool LexicalAnalyzer::IsHex(char c)
{
	if (c >= '0' && c <= '9') return true;
	if (c >= 'A' && c <= 'F') return true;

	return false;
}

bool LexicalAnalyzer::Base16Token(string &token)
{
	char c;
	input.GetChar(c);
	while (!input.EndOfInput() && IsHex(c)) {
		token += c;
		input.GetChar(c);
	}

	if (!input.EndOfInput()) {
		token += c;
		if (c == 'x') {
			if (!input.EndOfInput()) {
				input.GetChar(c);
				token += c;
				if (c == '1') {
					if (!input.EndOfInput()) {
						input.GetChar(c);
						token += c;
						if (c == '6') { // a Base16 number
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

Token LexicalAnalyzer::ScanIdOrKeyword()
{
    char c;
    input.GetChar(c);

    if (isalpha(c)) {
        tmp.lexeme = "";
        while (!input.EndOfInput() && isalnum(c)) {
            tmp.lexeme += c;
            input.GetChar(c);
        }
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.line_no = line_no;
        if (IsKeyword(tmp.lexeme))
            tmp.token_type = FindKeywordIndex(tmp.lexeme);
        else
            tmp.token_type = ID;
    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
    }
    return tmp;
}

// you should unget tokens in the reverse order in which they
// are obtained. If you execute
//
//    t1 = lexer.GetToken();
//    t2 = lexer.GetToken();
//    t3 = lexer.GetToken();
//
// in this order, you should execute
//
//    lexer.UngetToken(t3);
//    lexer.UngetToken(t2);
//    lexer.UngetToken(t1);
//
// if you want to unget all three tokens. Note that it does not
// make sense to unget t1 without first ungetting t2 and t3
//
TokenType LexicalAnalyzer::UngetToken(Token tok)
{
    tokens.push_back(tok);;
    return tok.token_type;
}

Token LexicalAnalyzer::GetToken()
{
    char c;

    // if there are tokens that were previously
    // stored due to UngetToken(), pop a token and
    // return it without reading from input
    if (!tokens.empty()) {
        tmp = tokens.back();
        tokens.pop_back();
        return tmp;
    }

    SkipSpace();
    tmp.lexeme = "";
    tmp.line_no = line_no;
    input.GetChar(c);
    switch (c) {
        case '.':
            tmp.token_type = DOT;
            return tmp;
        case '+':
            tmp.token_type = PLUS;
            return tmp;
        case '-':
            tmp.token_type = MINUS;
            return tmp;
        case '/':
            tmp.token_type = DIV;
            return tmp;
        case '*':
            tmp.token_type = MULT;
            return tmp;
        case '=':
            tmp.token_type = EQUAL;
            return tmp;
        case ':':
            tmp.token_type = COLON;
            return tmp;
        case ',':
            tmp.token_type = COMMA;
            return tmp;
        case ';':
            tmp.token_type = SEMICOLON;
            return tmp;
        case '[':
            tmp.token_type = LBRAC;
            return tmp;
        case ']':
            tmp.token_type = RBRAC;
            return tmp;
        case '(':
            tmp.token_type = LPAREN;
            return tmp;
        case ')':
            tmp.token_type = RPAREN;
            return tmp;
        case '<':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = LTEQ;
            } else if (c == '>') {
                tmp.token_type = NOTEQUAL;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = LESS;
            }
            return tmp;
        case '>':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = GTEQ;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = GREATER;
            }
            return tmp;
        default:
            if (isdigit(c)) {
                input.UngetChar(c);
                return ScanNumber();
            } else if (isalpha(c)) {
                input.UngetChar(c);
                return ScanIdOrKeyword();
            } else if (input.EndOfInput())
                tmp.token_type = END_OF_FILE;
            else
                tmp.token_type = ERROR;

            return tmp;
    }
}

int main()
{
    LexicalAnalyzer lexer;
    Token token;

    token = lexer.GetToken();
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        token = lexer.GetToken();
        token.Print();
    }
}
