package filter;

import global.Type;
import heap.Tuple;

/**
 * Range Predicate
 * Filters out records that don't lie within a certain range.
 * Note: Incompatible with Boolean schema values. You should throw a UnsupportedOperationException should
 * an index on a boolean value be used
 */
public abstract class Range implements Filter.Predicate {

    private boolean sorted;
    private Object rangeValue;

    public Range(Object rangeValue) {
        this(rangeValue, false);
    }

    public Range(Object rangeValue, boolean sorted) {
        this.rangeValue = rangeValue;
        this.sorted = sorted;
    }

    abstract protected boolean matches(int comparedValue);

    @Override
    public boolean matchesCondition(Tuple row, String columnName) {
        int index = row.getSchema().getIndexFromName(columnName);
        Type columnType = row.getSchema().getFieldType(index);
        switch (columnType) {
            case STRING:
                return matches(((String)row.getColumn(index)).compareTo((String)rangeValue));
            case INTEGER:
                return matches(Integer.compare((Integer)row.getColumn(index), (Integer)rangeValue));
            case DOUBLE:
                return matches(Double.compare((Double) row.getColumn(index), (Double) rangeValue));
            case BOOLEAN:
                throw new UnsupportedOperationException();
            default:
                throw new UnsupportedOperationException();
        }
    }
    @Override
    public boolean shouldStopSearching(Tuple row, String columnName) {
        return false;
    }


    /**
     * Below are a list of subclasses for each range operator (except for equals and not equals)
     * These extend the range class since they are essentially similar except they match different tuples
     * according to whether the entry is equal/greater than/less than etc.
     */

    /**
     * Subclass for >= comparisons
     */
    public static class GreaterThanEquals extends Range {

        public GreaterThanEquals(Object rangeValue) {
            super(rangeValue);
        }

        @Override
        protected boolean matches(int comparedValue) {
            return comparedValue >= 0;
        }
    }

    /**
     * Subclass for > comparisons
     */
    public static class GreaterThan extends Range {

        public GreaterThan(Object rangeValue) {
            super(rangeValue);
        }

        @Override
        protected boolean matches(int comparedValue) {
            return comparedValue > 0;
        }
    }

    /**
     * Subclass for <= comparisons
     */
    public static class LessThanEquals extends Range {

        public LessThanEquals(Object rangeValue) {
            super(rangeValue);
        }

        @Override
        protected boolean matches(int comparedValue) {
            return comparedValue <= 0;
        }
    }

    /**
     * Subclass for < comparisons
     */
    public static class LessThan extends Range {

        public LessThan(Object rangeValue) {
            super(rangeValue);
        }

        @Override
        protected boolean matches(int comparedValue) {
            return comparedValue < 0;
        }
    }
}
