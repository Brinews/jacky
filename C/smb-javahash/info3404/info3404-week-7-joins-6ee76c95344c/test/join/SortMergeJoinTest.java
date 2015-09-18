package join;

import access.read.AccessIterator;
import global.DBComponent;
import heap.HeapFile;
import heap.TupleDesc;
import join.data.ExtraSortedTestData;
import join.data.SortedTestData;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import parser.JoinArgs;
import utilities.ConstructorBuster;
import utilities.DBLogger;
import utilities.TestingUtils;

import static org.junit.Assert.*;

public class SortMergeJoinTest {

    private HeapFile students;
    private HeapFile tutors;
    private DBLogger logger;

    @Before
    public void setUp() throws Exception {
        DBComponent.resetComponents();
        DBComponent.DISK_MANAGER().reset();
        // Create Test Schema for students
        TupleDesc studentSchema = new TupleDesc();
        studentSchema.addString("name").addInteger("age").addString("class").addBoolean("male");
        DBComponent.CATALOG().createSchema(studentSchema, "students");
        HeapFile students = new HeapFile("students");
        // Create Test Schema for tutors
        TupleDesc tutorSchema = new TupleDesc();
        tutorSchema.addString("id").addString("tutor");
        DBComponent.CATALOG().createSchema(tutorSchema, "tutors");
        HeapFile tutors = new HeapFile("tutors");
        this.students = students;
        this.tutors = tutors;
        this.logger = new DBLogger();
    }

    @After
    public void tearDown() throws Exception {
        DBComponent.DISK_MANAGER().reset();
    }

    @Test(timeout = 100)
    public void constructorBuster() throws Exception {
        // You shouldn't be doing the join in the constructor
        JoinArgs args = new JoinArgs("_", "name", "name");
        AccessIterator results = new SortMergeJoin(new ConstructorBuster(), new ConstructorBuster(), args);
    }

    @Test
    public void testMergeJoinSimple() throws Exception {
        // Insert rows
        TestingUtils.insertRows(students, SortedTestData.STUDENT_ROWS_SMALL_SORTED);
        TestingUtils.insertRows(tutors, SortedTestData.TUTOR_ROWS_SMALL_SORTED);
        // Test
        logger.start();
        JoinArgs args = new JoinArgs("tutors", "class", "id");

		try(AccessIterator ret = new SortMergeJoin(students.iterator(), tutors.iterator(), args)) {
			while (ret.hasNext()) {
				System.out.println(ret.next().toString());
			}
		}

        try(AccessIterator results = new SortMergeJoin(students.iterator(), tutors.iterator(), args)) {
            String message = "The tuples you are returning aren't matching what is expected";
            assertTrue("You aren't returning enough tuples", results.hasNext());
            assertTrue(message, results.next().rowEquals("Roger", 20, "INFO1103", true, "INFO1103", "Joshua"));
            assertTrue("You aren't returning enough tuples", results.hasNext());
            assertTrue(message, results.next().rowEquals("Roger", 20, "INFO1103", true, "INFO1103", "Scott"));
            assertTrue("You aren't returning enough tuples", results.hasNext());
            assertTrue(message, results.next().rowEquals("Michael", 19, "INFO1103", true, "INFO1103", "Joshua"));
            assertTrue("You aren't returning enough tuples", results.hasNext());
            assertTrue(message, results.next().rowEquals("Michael", 19, "INFO1103", true, "INFO1103", "Scott"));
            assertTrue("You aren't returning enough tuples", results.hasNext());
            assertTrue(message, results.next().rowEquals("Jan", 18, "INFO1903", false, "INFO1903", "Steven"));
            assertTrue("You aren't returning enough tuples", results.hasNext());
            assertTrue(message, results.next().rowEquals("John", 18, "INFO1903", false, "INFO1903", "Steven"));
            assertFalse("Returning too many results", results.hasNext());
        }
        logger.stop();
        // When we reset or save/load the iterator, this will use need to access a page
        // Thus the worst case costs in the lectures may not apply
        // instead it will be Br + Bs + (number of pages loaded again to handle multiple joins)
		System.out.println(logger.getPageAccesses());
        assertTrue("You aren't reading as many pages as you need to",
                logger.getPageAccesses() >= 2 && logger.getPageAccesses() <= 3);

    }

    @Test
         public void testMergeJoinComplex() throws Exception {
        // Insert more rows
        TestingUtils.insertRows(students, SortedTestData.STUDENT_ROWS_SORTED);
        TestingUtils.insertRows(tutors, SortedTestData.TUTOR_ROWS_SORTED);
        // Test
        JoinArgs args = new JoinArgs("tutors", "class", "id");
        logger.start();
        try(AccessIterator results = new SortMergeJoin(students.iterator(), tutors.iterator(), args)) {
            String message = "The tuples you are returning aren't matching what is expected";
            assertEquals(message, 37814, TestingUtils.count(results));
        }
        logger.stop();
		System.out.println(logger.getPageAccesses());
        assertTrue("You aren't reading as many pages as you need to",
                logger.getPageAccesses() >= 51 && logger.getPageAccesses() <= 2295);
    }

    @Test
    public void testMergeJoinComplexUnique() throws Exception {
        // Insert more rows
        TestingUtils.insertRows(students, ExtraSortedTestData.SORTED_STUDENT_ROWS);
        TestingUtils.insertRows(tutors, ExtraSortedTestData.SORTED_TUTOR_ROWS);
        // Test
        JoinArgs args = new JoinArgs("tutors", "class", "id");
        logger.start();
        try(AccessIterator results = new SortMergeJoin(students.iterator(), tutors.iterator(), args)) {
            String message = "The tuples you are returning aren't matching what is expected";
            assertEquals(message, 141, TestingUtils.count(results));
        }
        logger.stop();
		System.out.println(logger.getPageAccesses());
        assertEquals("You aren't reading as many pages as you need to", 16, logger.getPageAccesses());
    }


}
