package heap;

import access.read.AccessIterator;
import access.write.AccessInserter;
import disk.PageId;
import global.DBComponent;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import static org.junit.Assert.*;

public class HeapFileTest {

    private HeapFile student;

    @Before
    public void setUp() throws Exception {
        DBComponent.resetComponents();
        DBComponent.DISK_MANAGER().reset();
        // Create Test Schema
        TupleDesc studentSchema = new TupleDesc();
        studentSchema.addString("name").addInteger("age").addDouble("speed").addBoolean("male");
        DBComponent.CATALOG().createSchema(studentSchema, "students");
        student = new HeapFile("students");
        try(AccessInserter inserter = student.inserter()) {
            for (int i = 0; i < 1000; i++) {
                inserter.insert("Michael", i / 2, 200.0 - i, true);
            }
        }
    }

    @Test(timeout = 1000)
    public void testInsertRecord() throws Exception {
        int i = 0;
        AccessIterator studentIterator = student.iterator();
        while(studentIterator.hasNext()) {
            Tuple studentTuple = studentIterator.next();
            assertTrue(studentTuple.rowEquals("Michael", i/2, 200.0 - i, true));
            i++;
        }
        assertEquals(1000, i);
    }

    @Test(timeout = 1000)
    public void testNoUnpinnedPages() throws Exception {
        int i = 0;
        AccessIterator studentIterator = student.iterator();
        while(studentIterator.hasNext()) {
            studentIterator.next();
            i++;
        }
        assertEquals(1000, i);
        studentIterator.close();
        assertEquals(0, DBComponent.BUFFER_MANAGER().getNumberOfPinnedPages());
        PageId cheatTest = DBComponent.BUFFER_MANAGER().getNewPage();
        DBComponent.BUFFER_MANAGER().getPage(cheatTest);
        assertNotEquals(0, DBComponent.BUFFER_MANAGER().getNumberOfPinnedPages());
		assertTrue("Not equals 0", DBComponent.BUFFER_MANAGER().getNumberOfPinnedPages() > 0);
    }

    @Test(timeout = 1000)
    public void testTupleReturnWithPageIdSet() throws Exception {
        int i = 0;
        AccessIterator studentIterator = student.iterator();
        while(studentIterator.hasNext()) {
            assertTrue(studentIterator.next().getPageId().isValid());
        }
    }


    @After
    public void tearDown() throws Exception {
        DBComponent.DISK_MANAGER().reset();
    }
}
