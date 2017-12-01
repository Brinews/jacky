from OrderedList import OrderedList
import random

o_list = OrderedList()
for i in range(10):
    rand_num = random.randrange(0,10)
    while o_list.search(rand_num):
        rand_num = random.randrange(0,10)
    o_list.add(rand_num)
    
print("After insertion:")
for item in o_list:
    print(item,end=" ")
print()

for i in range(10):
    rand_num = random.randrange(0,10)
    while not(o_list.search(rand_num)):
        rand_num = random.randrange(0,10)
    o_list.remove(rand_num)

print("After deletion:")
for item in o_list:
    print(item,end=" ")
print()
