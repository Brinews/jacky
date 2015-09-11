package disk;

import global.DatabaseConstants;
import global.TestUtils;
import org.junit.Before;
import org.junit.Test;

import static org.junit.Assert.*;

public class HeaderPageTest {

    private static final PageId INVALID_PAGE = new PageId(DatabaseConstants.INVALID_PAGE_ID);

    private HeaderPage mPage;
    private PageId mPageId;

    @Before
    public void setup() {
        mPage = new HeaderPage();
        mPageId = new PageId(DatabaseConstants.INVALID_PAGE_ID);
    }

    @Test
    public void testInitialise() throws Exception {
        mPage.initialise();
        assertEquals(INVALID_PAGE, mPage.getNextPage());
        PageId temp = new PageId(DatabaseConstants.INVALID_PAGE_ID);
        for(int i = 0; i < mPage.getNumPointers(); i++) {
            String entry = mPage.getFileEntry(0, temp);
            assertEquals("", entry);
            assertEquals(INVALID_PAGE, temp);
        }
    }

    @Test
    public void testSetNextPage() throws Exception {
        mPage.initialise();
        assertEquals(INVALID_PAGE, mPage.getNextPage());
        PageId newPageId = new PageId(1);
        mPage.setNextPage(newPageId);
        assertEquals(newPageId, mPage.getNextPage());
    }

    @Test(expected=RuntimeException.class)
    public void testNullSetNextPage() {
        mPage.initialise();
        mPage.setNextPage(null);
    }

    @Test
    public void testNumPointers() throws Exception {
        mPage.initialise();
        int numPointers = mPage.getNumPointers();
        mPage.setNumPointers(100);
        assertEquals(100, mPage.getNumPointers());
        mPage.setNumPointers(-1);
        assertEquals(-1, mPage.getNumPointers());
    }

    public void writeFileEntry(int record, int pageId) throws Exception {
        String max = TestUtils.generateAsciiString(pageId, DatabaseConstants.MAX_TABLE_NAME_LENGTH);
        mPage.setFileEntry(mPageId, max, record);
        PageId temp = new PageId(DatabaseConstants.INVALID_PAGE_ID);
        assertEquals(max, mPage.getFileEntry(record, temp));
        assertEquals(mPageId, temp);
    }

    @Test
    public void testMultipleFileEntry() throws Exception {
        mPage.initialise();
        int top = Integer.MAX_VALUE;
        mPageId.set(top);
        // Arbitrarily writing values to every record in the page
        for(int record = 0; record < mPage.getNumPointers(); record++) {
            String max = TestUtils.generateAsciiString(top - record, DatabaseConstants.MAX_TABLE_NAME_LENGTH);
            mPageId.set(top-record);
            mPage.setFileEntry(mPageId, max, record);
        }
        // We then read through it again and hope that our values are intact
        for(int record = 0; record < mPage.getNumPointers(); record++) {
            String max = TestUtils.generateAsciiString(top - record, DatabaseConstants.MAX_TABLE_NAME_LENGTH);
            String result = mPage.getFileEntry(record, mPageId);
            assertEquals(max, result);
            assertEquals(mPageId, new PageId(top - record));
        }
    }


}