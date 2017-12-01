from Node import Node
from LinkedListIterator import LinkedListIterator

import unittest

class OrderedList:

    def __init__(self):
        self.__head = None
        self.__count = 0
    
    def is_empty(self):
        return self.__count == 0

    def size(self):
        return self.__count

    def add(self,item):
        new_node = Node(item)
        curr = self.__head
        prev = None
        self.add_recursive(new_node,curr,prev)

    def add_recursive(self,new_node,curr,prev):
        #To be completed
        if curr is None and prev is None: # add head
            self.__head = new_node
            return

        if curr is None and prev is not None: # add tail
            prev.set_next(new_node)
            return

        if curr is not None and new_node.get_data() < curr.get_data():
            new_node.set_next(curr)
            if prev is not None: # add middle
                prev.set_next(new_node)
            else: # add head
                self.__head = new_node
            return

        self.add_recursive(new_node, curr.get_next(), curr)

        
    def search(self,item):
        curr = self.__head
        return self.search_recursive(item, curr)

    def search_recursive(self,item, curr):
        #To be completed
        if curr is None:
            return False
        if curr.get_data() == item:
            return True

        return self.search_recursive(item, curr.get_next())


    def remove(self,item):
        curr = self.__head
        prev = None
        self.remove_recursive(item,curr,prev)
        
    def remove_recursive(self,item,curr,prev):    
        #To be completed
        if curr is None:
            return

        if curr is not None and curr.get_data() == item:
            if prev is None:
                self.__head = curr.get_next()
            else:
                prev.set_next(curr.get_next())

        self.remove_recursive(item, curr.get_next(), curr)

    def __iter__(self):
        return LinkedListIterator(self.__head)

class LinkListTest(unittest.TestCase):
    def setUp(self):
        self.mlist = OrderedList()

    def test_add(self):
        self.mlist.add(10)
        self.assertEqual(self.search(10), True)

if __name__ == "__main__":
    unittest.main()
