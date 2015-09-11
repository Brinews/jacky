package execution;

import access.read.AccessIterator;
import access.write.AccessInserter;
import global.DBComponent;
import heap.HeapFile;
import heap.TupleDesc;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import parser.Query;
import parser.WhereArgs;

import static org.junit.Assert.*;

public class QueryEngineTest {

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
    public void testFilterWhereLess() throws Exception {
        WhereArgs.WhereTuple whereArgument = new WhereArgs.WhereTuple("age", WhereArgs.Comparison.LESS, "10");
        AccessIterator filtered = QueryEngine.filterWhere(rows, whereArgument);
        int total = COUNT(filtered);
        assertEquals(10, total);
    }

    @Test
    public void testFilterWhereLessOrEqual() throws Exception {
        WhereArgs.WhereTuple whereArgument = new WhereArgs.WhereTuple("age", WhereArgs.Comparison.LEQ, "10");
        AccessIterator filtered = QueryEngine.filterWhere(rows, whereArgument);
        int total = COUNT(filtered);
        assertEquals(11, total);
    }

    @Test
    public void testFilterWhereGreater() throws Exception {
        WhereArgs.WhereTuple whereArgument = new WhereArgs.WhereTuple("age", WhereArgs.Comparison.GREATER, "10");
        AccessIterator filtered = QueryEngine.filterWhere(rows, whereArgument);
        int total = COUNT(filtered);
        assertEquals(89, total);
    }

    @Test
    public void testFilterWhereGreaterOrEqual() throws Exception {
        WhereArgs.WhereTuple whereArgument = new WhereArgs.WhereTuple("age", WhereArgs.Comparison.GEQ, "10");
        AccessIterator filtered = QueryEngine.filterWhere(rows, whereArgument);
        int total = COUNT(filtered);
        assertEquals(90, total);
    }

    @Test
    public void testFilterWhereEqual() throws Exception {
        WhereArgs.WhereTuple whereArgument = new WhereArgs.WhereTuple("age", WhereArgs.Comparison.EQUAL, "10");
        AccessIterator filtered = QueryEngine.filterWhere(rows, whereArgument);
        int total = COUNT(filtered);
        assertEquals(1, total);
    }
    @Test
    public void testFilterWhereNotEqual() throws Exception {
        WhereArgs.WhereTuple whereArgument = new WhereArgs.WhereTuple("age", WhereArgs.Comparison.NOT_EQUAL, "10");
        AccessIterator filtered = QueryEngine.filterWhere(rows, whereArgument);
        int total = COUNT(filtered);
        assertEquals(99, total);
    }

    @Test
    public void testExecuteWhereSimple() throws Exception {
        Query query = Query.generateQuery("SELECT name, age FROM students WHERE speed < 209.5");
        AccessIterator results = QueryEngine.execute(query);
        assertTrue("You aren't returning the correct tuples", results.next().rowEquals("Michael", 0));
        assertTrue("You aren't returning the correct tuples", results.next().rowEquals("Michael", 1));
        assertTrue("You aren't returning the correct tuples", results.next().rowEquals("Michael", 2));
        assertTrue("You aren't returning the correct tuples", results.next().rowEquals("Michael", 3));
        assertTrue("You aren't returning the correct tuples", results.next().rowEquals("Michael", 4));
        assertTrue("You aren't returning the correct tuples", results.next().rowEquals("Michael", 5));
        assertTrue("You aren't returning the correct tuples", results.next().rowEquals("Michael", 6));
        assertTrue("You aren't returning the correct tuples", results.next().rowEquals("Michael", 7));
        assertTrue("You aren't returning the correct tuples", results.next().rowEquals("Michael", 8));
        assertFalse("Too many elements in the iterator", results.hasNext());
    }

    @Test
    public void testExecuteWhereRange() throws Exception {
        Query query = Query.generateQuery("SELECT name, age FROM students WHERE speed < 209.5 AND age > 5");
        AccessIterator results = QueryEngine.execute(query);
        assertTrue("You aren't returning the correct tuples", results.next().rowEquals("Michael", 6));
        assertTrue("You aren't returning the correct tuples", results.next().rowEquals("Michael", 7));
        assertTrue("You aren't returning the correct tuples", results.next().rowEquals("Michael", 8));
        assertFalse("Too many elements in the iterator", results.hasNext());
    }

    @Test
    public void testExecuteWhereRangeExact() throws Exception {
        Query query = Query.generateQuery("SELECT name, age FROM students WHERE speed <= 209.5 AND age >= 5");
        AccessIterator results = QueryEngine.execute(query);
        assertTrue("You aren't returning the correct tuples", results.next().rowEquals("Michael", 5));
        assertTrue("You aren't returning the correct tuples", results.next().rowEquals("Michael", 6));
        assertTrue("You aren't returning the correct tuples", results.next().rowEquals("Michael", 7));
        assertTrue("You aren't returning the correct tuples", results.next().rowEquals("Michael", 8));
        assertTrue("You aren't returning the correct tuples", results.next().rowEquals("Michael", 9));
        assertFalse("Too many elements in the iterator", results.hasNext());
    }

    @Test
    public void testExecuteWhereRangeInProjectionColumns() throws Exception {
        Query query = Query.generateQuery("SELECT speed, age FROM students WHERE speed <= 209.5 AND age >= 5");
        AccessIterator results = QueryEngine.execute(query);
        assertTrue("You aren't returning the correct tuples", results.next().rowEquals(205.5, 5));
        assertTrue("You aren't returning the correct tuples", results.next().rowEquals(206.5, 6));
        assertTrue("You aren't returning the correct tuples", results.next().rowEquals(207.5, 7));
        assertTrue("You aren't returning the correct tuples", results.next().rowEquals(208.5, 8));
        assertTrue("You aren't returning the correct tuples", results.next().rowEquals(209.5, 9));
        assertFalse("Too many elements in the iterator", results.hasNext());
    }

    @Test
    public void testExecuteWhereRangeLimited() throws Exception {
        Query query = Query.generateQuery("SELECT speed, age FROM students WHERE speed <= 209.5 AND age >= 5 LIMIT 2");
        AccessIterator results = QueryEngine.execute(query);
        assertTrue("You aren't returning the correct tuples", results.next().rowEquals(205.5, 5));
        assertTrue("You aren't returning the correct tuples", results.next().rowEquals(206.5, 6));
        assertFalse("Too many elements in the iterator", results.hasNext());
    }

    @After
    public void tearDown() throws Exception {
        rows.close();
        DBComponent.DISK_MANAGER().reset();
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
