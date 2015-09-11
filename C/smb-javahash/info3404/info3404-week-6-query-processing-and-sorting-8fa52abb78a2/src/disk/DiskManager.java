package disk;

import global.DatabaseConstants;

import java.io.File;
import java.io.IOException;
import java.io.RandomAccessFile;

/**
 * Disk Manager
 * - Manages access to the disk.
 * - Responsible for allocating/deallocating pages on file, and providing the methods to read/write a page
 *
 * Do note: This implementation does not keep track of deallocated pages.
 */
public class DiskManager implements DatabaseConstants {

    private static final PageId FIRST_PAGE = new PageId(DatabaseConstants.FIRST_PAGE_ID);

    private RandomAccessFile dbFile;
    private long numPages;
    private final String dbName;

    // For simple statistics
    private int pageAccesses;

	private static int count = 0;

    /**
     * Opens (and if it doesn't exist, creates) a database using the the given name.
     * If database does not exist, will create it with 1 page
     * @throws java.io.IOException failed to open/write to database
     */
    public DiskManager(String dbName) throws IOException {
        this(dbName, 1);
    }

    /**
     * Opens (and if it doesn't exist, creates) a database using the the given name.
     * @param dbName the name of the database file
     * @param numPages the number of pages to create if no database exists
     * @throws java.io.IOException failed to open/write to database
     */
    public DiskManager(String dbName, int numPages) throws IOException {
        this(dbName, numPages, new RandomAccessFile(dbName, "rws"));
    }

    public DiskManager(String dbName, int numPages, RandomAccessFile file) throws IOException {
        this.dbName = dbName;
        this.dbFile = file;
        initialise(numPages);
    }

    private void initialise(int numPages) throws IOException {
        // Initialise first header page
        if(dbFile.length() == 0) {
            this.numPages = numPages;
            HeaderPage hPage = new HeaderPage();
            hPage.initialise();
            writePage(FIRST_PAGE, hPage);
            // Makes the file numPages long (initialised to zero)
            dbFile.seek(numPages * DatabaseConstants.PAGE_SIZE);
            dbFile.writeByte(0);
        }
        this.numPages = dbFile.length() / DatabaseConstants.PAGE_SIZE;
        this.pageAccesses = 0;
    }

    /**
     * Resets the file representing the database. Any created tables will be destroyed!
     */
    public void reset() throws IOException {
        try {
            dbFile.close();
        }
        catch (IOException e) {}
        finally {
            File file = new File(dbName);
            file.delete();

			count++;
			
            dbFile = new RandomAccessFile(dbName+count, "rws");
            initialise(1);
        }
    }

    /**
     * Returns the number of pages in the database
     */
    public long getNumPages() {
        return numPages;
    }

    /**
     * Allocates a page on disk
     * @return the page id of the allocated page
     */
    public PageId allocatePage() {
        /* Iterate through all the pages to see if there is one free? */
        // Does not search for unused pages, each request extends the file
        return new PageId((int)numPages++);
    }

    /**
     * Deallocates the page associated with the given page id
     * Wipes the page and leaves it blank. Currently we do not keep track of deallocated pages.
     * Once such solution is to keep a list of PageIds of deallocated pages in the header.
     */
    public void deallocatePage(PageId pageId) {
        HeaderPage hPage = new HeaderPage();
        try {
            readPage(pageId, hPage);
            hPage.setNumPointers(DatabaseConstants.INVALID_PAGE_ID);
            writePage(pageId, hPage);
            pageId.set(DatabaseConstants.INVALID_PAGE_ID);

        } catch(IOException e) {}
    }

    /**
     * Reads the page from file
     * We pass it the page instance to conserve memory
     */
    public void readPage(PageId pageId, Page page) throws IOException {
        // We should only request pages that exist
        assert pageId.isValid() && pageId.get() < numPages : "Expects PageId exists";
        // Find page's position in the file
        dbFile.seek(pageId.get() * DatabaseConstants.PAGE_SIZE);
        // Reads page data from file into our page object
        dbFile.read(page.getData());
        pageAccesses++;
    }

    /**
     * Writes the page to disk
     */
    public void writePage(PageId pageId, Page page) throws IOException {
        // We should only write to pages that exist
        assert pageId.isValid() && pageId.get() < numPages : "Expects PageId exists";
        // Find page's position in the file
        dbFile.seek(pageId.get() * DatabaseConstants.PAGE_SIZE);
        // Reads page data from file into our page object
        dbFile.write(page.getData());
        pageAccesses++;
    }

    /**
     * Returns the number of disk reads that have taken place
     */
    public int getPageAccesses() {
        return pageAccesses;
    }
}
