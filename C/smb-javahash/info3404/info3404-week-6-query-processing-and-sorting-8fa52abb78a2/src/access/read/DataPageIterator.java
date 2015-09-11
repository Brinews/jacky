package access.read;

import global.DataPage;
import heap.Tuple;

import java.util.Iterator;

/**
 * Provides an iterator for the tuples on a page
 */
public class DataPageIterator implements Iterator<Tuple> {

    private DataPage dataPage;
    private int slotNo;

    /**
     * Takes a data page and initialises the current slot as 0
     * @param dataPage the page to iterate over
     */
    public DataPageIterator(DataPage dataPage) {
        this.dataPage = dataPage;
        this.slotNo = 0;
    }

    /**
     * Whether there is another record on the page to read
     * @return true if current slot number is still within the count on the record
     */
    @Override
    public boolean hasNext() {
        return slotNo < dataPage.getRecordCount();
    }

    /**
     * Returns the next tuple and increments the slot counter
     * @return the next tuple on the page
     */
    @Override
    public Tuple next() {
        Tuple newTuple = new Tuple(dataPage.getSchema());
        dataPage.getRecord(slotNo, newTuple);
        slotNo++;
        return newTuple;
    }

    /**
     * Unsupported remove operation. Ignore this.
     */
    @Override
    public void remove() {
        throw new UnsupportedOperationException();
    }

}
