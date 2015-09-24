#####################################
#	COMPSCI 105 S2 C, 2015          #
#	Assignment 1 Question 1         #
#                                   #
#	@author  	YOUR NAME and UPI   #
#	@version	THE DATE            #
#####################################

from Item import Item

class Cart:
	# the constructor
	def __init__(self):
		self.__items = []
	
	# This function gets the number of items in the shopping cart.
	def get_size(self):
		return len(self.__items)

	#################################################################################################
	# The implementation of the above functions have already been given.                            #
	# Please DO NOT MODIFY the content of the ABOVE functions, as they are used by other functions. #
	# Please given the implementation of the following five functions to complete the program.      #
	#################################################################################################

	# This function adds an item into the shopping cart.
	def add_item(self, item):		
		## IMPLEMENT THIS METHOD
		item.setQuantity(1)
		self.__items.append(item)
	
	# This function finds an item on sale based on the item code.
	def find_item(self, code):
		## IMPLEMENT THIS METHOD
		for item in self.__items:
			if item.getCode() == code:
				return item

		return None

	# This function removes an item from the shopping cart.
	def delete_item(self, item):
		## IMPLEMENT THIS METHOD
		self.__items.remove(item)

    # This function clears everything in the shopping cart.
	def discard_all(self):
		## IMPLEMENT THIS METHOD
		self.__items = []
	
	# This function prints out the items bought and calculates the total amount due.
	def check_out(self):
		## IMPLEMENT THIS METHOD
		total = 0
		for item in self.__items:
			print ("{0}/${1}\n".format(item.getDescription(), item.getPrice()))
			total += item.getPrice()*item.getQuantity()

		return total

