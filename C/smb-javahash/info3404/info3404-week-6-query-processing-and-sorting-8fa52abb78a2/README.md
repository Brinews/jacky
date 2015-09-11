# Query Processing and Sorting #

## Easy
 - Finish the implementation of the WHERE clause in our query parser. Have a look at QueryEngine, specifically the filterWhere and execute methods
 - Implement the LIMIT operator that can be found in the execution package.
 
## Medium
- Complete Easy, as well as implement the InMemoryOrderBy class in the sort package. This asks you to materialise the rows and sort them in memory
 
## Hard
- Complete Easy, as well as implement the ExternalMergeOrderBy in the sort package. This asks you to implement the External Merge Sort algorithm covered in the lectures
 
The code might seem familiar :) This time hopefully you all should have a bit more fun since you can execute queries and see the result. If you run the Main method, it will prompt you for input and an example query you can run would be "SELECT name, age FROM students". As you implement, LIMIT, ORDER BY and WHERE you can actually see the results in the queries that you do.