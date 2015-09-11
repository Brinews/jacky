package access.write;

import disk.PageId;
import global.DBComponent;
import global.DataPage;
import heap.HeapPage;

/**
 * HeapFile Implementation of the DataFileInserter
 */
public class HeapFileInserter extends DataFileInserter {

    public HeapFileInserter(PageId dataPageId) {
        super(dataPageId);
    }

    @Override
    protected DataPage getDataPage(PageId pageId) {
        return new HeapPage(DBComponent.BUFFER_MANAGER().getPage(pageId));
    }
}
