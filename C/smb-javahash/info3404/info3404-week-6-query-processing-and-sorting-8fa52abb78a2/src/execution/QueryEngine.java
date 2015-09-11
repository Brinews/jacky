package execution;

import access.read.AccessIterator;
import filter.*;
import global.DBComponent;
import heap.HeapFile;
import heap.TupleDesc;
import parser.Query;
import parser.WhereArgs;
import projection.Projection;
import sort.InMemoryOrderBy;

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
        TupleDesc schema = DBComponent.CATALOG().readSchema(query.getTableName());
        if(schema == null) {
            System.out.println("Invalid Schema");
            return false;
        }
        if(!query.validate(schema)) {
            System.out.println("Invalid Column Names");
            return false;
        }
        return true;
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
        // Filter rows according to where clauses

        /**
         * We current can't handle WHERE values (oh no!). You will need to loop over the query.getWhereArgs() and apply
         * the filter to our current rows iterator
         */
		if (query.hasWhereClause()) {
			for (WhereArgs.WhereTuple wt : query.getWhereArgs()) {
				rows = filterWhere(rows, wt);
			}
		}

        // Handle the order by clause
        if(query.hasOrderByClause()) {
            rows = new InMemoryOrderBy(rows, query.getOrderBy());
        }
        rows = new Projection(rows, query.getProjectedColumns());
        // Handles the limit clause
        if(query.hasLimitClause()) {
            rows = new Limit(rows, query.getLimit());
        }
        return rows;
    }

    /**
     * Checks if all of the OrderBy columns are in the Projected columns.
     * - This can be used as an optimisation when projecting and sorting. If the columns we wish to sort on are a subset
     *   of the columns we are projecting on, we can apply the projection first and then sort these columns.
     * - This is an optimisation since instead of having to sort the whole tuple and keep that in memory as we sort, we
     *   only need to store the columns we're projecting when sorting
     * - ["miranda", 10, 20.9, true] vs ["miranda", 10]
     * - Being able to store more tuples of the latter on pages while sorting can speed it up considerably.
     */
    protected static boolean isOrderByInProjected(String[] projected, String[] orderBy) {
        int freq = 0;
        for(String column : orderBy) {
            for(String projectedColumn : projected) {
                if(column.equals(projectedColumn)) {
                    freq++;
                }
            }
        }
        return freq == orderBy.length;
    }

    /**
     * Applies the filter condition described by whereArg to the iterator rows, and returns an iterator over this
     * pipelined view.
     *
     * You will need to implement the rest of the WHERE clause comparison signs. Currently we can only check if a column
     * is equal. Have a look at the signs you need to implement in WhereArgs, and look at the filters that are available
     * in Range and NotModifier classes
     */
    protected static AccessIterator filterWhere(AccessIterator rows, WhereArgs.WhereTuple whereArg) {
        TupleDesc schema = rows.getSchema();
        // Gets the column name of the where clause
        String column = whereArg.getColumn();
        // Obtains the where value in the appropriate type to use for filtering (i.e. we need to convert the string
        // "10.9" to a (double)'10.9' so we can compare our records
        Object value = schema.getFieldType(schema.getIndexFromName(column)).parseType(whereArg.getValue());
        // Applies the right where filter
        switch (whereArg.getSign()) {
            case EQUAL:
                return new Filter(rows, column, new Equals(value));
            /**
             * Oh no! We can only current compare equality. Implement the rest of the comparison signs defined in WhereArgs
             */
			case LESS:
				return new Filter(rows, column, new Range.LessThan(value));
			case GREATER:
				return new Filter(rows, column, new Range.GreaterThan(value));
			case NOT_EQUAL:
				return new Filter(rows, column, new NotModifier(new Equals(value)));

			case GEQ:
				return new Filter(rows, column, new Range.GreaterThanEquals(value));
			case LEQ:
				return new Filter(rows, column, new Range.LessThanEquals(value));

            default:
                return rows;
        }
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
