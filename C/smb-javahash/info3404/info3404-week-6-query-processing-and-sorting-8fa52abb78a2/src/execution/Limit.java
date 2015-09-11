package execution;

import access.read.AccessIterator;
import filter.Filter;
import heap.Tuple;
import heap.TupleDesc;


/**
 * Class to restrict the number of tuples that are output.
 * - We will accept a tuple (match its condition), if we haven't already matched more than 'limit' tuples
 * - Once we have accepted 'limit' tuples, we should stop searching
 */
public class Limit extends AccessIterator {

    private AccessIterator rows;
	protected Tuple nextTuple;
	private int accounter;

    public Limit(AccessIterator rows, int limit) {
        this.rows = rows;
		nextTuple = null;
		accounter = limit;
    }

    @Override
    public void close() {
        rows.close();
    }

    @Override
    public TupleDesc getSchema() {
        return rows.getSchema();
    }

    @Override
    public boolean hasNext() {
		if (!rows.hasNext()) return false;

		while (rows.hasNext() && accounter > 0) {
			nextTuple = rows.next();

			return true;
		}

        return false;
    }

    @Override
    public Tuple next() {
		if (hasNext()) {
			Tuple next = nextTuple;
			nextTuple = null;
			accounter--;

			return next;
		}

		return null;
    }
}
