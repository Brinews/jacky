package parser;

/**
 * Join Arguments
 *  - Object to encapsulate a join command
 *  - Stores the name of the table to join on, the column to join on from the main table,
 *    and the column of the right table
 */
public class JoinArgs {
    private String leftColumn;
    private String rightColumn;
    private String joinTable;

    public JoinArgs(String joinTable, String leftColumn, String rightColumn) {
        this.leftColumn = leftColumn;
        this.rightColumn = rightColumn;
        this.joinTable = joinTable;
    }

    /**
     * Returns the name of the left column to join on
     */
    public String getLeftColumn() {
        return leftColumn;
    }

    /**
     * Returns the name of the right column to join on
     */
    public String getRightColumn() {
        return rightColumn;
    }

    /**
     * Returns the name of the table to join on
     */
    public String getJoinTable() {
        return joinTable;
    }

    @Override
    public String toString() {
        final StringBuffer sb = new StringBuffer();
        sb.append("TABLE[").append(joinTable).append(']');
        sb.append('{').append(leftColumn);
        sb.append(" = ").append(rightColumn).append('}');
        return sb.toString();
    }
}
