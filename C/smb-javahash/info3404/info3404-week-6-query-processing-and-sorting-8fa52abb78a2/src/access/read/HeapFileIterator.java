package access.read;

import disk.PageId;
import global.DBComponent;
import global.DataPage;
import heap.HeapPage;

/**
 * Iterator to traverse over a HeapFile (collection of unordered records)
 */
public class HeapFileIterator extends DataFileIterator {

    public HeapFileIterator(PageId dataFilePage) {
        super(dataFilePage);
    }

    @Override
    protected DataPage getDataPage(PageId pageId) {
        return new HeapPage(DBComponent.BUFFER_MANAGER().getPage(pageId));
    }

}
