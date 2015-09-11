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

import java.util.List;

import static org.junit.Assert.*;

public class ExternalMergeOrderByTest {

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
    public void testProduceSortedRuns() throws Exception {
        ExternalMergeOrderBy sorter = new ExternalMergeOrderBy(rows.getSchema(), new String[]{"speed"});
        ColumnComparator columnComparator = new ColumnComparator(new String[]{"speed"});
        List<HeapFile> sortedRuns = sorter.produceSortedRuns(rows);
        assertEquals("Invalid number of runs. You should have ceil(numTuples / tuplesPerBlock) blocks", 2, sortedRuns.size());
        int count = 0;
        for(HeapFile sortedFile : sortedRuns) {
            assertSorted(sortedFile, columnComparator);
            try(AccessIterator iterator = sortedFile.iterator()) {
                count += COUNT(iterator);
            }
        }
        assertEquals("You are missing/have to many elements. Check how you're sorting.", 100, count);
    }

    @Test
    public void testMergeSortedRuns() throws Exception {
        ExternalMergeOrderBy sorter = new ExternalMergeOrderBy(rows.getSchema(), new String[]{"speed"});
        ColumnComparator columnComparator = new ColumnComparator(new String[]{"speed"});
        List<HeapFile> sortedRuns = sorter.produceSortedRuns(rows);
        // Only 1 Merge run this time
        int startingPageAccesses = DBComponent.BUFFER_MANAGER().getPageAccesses().size();
        sortedRuns = sorter.mergeSortedRuns(sortedRuns);
        // We should only have 8 page accesses, but when we create our output file it has an overhead of 1 additional load.
        assertEquals("You aren't implementing External Merge Sort properly", 9, DBComponent.BUFFER_MANAGER().getPageAccesses().size() - startingPageAccesses);
        // Check that it is sorted
        for(HeapFile sortedFile : sortedRuns) {
            assertSorted(sortedFile, columnComparator);
        }
        int count = 0;
        try(AccessIterator iterator = sortedRuns.get(0).iterator()) {
            count += COUNT(iterator);
        }
        assertEquals("You are missing/have to many elements. Check how you're sorting.", 100, count);
    }

    @Test
    public void testMergeSortedRunsComplex() throws Exception {
        // Add more records to make it more complex
        HeapFile student = new HeapFile("students");
        try(AccessInserter studentInserter = student.inserter()) {
            for (int i = 0; i < 1000; i++) {
                studentInserter.insert("Rachael", i, Math.random() * 10000, false);
            }
        }
        ExternalMergeOrderBy sorter = new ExternalMergeOrderBy(rows.getSchema(), new String[]{"speed", "age"});
        ColumnComparator columnComparator = new ColumnComparator(new String[]{"speed", "age"});
        List<HeapFile> sortedRuns = sorter.produceSortedRuns(rows);
        while(sortedRuns.size() > 1) {
            sortedRuns = sorter.mergeSortedRuns(sortedRuns);
            for(HeapFile sortedFile : sortedRuns) {
                assertSorted(sortedFile, columnComparator);
            }
        }
        int count = 0;
        try(AccessIterator iterator = sortedRuns.get(0).iterator()) {
            count += COUNT(iterator);
        }
        assertEquals("You are missing/have to many elements. Check how you're sorting.", 1100, count);
    }

    @Test
    public void testPickNextTuple() throws Exception {

    }

    private static void assertSorted(HeapFile file, ColumnComparator columnComparator) {
        try(AccessIterator iterator = file.iterator()) {
            assertTrue("Not enough elements in iterator", iterator.hasNext());
            Tuple previous = iterator.next();
            while(iterator.hasNext()) {
                assertTrue("Elements are NOT sorted", columnComparator.compare(previous, iterator.next()) <= 0);
            }
        }
    }

    private static int COUNT(AccessIterator rows) {
        int i = 0;
        while(rows.hasNext()) {
            rows.next();
            i++;
        }
        return i;
    }
}