package join;

import heap.Tuple;

import java.util.Comparator;

/**
 * Join Comparator
 * - Allows you to compare two tuples and see if they're greater, less than, or equal to each other
 * - Used specifically for comparing two tuples that could be joined.
 */
public class JoinComparator implements Comparator<Tuple> {
    private final String left;
    private final String right;

    /**
     * Used two compare two tuples
     * @param columnLeft name of the left column to join on
     * @param columnRight name of the right column to join on
     */
    public JoinComparator(String columnLeft, String columnRight) {
        this.left = columnLeft;
        this.right = columnRight;
    }

    @Override
    public int compare(Tuple o1, Tuple o2) {
        Object leftValue = o1.getColumn(left);
        Object rightValue = o2.getColumn(right);
        Integer compare = null;
        switch (o1.getSchema().getFieldType(left)) {
            case INTEGER:
                compare = Integer.compare((Integer) leftValue, (Integer) rightValue);
                break;
            case DOUBLE:
                compare = Double.compare((Double) leftValue, (Double) rightValue);
                break;
            case STRING:
                compare = ((String) leftValue).compareTo((String) rightValue);
                break;
            case BOOLEAN:
                compare = Boolean.compare((Boolean) leftValue, (Boolean) rightValue);
                break;
        }
        return compare;
    }
}
