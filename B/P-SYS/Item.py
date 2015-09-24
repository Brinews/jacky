class Item:
	def __init__(self, code, desc, price, quantity):
		self.__code = code             # the item code
		self.__description = desc      # the item description
		self.__price = price           # the item unit price
		self.__quantity = quantity     # the number of item available
	
	def getCode(self):
		return self.__code
	
	def setCode(self, code):
		self.__code = code;
	
	def getDescription(self):
		return self.__description
	
	def setDescription(self, desc):
		self.__description = desc
	
	def getPrice(self):
		return self.__price
	
	def setPrice(self, price):
		self.__price = price
	
	def getQuantity(self):
		return self.__quantity
	
	def setQuantity(self, quantity):
		self.__quantity = quantity
	
	def __str__(self):
		return '{0}, {1}, {2}, {3}'.format(self.__code, self.__description, str(self.__price), str(self.__quantity))
