package linearhashing;

import core.Bucket;
import core.DatabaseConstants;
import core.HashTable;
import core.Tuple;
import org.junit.Before;
import org.junit.Test;
import statichashing.StaticHashTable;

import javax.xml.crypto.Data;
import java.util.HashSet;

import static org.junit.Assert.*;

public class LinearHashTableTest {

    private HashTable hashTable;

    @Before
    public void setUp() throws Exception {
        hashTable = new LinearHashTable();
    }


    @Test(timeout = 1000)
    public void testInitialSize() throws Exception {
        String message = "The LinearHashTable should start by at least 2 buckets";
        assertTrue(message, hashTable.numberOfBuckets() <= 2 && hashTable.numberOfBuckets() > 0);
    }

    @Test(timeout = 1000)
    public void testAdd() throws Exception {
        Tuple tupleOne = new Tuple(0, "Australia", "27 Degrees");

		System.out.println("tupleOne:" + tupleOne.getKeyHash());

        for(int i = 0; i < DatabaseConstants.BUCKET_CAPACITY; i++) {
            hashTable.put(tupleOne);
        }
        assertEquals("Make sure you are splitting when >= 80 capacity", 2, hashTable.numberOfBuckets());
        // Add another
        Tuple tupleTwo = new Tuple(0, "New Zealand", "18 Degrees");

		System.out.println("tupleTwo:" + tupleTwo.getKeyHash());

        for(int i = 0; i < DatabaseConstants.BUCKET_CAPACITY; i++) {
            hashTable.put(tupleTwo);
        }

        assertEquals("Make sure you are splitting when >= 80 capacity", 3, hashTable.numberOfBuckets());
        String message = "If this fails, then you are assigning the wrong values to the wrong buckets. "
                            + "Try stepping through in the debugger to follow the logic of your code.";
        assertEquals(message, 10, hashTable.getBucketsForTesting()[0].size());
        // Add another
        for(int i = 0; i < DatabaseConstants.BUCKET_CAPACITY; i++) {
            hashTable.put(tupleTwo);
        }
        assertEquals(message, 20, hashTable.getBucketsForTesting()[3].size());
    }

    @Test(timeout = 1000)
    public void testGetNoSuchElement() {
        String message = "Occurs when it tries to get an element that doesn't exist."
                + "Check that you are returning null when there is no element found";
        assertNull(message, hashTable.get("apples"));
    }

    @Test(timeout = 1000)
    public void testOverflow() {
        Tuple tuple = new Tuple(0, "Australia", "27 Degrees");

		System.out.println("tuple:" + tuple.getKeyHash());

        for(int i = 0; i < DatabaseConstants.BUCKET_CAPACITY * 2; i++) {
            hashTable.put(tuple);
        }
        String message = "Make sure you're handing bucket overflows when getting elements correctly";
        assertEquals(message, DatabaseConstants.BUCKET_CAPACITY * 2, hashTable.numberOfTuples());
        Bucket found = hashTable.getBucket(tuple.getKeyHash());
        assertEquals(message, DatabaseConstants.BUCKET_CAPACITY * 2, found.size());
        assertEquals(message, DatabaseConstants.BUCKET_CAPACITY * 2, found.getAll(tuple.getKey()).size());
    }

    @Test(timeout = 1000)
    public void testGetBucket() {
        Bucket first = hashTable.getBucketsForTesting()[0];
        // Add some values
        Tuple tupleOne = new Tuple(0, "Australia", "27 Degrees");
        for(int i = 0; i < DatabaseConstants.BUCKET_CAPACITY * 2; i++) {
            hashTable.put(tupleOne);
        }
        // Test that they were placed in the right buckets
        // Test after global pointer
        Bucket second = hashTable.getBucketsForTesting()[1];
        assertEquals("Buckets after pointer should hash to same bucket", second, hashTable.getBucket(1)); // 0001
        assertEquals("Buckets after pointer should hash to same bucket", second, hashTable.getBucket(3)); // 0011
        // Test before global pointer
        Bucket third = hashTable.getBucketsForTesting()[2];
        assertEquals("Buckets before pointer should hash to different buckets", first, hashTable.getBucket(0)); // 0000
        assertEquals("Buckets before pointer should hash to different buckets", third, hashTable.getBucket(2)); // 0010
    }

}
