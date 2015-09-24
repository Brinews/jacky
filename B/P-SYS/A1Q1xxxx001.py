#####################################
#	COMPSCI 105 S2 C, 2015          #
#	Assignment 1 Question 1         #
#                                   #
#	@author  	YOUR NAME and UPI   #
#	@version	THE DATE            #
#####################################

from Stock import Stock
from Cart import Cart

def display_intro():
    UPI = "abcd001"
    message = "-"*8 + " Simple Online Grocery Program by " + UPI + " "+ "-"*8
    print(message)

###########################################################################
# The implementation of the following functions have already been given.  #
# Please DO NOT MODIFY the content of the functions BELOW, as they are    #                 
# using/calling by other functions. Please rename the program and update  #
# the above constant value in the introduction message with your own UPI. #
###########################################################################

def display_menu():
    print("1. Display All Items on Sale")
    print("2. Add an Item to Shopping Cart")
    print("3. Remove an Item from Cart")
    print("4. Check Out Shopping Cart")
    print("5. Discard Shopping Cart")
    print("6. Exit the System")

def display_separator():
    lines = "-" * 58
    print(lines)

def input_code(item_list):
	code = raw_input("Enter item code: ")
	item = item_list.find_item(code)
	return item

def get_user_input(start,end):
	choice = raw_input("Enter your choice: ")
	while (not choice.isdigit() or int(choice) >end or int(choice) < start):
		print("Invalid menu option.")
		choice = raw_input("Please try again: ")
	return int(choice)

def main_menu(stock, cart):
	display_menu()
	display_separator()
	option = get_user_input(1,6)
	while option != 6:
		if option == 1:
			display_separator()
			print("List of items on sale")
			display_separator()
			stock.display_items()
		elif option == 2:
			display_separator()
			item = input_code(stock)
			display_separator()
			if item != None:
				cart.add_item(item)
			else:
				print('Invalid item code!')
		elif option == 3:
			display_separator()
			item = input_code(cart)
			display_separator()
			if item != None:
				cart.delete_item(item)
			else:
				print('No such item in the cart!')
		elif option == 4:
			display_separator()
			if cart.get_size() > 0:
				print("Your Shopping Bill")
				display_separator()
				sum = cart.check_out()
				display_separator()
				print("Amount due: $" + str(format(sum, ".2f")))
			else:
				print("No item in the shopping cart.")
		else:
			display_separator()
			if cart.get_size() > 0:
				cart.discard_all()
			else:
				print("No item in the shopping cart.")
		display_separator()
		display_menu()
		display_separator()
		option = get_user_input(1,6)
	display_separator()
	print("Thank you for shopping with us!")

def main():
	stock = Stock()
	cart = Cart()
	stock.load_items("stock.txt")
	display_separator()
	display_intro()
	display_separator()
	main_menu(stock, cart)
	display_separator()
	stock.save_items("stock2.txt")

main()
