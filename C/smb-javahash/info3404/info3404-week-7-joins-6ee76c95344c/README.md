# Week 7 - Joins

This week we are looking at different types of Joins. In the lectures, we covered:

* Simple Nested-Loop
* Block Nested-Loop
* Index Nested-Loop
* Sort-Merge

For the PASTA task, you have the choice of implementing either Simple Nested-Loop, Block Nested-Loop or Sort-Merge joins; for 2, 3 or 4 points respectively.

This week, you will only need to change the files that are in the joins package.

## Simple Nested-Loop (2 Marks)
Your task is to implement the algorithm in the **NestedLoopJoin.java** file. Some of the boilerplate is already taken care of for you (see **GenericJoin.java**), however feel free to customize it and make it your own.

We have made some changes to AccessIterator which might make your implementation easier. We have added a new method: **reset()**. Which will reset the iterator back to the first tuple. You may find this useful for looping.

## Block Nested-Loop (3 Marks)
Your task is to implement the algorithm in the **BlockNestedLoopJoin.java** file. Some of the boilerplate is already taken care of for you (see **GenericJoin.java**), however feel free to customize it and make it your own.

We have made some changes to AccessIterator which might make your implementation easier. We have added a new method: **reset()**. Which will reset the iterator back to the first tuple. You may find this useful for looping.

There is also a method in **GenericJoin** - getNextBlocks(AccessIterator) - which will return the next B pages worth of tuples from the iterator. This may be useful in generating the blocks to iterate over.

## Sort-Merge (4 Marks)
Your task is to implement the algorithm in the **SortMergeJoin.java** file. Some of the boilerplate is already taken care of for you (see **GenericJoin.java**), however feel free to customize it and make it your own.

We have made some changes to AccessIterator which might make your implementation easier. We have added a new method: **reset()**. Which will reset the iterator back to the first tuple. You may find this useful for looping. We have also added two methods: **savePosition()** and **loadPosition**. This will allow you to save the position of an iterator and revert back to it.

We have also added a JoinComparator class that may help you compare tuples for a join.
