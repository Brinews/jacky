package filter;

import access.read.AccessIterator;
import heap.Tuple;
import heap.TupleDesc;

import java.util.NoSuchElementException;

/**
 * Generic Filter Class
 * - Takes a Predicate to determine whether a tuple is to be selected
 *
 * This class can wrap an AccessIterator to provide for pipelining of query data.
 * For example:
 * AccessIterator filtered = new Filter(students.iterator(), "age", new Equals(20));
 *
 * Will filter the entries in the student's iterator, so it will only return those students whose age = 20;
 *
 * This could be written as "SELECT * FROM students WHERE age = 20;
 * This filter class is essentially a way to handle the WHERE clause of a query.
 */
public class Filter extends AccessIterator {

    protected AccessIterator iterator;
    protected Predicate predicate;
    protected Tuple nextTuple;
    protected String columnName;
    protected boolean stop;

    public Filter(AccessIterator iterator, String columnName, Predicate predicate) {
        this.iterator = iterator;
        this.columnName = columnName;
        this.nextTuple = null;
        this.predicate = predicate;
        this.stop = false;
    }

    @Override
    public boolean hasNext() {
        if(stop) return false;
        if(nextTuple != null) return true;
        if(!iterator.hasNext()) return false;
        // Find the next matching tuple, breaks after first run of values
        while(iterator.hasNext() && !stop) {
            Tuple next = iterator.next();
            if(predicate.matchesCondition(next, columnName)) {
                nextTuple = next;
                break;
            }
            stop = predicate.shouldStopSearching(next, columnName);
        }
        return nextTuple != null;
    }

    @Override
    public Tuple next() {
        if(hasNext()) {
            Tuple next = nextTuple;
            nextTuple = null;
            return next;
        }
        throw new NoSuchElementException();
    }

    @Override
    public void remove() {
        throw new UnsupportedOperationException();
    }

    @Override
    public TupleDesc getSchema() {
        return iterator.getSchema();
    }

    @Override
    public void close() {
        iterator.close();
    }

    /**
     * Interface to checks whether the given tuple matches the filter condition
     */
    public static interface Predicate {
        /**
         * Returns true if a tuple and it's column column matches the conditions of the predicate.
         */
        public boolean matchesCondition(Tuple row, String columnName);
        /**
         * If the predicate has read its last value, then we should stop searching.
         */
        public boolean shouldStopSearching(Tuple row, String columnName);
    }
}
