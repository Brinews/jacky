import access.read.AccessIterator;
import access.write.AccessInserter;
import access.write.HeapFileInserter;
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
        // Create the Schema
        TupleDesc studentSchema = new TupleDesc()
                .addString("name").addInteger("age").addDouble("speed").addBoolean("male");

        // Add Schema to the Catalog
        DBComponent.CATALOG().createSchema(studentSchema, "students");

        // Create a new heap file
        HeapFile students = new HeapFile("students");
        AccessInserter efficientStudentInsert = students.inserter();
        // Inserts some records into the heap file
        for(int i = 0; i < 10000; i++) {
            efficientStudentInsert.insert("Michael", i*100, Math.random()*100, true);
            efficientStudentInsert.insert("Rachael", i*2,  Math.random()*100, false);
        }
        efficientStudentInsert.close();
        QueryEngine.run();
        DBComponent.BUFFER_MANAGER().flushDirty();
    }

}
