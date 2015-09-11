package execution;

import access.read.AccessIterator;
import access.write.AccessInserter;
import global.DBComponent;
import heap.HeapFile;
import heap.TupleDesc;
import org.junit.Before;
import org.junit.Test;

import static org.junit.Assert.*;

public class LimitTest {

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
                studentInserter.insert("Michael", i, 200.5 + i, true);
            }
        }
        rows = student.iterator();
    }

    @Test
    public void testHasNext() throws Exception {
        AccessIterator limit = new Limit(rows, 10);
        assertTrue("You are being too strict; denying the first tuple to be accessed", limit.hasNext());
        for(int i = 0; i < 10; i++) limit.next();
        assertFalse("You aren't enforcing the limit", limit.hasNext());
    }

    @Test
    public void testEmptyIterator() throws Exception {
        // Test empty iterator to start with
        while(rows.hasNext()) {rows.next();}
        AccessIterator limit = new Limit(rows, 10);
        assertFalse("You aren't handling an empty iterator properly", limit.hasNext());
    }

    @Test
    public void testNotEnough() throws Exception {
        // Test empty iterator to start with
        while(rows.hasNext()) {rows.next();}
        AccessIterator limit = new Limit(rows, 101);
        // If this fails, this is because you're only checking the limit,
        // and not if there is still an element in the iterator
        while(limit.hasNext()) limit.next();
    }

    @Test
    public void testNext() throws Exception {
        AccessIterator limit = new Limit(rows, 10);
        assertTrue("You aren't returning the correct tuples", limit.next().rowEquals("Michael", 0, 200.5, true));
        assertTrue("You aren't returning the correct tuples", limit.next().rowEquals("Michael", 1, 201.5, true));
        assertTrue("You aren't returning the correct tuples", limit.next().rowEquals("Michael", 2, 202.5, true));
        assertTrue("You aren't returning the correct tuples", limit.next().rowEquals("Michael", 3, 203.5, true));
        assertTrue("You aren't returning the correct tuples", limit.next().rowEquals("Michael", 4, 204.5, true));
        assertTrue("You aren't returning the correct tuples", limit.next().rowEquals("Michael", 5, 205.5, true));
        assertTrue("You aren't returning the correct tuples", limit.next().rowEquals("Michael", 6, 206.5, true));
        assertTrue("You aren't returning the correct tuples", limit.next().rowEquals("Michael", 7, 207.5, true));
        assertTrue("You aren't returning the correct tuples", limit.next().rowEquals("Michael", 8, 208.5, true));
        assertTrue("You aren't returning the correct tuples", limit.next().rowEquals("Michael", 9, 209.5, true));
    }
}