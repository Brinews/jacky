package sort;

import access.read.AccessIterator;
import access.write.AccessInserter;
import global.DBComponent;
import heap.HeapFile;
import heap.Tuple;
import heap.TupleDesc;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import static org.junit.Assert.*;

public class InMemoryOrderByTest {

    private AccessIterator rows;

    @Before
    public void setUp() throws Exception {
        DBComponent.resetComponents();
        DBComponent.DISK_MANAGER().reset();
        // Create Test Schema
        TupleDesc studentSchema = new TupleDesc();
        studentSchema.addString("name").addInteger("age").addDouble("speed").addBoolean("male");
        DBComponent.CATALOG().createSchema(studentSchema, "students");
        HeapFile student = new HeapFile("students");
        try(AccessInserter studentInserter = student.inserter()) {
            for (int i = 0; i < 100; i++) {
                studentInserter.insert("Michael", i, Math.random() * 100, true);
            }
        }
        rows = student.iterator();
    }

    @After
    public void tearDown() throws Exception {
        rows.close();
        DBComponent.DISK_MANAGER().reset();
    }

    @Test
    public void testSortSimply() throws Exception {
        int pageLoads = DBComponent.BUFFER_MANAGER().getPageAccesses().size();
        AccessIterator sorted = new InMemoryOrderBy(rows, new String[]{"speed"});
        assertTrue("No elements in sorted iterator. Check that you are implementing hasNext & next() properly", sorted.hasNext());
        Tuple previous = sorted.next();
        ColumnComparator columnComparator = new ColumnComparator(new String[]{"speed"});
        for(int i = 0; i < 99; i++) {
            assertTrue("Not enough elements in the iterator. You might be missing an edge case", sorted.hasNext());
            Tuple next = sorted.next();
            assertTrue("Elements are NOT sorted", columnComparator.compare(previous, next) <= 0);
            previous = next;
        }
        assertFalse("Too many elements in list. Make sure you aren't accidently reusing a list", sorted.hasNext());
        int totalPageLoads = DBComponent.BUFFER_MANAGER().getPageAccesses().size() - pageLoads;
        // Need to include the first page loaded in the rows Iterator
        assertEquals("You are loading too many pages too many times. Sort in memory.", 4, totalPageLoads+1);
    }

    @Test
    public void testSortMulticolumn() throws Exception {
        int pageLoads = DBComponent.BUFFER_MANAGER().getPageAccesses().size();
        AccessIterator sorted = new InMemoryOrderBy(rows, new String[]{"speed", "age"});
        assertTrue("No elements in sorted iterator. Check that you are implementing hasNext & next() properly", sorted.hasNext());
        Tuple previous = sorted.next();
        ColumnComparator columnComparator = new ColumnComparator(new String[]{"speed", "age"});
        for(int i = 0; i < 99; i++) {
            assertTrue("Not enough elements in the iterator. You might be missing an edge case", sorted.hasNext());
            Tuple next = sorted.next();
            assertTrue("Elements are NOT sorted", columnComparator.compare(previous, next) <= 0);
            previous = next;
        }
        int totalPageLoads = DBComponent.BUFFER_MANAGER().getPageAccesses().size() - pageLoads;
        assertFalse("Too many elements in list. Make sure you aren't accidently reusing a list", sorted.hasNext());
        // Need to include the first page loaded in the rows Iterator
        assertEquals("You are loading too many pages too many times. Sort in memory.", 4, totalPageLoads+1);
    }
}