package disk;

import buffer.BufferManager;
import global.DatabaseConstants;

/**
 * Created by developer on 19/07/15.
 */
public class HeaderPage extends Page {

    // Offsets for the positions of our values in the HeaderPage
    private static final int NEXT_PAGE_INDEX = 0;
    private static final int NUM_POINTERS_INDEX = 4;
    private static final int HEADER_SIZE = 4 + 4;
    private static final int POINTER_PAGE_ID_OFFSET = 0;
    private static final int POINTER_REL_NAME_OFFSET = 4;
    private static final int POINTER_ENTRY_SIZE = 4 + 2 + DatabaseConstants.MAX_TABLE_NAME_LENGTH;

    public HeaderPage() {
        super();
    }

    public HeaderPage(Page page) {
        data = page.getData();
    }

    /**
     * Initialises the HeaderPage to it's default/sane values
     */
    public void initialise() {
        PageId invalidPage = new PageId(DatabaseConstants.INVALID_PAGE_ID);
        setNextPage(invalidPage);
        int maxSpace = DatabaseConstants.PAGE_SIZE - HEADER_SIZE;
        int maxNumberOfPointers = maxSpace / POINTER_ENTRY_SIZE;
        setNumPointers(maxNumberOfPointers);
        // Initialise all records on the page
        for(int i = 0; i < maxNumberOfPointers; i++) {
            setFileEntry(invalidPage, "", i);
        }
    }

    /**
     * Sets the Page ID of the next HeaderPage. Used to form a linked list of headers
     */
    public void setNextPage(PageId pageId) {
        if(pageId == null) {
            throw new RuntimeException("Page ID is NULL");
        }
        setIntegerValue(pageId.get(), NEXT_PAGE_INDEX);
    }

    /**
     * Returns the PageId of the next header page
     */
    public PageId getNextPage() {
        int pageId = getIntegerValue(NEXT_PAGE_INDEX);
        return new PageId(pageId);
    }

    /**
     * Sets the maximum number of pointers in the page
     */
    public void setNumPointers(int value) {
        setIntegerValue(value, NUM_POINTERS_INDEX);
    }

    /**
     * Returns the maximum number of pointers that can be stored on this page
     */
    public int getNumPointers() {
        return getIntegerValue(NUM_POINTERS_INDEX);
    }

    /**
     * Creates an entry in this page of (pageId, entryText). Used as a "relation index" to indicate
     * the first page of a relation's file.
     * e.g. (5, "employees") : 5 is the pageId of the first "employees" page
     * @param pageId page id of the first page associated with entry
     * @param entry string representing the relation
     * @param recordNumber position in the file to set the entry
     */
    public void setFileEntry(PageId pageId, String entry, int recordNumber) {
        assert entry.length() <= DatabaseConstants.MAX_TABLE_NAME_LENGTH : "Entry cannot be longer than specified value";
        assert recordNumber >= 0 && recordNumber < getNumPointers() : "Record Number must be on page";

        int offset = HEADER_SIZE + recordNumber * POINTER_ENTRY_SIZE;
        setIntegerValue(pageId.get(), offset + POINTER_PAGE_ID_OFFSET);
        setStringValue(entry, offset + POINTER_REL_NAME_OFFSET);
    }

    /**
     * Gets the (pageId, entryText) entry associated with the recordNumber
     * @param recordNumber which record in the page to inspect
     * @param emptyPage modified in place to contain the entry's pageId
     * @return the entryText found at the slot recordNumber
     */
    public String getFileEntry(int recordNumber, PageId emptyPage) {
        assert recordNumber >= 0 && recordNumber < getNumPointers() : "Record Number must be on page";

        int offset = HEADER_SIZE + recordNumber * POINTER_ENTRY_SIZE;
        emptyPage.set(getIntegerValue(offset + POINTER_PAGE_ID_OFFSET));
        return getStringValue(offset + POINTER_REL_NAME_OFFSET);
    }

    /**
     * Puts a file entry in the header page. This is effectively a mapping from entry => pageId.
     * Traverses across all the header pages until it can find a spot to store the value. If no spots are
     * found, it will create a new header page and store it there.
     * @param bufferManager instance of a buffer manager to use for page access
     * @param entry string of the value to store in the page
     * @param pageId the page id to associate with the string value
     */
    public static void setFileEntry(BufferManager bufferManager, String entry, PageId pageId) {
        if (entry.length() > DatabaseConstants.MAX_TABLE_NAME_LENGTH)
            throw new AssertionError("Entry cannot be longer than specified value");

        if (!(pageId.isValid() || pageId.get() < bufferManager.getTotalDiskPages()))
            throw new AssertionError("Expects PageId to exist");

        if(HeaderPage.getFileEntry(bufferManager, entry).isValid())
            throw new AssertionError("File Entry Already Exists");

        PageId currentPageId = new PageId(DatabaseConstants.FIRST_PAGE_ID);
        PageId tempPageId = new PageId();
        HeaderPage hPage;
        // Iterate over header files to find empty space to put this file entry
        while(currentPageId.isValid()) {
            // Pin and read page
            hPage = new HeaderPage(bufferManager.getPage(currentPageId));
            int numRecords = hPage.getNumPointers();
            for(int i=0; i < numRecords; i++) {
                hPage.getFileEntry(i, tempPageId);
                if(!tempPageId.isValid()) {
                    hPage.setFileEntry(pageId, entry, i);
                    bufferManager.unpin(currentPageId, true);
                    bufferManager.flushDirty();
                    return;
                }
            }
            // Go to the next page
            tempPageId = currentPageId;
            currentPageId = hPage.getNextPage();
            // unpin old page
            bufferManager.unpin(tempPageId, false);
        }
        // If we have made it here, then there is no free space on any page
        PageId newPageId = bufferManager.getNewPage();
        HeaderPage newPage = new HeaderPage(bufferManager.getPage(newPageId));
        newPage.initialise();
        newPage.setFileEntry(pageId, entry, 0);
        bufferManager.unpin(newPageId, true);

        // Update reference in previous page to point to our new header
        hPage = new HeaderPage(bufferManager.getPage(tempPageId));
        hPage.setNextPage(newPageId);
        bufferManager.unpin(tempPageId, true);
        bufferManager.flushDirty();
    }

    /**
     * Gets the page id associated with an entry string (if one exists)
     * @param bufferManager instance of a buffer manager to use for page access
     * @param entry string of the value to find in the header
     * @return the page id associated with entry, or an Invalid Page ID
     */
    public static PageId getFileEntry(BufferManager bufferManager, String entry) {
        if (entry.length() > DatabaseConstants.MAX_TABLE_NAME_LENGTH)
            throw new AssertionError("Name cannot be longer than specified value");

        PageId pageId = new PageId();
        PageId currentPageId = new PageId(DatabaseConstants.FIRST_PAGE_ID);
        HeaderPage hPage;
        // Searches across the header pages
        while(currentPageId.isValid()) {
            hPage = new HeaderPage(bufferManager.getPage(currentPageId));
            int numRecords = hPage.getNumPointers();
            // Reads each row in the page
            for(int i=0; i < numRecords; i++) {
                String found = hPage.getFileEntry(i, pageId);
                // if entry strings are equal, then we've found our match so return the page id
                if (entry.equals(found)) {
                    bufferManager.unpin(currentPageId, false);
                    return pageId;
                }
            }
            bufferManager.unpin(currentPageId, false);
            currentPageId = hPage.getNextPage();
        }
        return new PageId(DatabaseConstants.INVALID_PAGE_ID);
    }

}
