package heap;

import access.read.AccessIterator;
import access.read.HeapFileIterator;
import access.write.AccessInserter;
import access.write.HeapFileInserter;
import disk.HeaderPage;
import disk.PageId;
import global.DBComponent;

/**
 * Represents a collection of unordered pages containing tuples. (A collection of HeapPages)
 * - Relies on a schema to interpret the Tuples
 */
public class HeapFile {

    private String schemaName;
    private TupleDesc schema;
    private PageId firstPageId;

    /**
     * Initialises the HeapFile with the given schema
     * Note: Assumes the schema has already been created in Catalog
     * @param schemaName
     */
    public HeapFile(String schemaName) {
        this.schemaName = schemaName;
        schema = DBComponent.CATALOG().readSchema(schemaName);
        firstPageId = HeaderPage.getFileEntry(DBComponent.BUFFER_MANAGER(), schemaName);
        // If we haven't created the HeapFile for this schema yet, then do so
        if(!firstPageId.isValid()) {
            firstPageId = DBComponent.BUFFER_MANAGER().getNewPage();
            HeaderPage.setFileEntry(DBComponent.BUFFER_MANAGER(), schemaName, firstPageId);
            HeapPage firstPage = new HeapPage(DBComponent.BUFFER_MANAGER().getPage(firstPageId));
            firstPage.initialise(schemaName);
            DBComponent.BUFFER_MANAGER().unpin(firstPageId, true);
        }
    }

    /**
     * Creates a temporary HeapFile with the given schema
     * Note: this is created in our database. As such, over time there will be empty pages
     * that accumulate due to the lack of deletion in the system. Resetting info3404_database should resolve this
     * issue
     */
    public HeapFile(String schemaName, boolean isTempFile) {
        if(!isTempFile) throw new AssertionError("Should only be used for temporary files");
        this.schemaName = schemaName;
        schema = DBComponent.CATALOG().readSchema(schemaName);
        firstPageId = DBComponent.BUFFER_MANAGER().getNewPage();
        HeapPage firstPage = new HeapPage(DBComponent.BUFFER_MANAGER().getPage(firstPageId));
        firstPage.initialise(schemaName);
        DBComponent.BUFFER_MANAGER().unpin(firstPageId, true);
    }

    public AccessIterator iterator() {
        return new HeapFileIterator(firstPageId);
    }

    public AccessInserter inserter() {
        return new HeapFileInserter(firstPageId);
    }

}
