#include "common.h"

#include <cstdlib>

enum TokenState {
    OPERATOR,
	NUM_NON_DOTTED,
	NUM_DOTTED,
	NUM_NEG,
	L_PAREN,
	R_PAREN,
	EXCEPTION,
};

OpType getType(char c)
{
    if (c == '+') return op_add;
    if (c == '-') return op_sub;
    if (c == '*') return op_mul;
    if (c == '/') return op_div;
    if (c == '(') return l_paren;
    if (c == ')') return r_paren;

    return op_unknown;
}

bool token_parse(const string &expr, Stack &raw_stack)
{
    string value;
    char chr;
    int paren_num = 0;
    bool polar = false;
    int idx = 0;
    int len = expr.size();

    TokenState state = OPERATOR, next_state;
    OpType optype;
    StackEle *se;
    double val;
    bool num_space = false; // no space in num

    while (state != EXCEPTION && idx < len) {
        chr = expr[idx++];

        switch (state) {
            case NUM_NON_DOTTED:
                if (isdigit(chr) && !num_space) {
                    value += chr;
                    next_state = NUM_NON_DOTTED;
					if (idx == len) {
						/* 表达式结尾 */
						val = polar ? atof(value.c_str()) : -atof(value.c_str());
                        se = new DataStackEle(val);
                        raw_stack.push(se);
					}
                } else if (chr == '.') {
                    if (value.size() == 0) {
                        next_state = EXCEPTION;
                    } else {
                        value += chr;
                        next_state = NUM_DOTTED;
                    }
                } else if (chr== '+'||chr=='-'||chr=='*'||chr=='/') {
                    next_state = OPERATOR;

                    if (value.size() == 0) {
                        next_state = EXCEPTION;
                    } else {
                        val = polar ? atof(value.c_str()) : -atof(value.c_str());
                        se = new DataStackEle(val);
                        raw_stack.push(se);

                        optype = getType(chr);
                        se = new OpStackEle(optype);
                        raw_stack.push(se);

                        num_space = false;
                    }
                } else if (chr == '(') {
                    next_state = EXCEPTION;
                } else if (chr == ')') {
                    next_state = R_PAREN;

                    if (value.size() == 0) {
                        next_state = EXCEPTION;
                    } else {
                        val = polar ? atof(value.c_str()) : -atof(value.c_str());
                        se = new DataStackEle(val);
                        raw_stack.push(se);

                        optype = getType(chr);
                        se = new OpStackEle(optype);
                        raw_stack.push(se);

                        num_space = false;
                        paren_num--;
                        if (paren_num < 0) {
                            next_state = EXCEPTION;
                        }
                    }
                } else if (chr == ' ') {
                    num_space = true;
                } else {
                    next_state = EXCEPTION;
                }
                break;
            case NUM_DOTTED:
                if (isdigit(chr) && !num_space) {
                    next_state = NUM_DOTTED;
                    value += chr;
					if (idx == len) {
						/* 表达式结尾 */
						val = polar ? atof(value.c_str()) : -atof(value.c_str());
                        se = new DataStackEle(val);
                        raw_stack.push(se);
					}
                } else if (chr == '.'){
                    next_state = EXCEPTION;
                } else if (chr== '+'||chr=='-'||chr=='*'||chr=='/') {
                    next_state = OPERATOR;

                    val = polar ? atof(value.c_str()) : -atof(value.c_str());
                    se = new DataStackEle(val);
                    raw_stack.push(se);

                    optype = getType(chr);
                    se = new OpStackEle(optype);
                    raw_stack.push(se);

                    num_space = false;
                } else if (chr == '(') {
                    next_state = EXCEPTION;
                } else if (chr == ')') {
                    next_state = R_PAREN;

                    val = polar ? atof(value.c_str()) : -atof(value.c_str());
                    se = new DataStackEle(val);
                    raw_stack.push(se);

                    optype = getType(chr);
                    se = new OpStackEle(optype);
                    raw_stack.push(se);

                    num_space = false;

                    paren_num--;
                    if (paren_num < 0) {
                        next_state = EXCEPTION;
                    }
                } else if (chr == ' ') {
                    num_space = true;
                } else {
                    next_state = EXCEPTION;
                }
                break;
            case OPERATOR:
                if (isdigit(chr)) {
                    next_state = NUM_NON_DOTTED;
                    polar = true;
                    value = chr;
					if (idx == len) {
						/* 表达式结尾 */
						val = polar ? atof(value.c_str()) : -atof(value.c_str());
                        se = new DataStackEle(val);
                        raw_stack.push(se);
					}
                } else if (chr == '+' || chr == '*' || chr == '/') {
                    next_state = EXCEPTION;
                } else if (chr == '-') {
                    next_state = NUM_NON_DOTTED;
                    polar = false;
                    value = "";
                } else if (chr == '(') {
                    next_state = L_PAREN;
                    paren_num++;
                    se = new OpStackEle(l_paren);
                    raw_stack.push(se);
                } else if (chr == ' ') {
                    /* do nothing */
                } else {
                    /* non-illegal char */
                    next_state = EXCEPTION;
                }
                break;
            case L_PAREN:
                if (isdigit(chr)) {
                    next_state = NUM_NON_DOTTED;
                    polar = true;
                    value = chr;
                } else if (chr == '-') {
                    next_state = NUM_NON_DOTTED;
                    polar = false;
                    value = "";
                } else if (chr = ' ') {
                    /* do nothing */
                } else if (chr == '(') {
                    next_state = L_PAREN;
                    paren_num++;
                    se = new OpStackEle(l_paren);
                    raw_stack.push(se);
                } else {
                    next_state = EXCEPTION;
                }
                break;
            case R_PAREN:
                if (chr == '+'||chr == '-'||chr == '*'||chr == '/') {
                    next_state = OPERATOR;
                    optype = getType(chr);
                    se = new OpStackEle(optype);
                    raw_stack.push(se);
                } else if (chr == ')') {
                    next_state = R_PAREN;
                    paren_num--;
                    if (paren_num < 0) {
                        next_state = EXCEPTION;
                    } else {
                        se = new OpStackEle(r_paren);
                        raw_stack.push(se);
                    }
                } else if (chr == ' ') {
                    /* do nothing */
                } else {
                    next_state = EXCEPTION;
                }
                break;
        }

		if (next_state == EXCEPTION) {
			while (!raw_stack.empty()) {
				delete raw_stack.pop();
			}
			return false;
		}

        state = next_state;
    }

	return true;
}
