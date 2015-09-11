package filter;

import heap.Tuple;

/**
 * Implements equality testing for an AccessIterator
 */
public class Equals implements Filter.Predicate {

    private Object value;

    public Equals(Object targetValue) {
        value = targetValue;
    }

    @Override
    public boolean matchesCondition(Tuple row, String columnName) {
        int index =  row.getSchema().getIndexFromName(columnName);
        return row.getColumn(index).equals(value);
    }

    @Override
    public boolean shouldStopSearching(Tuple row, String columnName) {
        return false;
    }
}
