package sort;

import access.read.AccessIterator;
import heap.Tuple;
import heap.TupleDesc;

import java.util.*;

/**
 * In-memory Sorting of the Tuples for the OrderBy Operation
 * - Loads all of the tuples into memory, and then sorts this list
 * - Returns the sorted tuples in order when the iterator is accessed
 */
public class InMemoryOrderBy extends AccessIterator {

    private TupleDesc schema;
	private Tuple[] tpArray;
	private ArrayList<Tuple> tupleArrayList = new ArrayList<Tuple>();
	private AccessIterator myRows;
	private int index = 0;

    /**
     * Constructor for the in-memory OrderBy sort iterator
     * - You will need to sort the tuples in the constructor
     * - Look at using `new ColumnComparator(columns)` and Collections.sort(...)
     * @param rows iterator over the rows to sort
     * @param columns the columns to sort on
     */
    public InMemoryOrderBy(AccessIterator rows, final String[] columns) {
        schema = rows.getSchema();
		myRows = rows;

		while (rows.hasNext()) {
			tupleArrayList.add(rows.next());
		}

		tpArray = tupleArrayList.toArray(new Tuple[tupleArrayList.size()]);

		ColumnComparator comp = new ColumnComparator(columns);

		/* quick sort */
		Arrays.sort(tpArray, comp);

		index = 0;
    }

    @Override
    public void close() {
		myRows.close();
    }

    @Override
    public TupleDesc getSchema() {
        return schema;
    }

    @Override
    public boolean hasNext() {
		if (tpArray != null && index < tpArray.length) return true;
        return false;
    }

    @Override
    public Tuple next() {
		if (hasNext()) return tpArray[index++];
        return null;
    }
}
