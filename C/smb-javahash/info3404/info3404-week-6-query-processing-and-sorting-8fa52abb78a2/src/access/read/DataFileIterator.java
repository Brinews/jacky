package access.read;

import disk.PageId;
import global.DBComponent;
import global.DataPage;
import heap.Tuple;
import heap.TupleDesc;

import java.util.NoSuchElementException;

/**
 * Iterator over a DataFile (collection of data pages)
 */
public abstract class DataFileIterator extends AccessIterator {

    private PageId dataPageId;
    private DataPage dataPage;
    private DataPageIterator dataPageIterator;

    /**
     * Takes the PageId of the first page in the file and opens (and pins) it
     * @param dataPageId page id of the first page
     */
    public DataFileIterator(PageId dataPageId) {
        this.dataPageId = dataPageId;
        this.dataPage = getDataPage(dataPageId);
        this.dataPageIterator = new DataPageIterator(dataPage);
    }

    /**
     * Returns a pinned data page (used to support schema/indexes)
     * @param pageId page of the page to load
     * @return pinned data page corresponding to pageId
     */
    protected abstract DataPage getDataPage(PageId pageId);

    /**
     * Close the iterator (and unpin any pages pinned by it)
     */
    @Override
    public void close() {
        dataPageIterator = null;
        DBComponent.BUFFER_MANAGER().unpin(dataPageId, false);
    }

    /**
     * Checks where there is another record in the file
     * Requires checking not only current page, but the following page
     * in case there are also records on there
     * @return true if there is another record, false if not
     */
    @Override
    public boolean hasNext() {
        if(dataPageIterator.hasNext()) {
            return true;
        }
        while(dataPage.getNextPageId().isValid()) {
            PageId next = dataPage.getNextPageId();
            DBComponent.BUFFER_MANAGER().unpin(dataPageId, false);
            dataPage = getDataPage(next);
            dataPageId = next;
            dataPageIterator = new DataPageIterator(dataPage);
            if(dataPageIterator.hasNext()) return true;
        }
        return false;
    }

    /**
     * @return the next tuple in the file
     */
    @Override
    public Tuple next() {
        if(hasNext()) {
            Tuple next = dataPageIterator.next();
            next.setPageId(dataPageId);
            return next;
        }
        throw new NoSuchElementException("No such tuple");
    }

    @Override
    public TupleDesc getSchema() {
        return dataPage.getSchema();
    }
}
