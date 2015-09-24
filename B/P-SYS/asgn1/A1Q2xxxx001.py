#####################################
#	COMPSCI 105 S2 C, 2015          #
#	Assignment 1 Question 2         #
#                                   #
#	@author  	YOUR NAME and UPI   #
#	@version	THE DATE            #
#####################################

def q2(input):

    rpn_stack = Stack() #change in-order sequence to postfix sequence

    expression = []

    operator_prio = {} #operator's higher precedence

    operator_prio['('] = 5
    operator_prio[')'] = 5
    operator_prio['^'] = 4
    operator_prio['*'] = 3
    operator_prio['/'] = 3
    operator_prio['+'] = 2
    operator_prio['-'] = 2
    operator_prio['<'] = 1
    operator_prio['>'] = 1

	# expression translate
    for i in input.split(' '):
        if i not in operator_prio:
            expression.append(i)
        elif rpn_stack.empty() == True or i == '(':
            rpn_stack.push(i)
        elif i == ')':
            while rpn_stack.empty() == False and rpn_stack.top() != '(':
                expression.append(rpn_stack.pop())
            rpn_stack.pop()
        elif operator_prio[i] > operator_prio[rpn_stack.top()] or rpn_stack.top() == '(':
            rpn_stack.push(i)
        else:
            while rpn_stack.empty() == False and rpn_stack.top() != '(' and operator_prio[rpn_stack.top()] >= \
                    operator_prio[i]:
                expression.append(rpn_stack.pop())
            rpn_stack.push(i)
    while rpn_stack.empty() == False:
        expression.append(rpn_stack.pop())

    eval_stack = Stack() # expression evaluate stack

    for i in expression:
        if i not in operator_prio:
           eval_stack.push(int(i)) # push operands
        else: # diff operator evaluate
            if i == '^':
                a = eval_stack.pop()
                b = eval_stack.pop()
                eval_stack.push(b ** a)
            elif i == '*':
                a = eval_stack.pop()
                b = eval_stack.pop()
                eval_stack.push(b * a)
            elif i == '/':
                a = eval_stack.pop()
                b = eval_stack.pop()
                eval_stack.push(b / a)
            elif i == '+':
                a = eval_stack.pop()
                b = eval_stack.pop()
                eval_stack.push(b + a)
            elif i == '-':
                a = eval_stack.pop()
                b = eval_stack.pop()
                eval_stack.push(b - a)
            elif i == '<':
                a = eval_stack.pop()
                b = eval_stack.pop()
                eval_stack.push(min(a, b))
            elif i == '>':
                a = eval_stack.pop()
                b = eval_stack.pop()
                eval_stack.push(max(a, b))

	# complete the evaluate
    return eval_stack.pop()

class Stack:
	'''
	stack container
	'''
	def __init__(self):
		self.data = []

	def push(self, item):
		self.data.append(item)

	def empty(self):
		if len(self.data) > 0:
			return False
		else:
			return True

	def pop(self):
		return self.data.pop()

	def top(self):
		return self.data[-1]


#test case
print(q2('2 ^ ( 1 + 3 ^ 2 )'))
print(q2('( 3 * 5 ) - ( 1 > 2 > 3 < 4 )'))
print(q2('( 300 * 5 ) - ( 100 > 2 > 3 < 4 )'))
