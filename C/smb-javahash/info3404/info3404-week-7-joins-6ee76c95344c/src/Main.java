import access.write.AccessInserter;
import execution.QueryEngine;
import global.DBComponent;
import heap.HeapFile;
import heap.TupleDesc;

import java.io.IOException;

/**
 * Example Execution using Database Components.
 * See the test cases for more examples
 * Here a few to get you started to play around:
 * SELECT name, age FROM students WHERE age < 10;
 * SELECT name, speed FROM students WHERE speed > 10 ORDER BY speed, name;
 */
public class Main {

    public static void main(String[] args) throws IOException {
        DBComponent.initialiseComponents();
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

        // Insert rows
        insertRows(students, STUDENT_ROWS_SMALL);
        insertRows(tutors, TUTOR_ROWS_SMALL);

        QueryEngine.run();
        DBComponent.BUFFER_MANAGER().flushDirty();
    }

    public static void insertRows(HeapFile table, Object[][] rows) {
        try(AccessInserter inserter = table.inserter()) {
            for(Object[] row : rows) {
                inserter.insert(row);
            }
        }
    }

    public static final Object[][] STUDENT_ROWS_SMALL = new Object[][] {
            new Object[]{"Michael", 19, "INFO1103", true},
            new Object[]{"Jan", 18, "INFO1903", false},
            new Object[]{"Roger", 20, "INFO1103", true},
            new Object[]{"Rachael", 21, "ELEC1601", false}
    };

    public static final Object[][] TUTOR_ROWS_SMALL = new Object[][] {
            new Object[]{"INFO1103", "Joshua"},
            new Object[]{"INFO1103", "Scott"},
            new Object[]{"COMP2129", "Maxwell"},
            new Object[]{"INFO1903", "Steven"}
    };
}
