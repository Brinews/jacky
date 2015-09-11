package parser;

import heap.TupleDesc;
import parser.WhereArgs.WhereTuple;

import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Represents a query to run on the database
 * Supports the follow syntax:
 * SELECT column, column FROM table WHERE column < 10 ORDER BY column, column
 *
 * We will provide support for performing every range query on a column (Boolean columns may be funny here)
 * You can select columns for your table, and filter based on different columns.
 *
 * Note: We only provide support for WHERE <condition> AND <condition>. That is to say, you cannot construct a query
 * that uses an OR. Likewise with ORDER BY, we only allow columns to be sorted ascending.
 */
public class Query {

    private String tableName;
    private String[] projectedColumns;
    private WhereTuple[] whereArgs;
    private String[] orderBy;
    private Integer limit;

    public Query(String tableName, String[] projectedColumns, WhereTuple[] whereArgs, String[] orderBy, Integer limit) {
        this.tableName = tableName;
        this.projectedColumns = projectedColumns;
        this.whereArgs = whereArgs;
        this.orderBy = orderBy;
        this.limit = limit;
    }

    /**
     * Get the columns to project from the query
     */
    public String[] getProjectedColumns() {
        return projectedColumns;
    }

    /**
     * Get the table name from the query
     */
    public String getTableName() {
        return tableName;
    }

    /**
     * Get the where arguments
     */
    public WhereTuple[] getWhereArgs() {
        return whereArgs;
    }

    /**
     * Get the columns to order by
     */
    public String[] getOrderBy() {
        return orderBy;
    }

    /**
     * Get the number of rows to return
     */
    public Integer getLimit() {
        return Math.max(limit, 0);
    }

    /**
     * Returns whether the query needs to be sorted
     */
    public boolean hasOrderByClause() {
        return orderBy.length > 0;
    }

	/**
	 * Returns whether the query has a where clause
	 */
	public boolean hasWhereClause() {
		return whereArgs.length > 0;
	}

    /**
     * Returns whether the query has a limit clause
     */
    public boolean hasLimitClause() {
        return limit != null;
    }

    /**
     * Validates the query according to the appropriate schema (obtained using the table name)
     */
    public boolean validate(TupleDesc schema) {
        try {
            for(String column : projectedColumns) {
                schema.getIndexFromName(column);
            }
            for(WhereTuple args : whereArgs) {
                int index = schema.getIndexFromName(args.getColumn());
                if(args.getSign() == null) return false;
                if(schema.getFieldType(index).parseType(args.getValue()) == null) return false;
            }
            for(String column : orderBy) {
                schema.getIndexFromName(column);
            }
        }
        catch(NoSuchElementException e) {
            return false;
        }
        return true;
    }

    static Pattern expected = Pattern.compile("SELECT (\\w+(, \\w+)*)" + // Column Names (Index 1)
            " FROM (\\w+)" + // Table Name (Index 3)
            "( WHERE ((\\w+ (>=|<>|<=|=|>|<) [\\w\\.]+)( AND \\w+ (>=|<>|<=|=|>|<) [\\w\\.]+)*))?" + // Where Clause (Optional, Index 5)
            "( ORDER BY (\\w+(, \\w+)*))?" + // Order By Clause (Optional, Index 10)
            "( LIMIT (\\d+))?;?"); // Limit Clause (optional, index 13

    public static Query generateQuery(String command) {
        // Checks if command matches query string format
        Matcher matcher = expected.matcher(command);
        if (!matcher.matches()) return null;
        // Parse information out of command string
        String tableName = matcher.group(3);
        String[] projectedColumns = matcher.group(1).split(", ");
        // Parse Where clause
        WhereTuple[] whereArgs = new WhereTuple[0];
        if (matcher.group(4) != null) {
            whereArgs = WhereArgs.parse(matcher.group(5));
        }
        // Parse OrderBy Clause
        String[] orderBy = new String[0];
        if(matcher.group(10) != null) {
            orderBy = matcher.group(11).split(", ");
        }
        // Parse Limit Clause
        Integer limit = null;
        if(matcher.group(13) != null) {
            limit = Integer.parseInt(matcher.group(14));
        }
        return new Query(tableName, projectedColumns, whereArgs, orderBy, limit);
    }

    @Override
    public String toString() {
        final StringBuffer sb = new StringBuffer("Running a query on: ").append(tableName);
        sb.append(" projecting over columns: ").append(Arrays.toString(projectedColumns));
        sb.append(" filtering on: ").append(Arrays.toString(whereArgs));
        sb.append(" ordering by: ").append(Arrays.toString(orderBy));
        return sb.toString();
    }
}
