package parser;

import global.DBComponent;
import heap.TupleDesc;

import java.util.Arrays;
import java.util.List;
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
    private JoinArgs joinArgs;

    public Query(String tableName, String[] projectedColumns, JoinArgs joinArgs) {
        this.tableName = tableName;
        this.projectedColumns = projectedColumns;
        this.joinArgs = joinArgs;
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
     * Returns whether the table is being joined
     */
    public boolean hasJoinArguments() {
        return joinArgs != null;
    }

    /**
     * Get the join arguments from the query
     */
    public JoinArgs getJoinArgs() {
        return joinArgs;
    }

    /**
     * Validates the query according to the appropriate schema (obtained using the table name)
     */
    public String validate() {
        TupleDesc schema = DBComponent.CATALOG().readSchema(tableName);
        if(schema == null) return "Invalid Schema";
        List<String> schemaColumns = schema.getColumnNames();
        // If is not being joined
        if(!hasJoinArguments()) {
            for (String column : projectedColumns) {
                if(!schemaColumns.contains(column)) return "Invalid Column: " + column;
            }
            return null;
        }
        // Validates the JOIN arguments
        // If being joined, check both tables for columns
        TupleDesc joinSchema = DBComponent.CATALOG().readSchema(joinArgs.getJoinTable());
        if(joinSchema == null) return "Invalid Schema";
        List<String> joinSchemaColumns = joinSchema.getColumnNames();
        for (String column : projectedColumns) {
            if(!schemaColumns.contains(column) && !joinSchemaColumns.contains(column)) {
                return "Invalid Column: " + column;
            }
            if(schemaColumns.contains(column) && joinSchemaColumns.contains(column)) {
                return "Ambiguous Column: " + column;
            }
        }
        if(!schemaColumns.contains(joinArgs.getLeftColumn()) || !joinSchemaColumns.contains(joinArgs.getRightColumn())) {
            return "Join condition columns cannot be found in the schema";
        }
        if(schema.getFieldType(joinArgs.getLeftColumn()) != joinSchema.getFieldType(joinArgs.getRightColumn())) {
            return "Join columns are of a different type";
        }
        return null;

    }

    static Pattern expected = Pattern.compile("SELECT (\\w+(, \\w+)*)" + // Column Names (Index 1)
            " FROM (\\w+)" + // Table Name (Index 3)
            "( JOIN (\\w+) ON (\\w+) = (\\w+))?;?");

    /**
     * Parses the query using the above regex from a string into a Query object
     */
    public static Query generateQuery(String command) {
        // Checks if command matches query string format
        Matcher matcher = expected.matcher(command);
        if (!matcher.matches()) return null;
        // Parse information out of command string
        String tableName = matcher.group(3);
        String[] projectedColumns = matcher.group(1).split(", ");
        JoinArgs joinArgs = null;
        if(matcher.group(4) != null) { // Has JOIN arguments
            joinArgs = new JoinArgs(matcher.group(5), matcher.group(6), matcher.group(7));
        }
        return new Query(tableName, projectedColumns, joinArgs);
    }

    @Override
    public String toString() {
        final StringBuffer sb = new StringBuffer("Running a query on: ").append(tableName);
        sb.append(" projecting over columns: ").append(Arrays.toString(projectedColumns));
        if(hasJoinArguments()) {
            sb.append(" joining ").append(joinArgs);
        }
        return sb.toString();
    }
}
