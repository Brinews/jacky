package access.write;

import disk.PageId;
import global.DBComponent;
import global.DataPage;
import heap.Tuple;
import heap.TupleDesc;

import java.util.NoSuchElementException;

/**
 * Inserter class to allow for efficient insertion into a DataFile
 * - The idea is that it works similar to an iterator, in that it keeps a reference
 * to the current page alive, however these set of classes work together to allow to insert tuples.
 */
public abstract class DataFileInserter extends AccessInserter {

    private PageId dataPageId;
    private DataPage dataPage;
    private DataPageInserter dataPageInserter;

    /**
     * Takes the PageId of the first page in the file and opens (and pins) it
     * @param dataPageId page id of the first page
     */
    public DataFileInserter(PageId dataPageId) {
        this.dataPageId = dataPageId;
        this.dataPage = getDataPage(dataPageId);
        this.dataPageInserter = new DataPageInserter(dataPage);
    }

    /**
     * Returns a pinned data page (used to support schema/indexes)
     * @param pageId page of the page to load
     * @return pinned data page corresponding to pageId
     */
    protected abstract DataPage getDataPage(PageId pageId);

    @Override
    public void insert(Tuple item) {
        if(!canInsert()) {
            throw new NoSuchElementException("No such tuple");
        }
        dataPageInserter.insert(item);
    }

    @Override
    public boolean canInsert() {
        if(dataPageInserter.canInsert()) {
            return true;
        }
        while(dataPage.getNextPageId().isValid()) {
            PageId next = dataPage.getNextPageId();
            DBComponent.BUFFER_MANAGER().unpin(dataPageId, true);
            dataPage = getDataPage(next);
            dataPageId = next;
            dataPageInserter = new DataPageInserter(dataPage);
            if(dataPageInserter.canInsert()) return true;
        }
        // Create new page we don't have any space
        // Note: slightly ugly, should probably refactor into a DataPage.createNextPage() method
        PageId newPageId = DBComponent.BUFFER_MANAGER().getNewPage();
        DataPage newPage = getDataPage(newPageId);
        newPage.initialise(DBComponent.CATALOG().findNameOfSchema(getSchema()));
        newPage.setPreviousPageId(dataPageId);
        dataPage.setNextPageId(newPageId);
        DBComponent.BUFFER_MANAGER().unpin(dataPageId, true);
        dataPageId = newPageId;
        dataPage = newPage;
        dataPageInserter = new DataPageInserter(dataPage);
        return dataPageInserter.canInsert();
    }

    @Override
    public void close() {
        dataPageInserter = null;
        DBComponent.BUFFER_MANAGER().unpin(dataPageId, true);
    }

    @Override
    public TupleDesc getSchema() {
        return dataPage.getSchema();
    }
}
