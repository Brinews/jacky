package execution;

import access.read.AccessIterator;
import heap.HeapFile;
import parser.Query;
import projection.Projection;
import join.NestedLoopJoin;

import java.util.Scanner;

/**
 * This class parses and executes the query given to the program by the user.
 *
 * It loops waiting for input (run method), once a command has been entered, it creates a new query and then checks that
 * it is valid. If it is, it will execute the query. This involves pipelining the query based on the different components
 *
 * E.g. in the query SELECT age, name FROM students WHERE age > 10 AND age < 20;
 *
 * It will load in the iterator for the students table (AccessIterator rows = table.iterator();)
 * It will then wrap this iterator in a filter for both (age > 10), and another filter for (age < 20)
 * Finally it will project out the columns from this filtered iterator (creating a new TupleDesc to represnt them)
 */
public class QueryEngine {

    /**
     * Input Loop
     * - Loops over input read in from the user, validating the correctness and calling execute on valid queries
     * - Exits once "exit" is called
     * - Also will do manually timing of query execution when TIME command is entered;
     */
    public static void run() {
        boolean isTiming = false;
        Scanner input = new Scanner(System.in);
        String command = "";
        System.out.println("Welcome to the query engine.\n" +
                "Please enter a query below: e.g. SELECT name, age FROM students;");
        while(true) {
            System.out.print("Enter your query: ");
            command = input.nextLine();
            if(command.equals("exit")) break;
            // Supports timing command execution
            if(command.equals("TIME")) {
                isTiming = !isTiming;
                System.out.println(isTiming ? "Timing is enabled" : "Timing is disabled");
                continue;
            }
            Query query = Query.generateQuery(command);
            if(!checkQuery(query)) continue;
            // Time query and run it
            long startTime = System.nanoTime();
            AccessIterator result = execute(query);
            long endTime = System.nanoTime();
            if(isTiming) { System.out.printf("Took %.2f seconds\n", (endTime-startTime) / 1000000000.0);}
            // Print out results and close iterator
            printResult(result);
            result.close();
        }
        input.close();
    }

    /**
     * Checks that a query is valid. If a query given here is null, then it failed to be parsed properly by
     * Query.generateQuery(...). Ensures that the table name given was a valid schema, and that all column names
     * referenced can be found in that schema
     */
    private static boolean checkQuery(Query query) {
        if(query == null) {
            System.out.println("Invalid formatting of query");
            return false;
        }
        String error = query.validate();
        if(error != null) {
            System.out.println(error);
        }
        return error == null;
    }

    /**
     * Executes a query.
     * 1. Loads an iterator over the rows from the table used in the query
     * 2. Filters this iterator based on the where clauses
     * 3. Sorts the query (?)
     * 4. Projects/Filters the columns
     *
     * You need to complete the filterWhere method, and add it to this execution method. Currently, we're unable to
     * filter rows. We need to be able to apply all of the WhereTuples in our query.getWhereArgs() to our current
     * row/table, to filter out all the rows we don't want. This takes advantage of the idea of pipelining. You should
     * try and chain your iterators together when filtering the rows value.
     */
    protected static AccessIterator execute(Query query) {
        HeapFile table = new HeapFile(query.getTableName());
        // Get all rows
        AccessIterator rows = table.iterator();
        if (query.hasJoinArguments()) {
            HeapFile joinTable = new HeapFile(query.getJoinArgs().getJoinTable());
            rows = new NestedLoopJoin(rows, joinTable.iterator(), query.getJoinArgs());
        }
        rows = new Projection(rows, query.getProjectedColumns());
        return rows;
    }

    /**
     * Helper method to simply print out all the entries in an access iterator
     */
    private static void printResult(AccessIterator rows) {
        while(rows.hasNext()) {
            System.out.println(rows.next().toRow());
        }
    }

}
