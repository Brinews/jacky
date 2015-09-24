#####################################
#	COMPSCI 105 S2 C, 2015          #
#	Assignment 1 Question 2         #
#                                   #
#	@author  	YOUR NAME and UPI   #
#	@version	THE DATE            #
#####################################

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
