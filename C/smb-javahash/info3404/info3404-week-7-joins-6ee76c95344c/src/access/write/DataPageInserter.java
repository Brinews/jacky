package access.write;

import global.DataPage;
import heap.Tuple;
import heap.TupleDesc;

/**
 * Inserter class to allow for easy insertion onto a DataPage
 */
public class DataPageInserter extends AccessInserter {
    private DataPage dataPage;

    /**
     * Takes a data page and initialises the current slot as 0
     * @param dataPage the page to iterate over
     */
    public DataPageInserter(DataPage dataPage) {
        this.dataPage = dataPage;
    }

    @Override
    public TupleDesc getSchema() {
        return dataPage.getSchema();
    }

    @Override
    public void insert(Tuple item) {
        dataPage.insertRecord(item);
    }

    @Override
    public boolean canInsert() {
        return dataPage.getRecordCount() < dataPage.getMaxRecordsOnPage();
    }

    @Override
    public void close() {}
}
