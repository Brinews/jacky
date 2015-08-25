package statichashing;

import core.Bucket;
import core.DatabaseConstants;
import core.HashTable;
import core.Tuple;
import org.junit.Before;
import org.junit.Test;

import java.util.HashSet;

import static org.junit.Assert.*;

public class StaticHashTableTest {

    private HashTable hashTable;

    @Before
    public void setUp() throws Exception {
        hashTable = new StaticHashTable();
    }

    @Test(timeout = 1000)
    public void testInitialSize() throws Exception {
        String message = "Make sure you use DatabaseConstants.HASHTABLE_SIZE as the number of initial buckets for the StaticHashTable";
        assertEquals(message, DatabaseConstants.HASHTABLE_SIZE, hashTable.numberOfBuckets());
    }

    @Test(timeout = 1000)
    public void testAllUnique() throws Exception {
        HashSet<Bucket> setOfBuckets = new HashSet<Bucket>();
        for(Bucket bucket : hashTable.getBucketsForTesting()) {
            setOfBuckets.add(bucket);
        }
        String message = "Make sure you're initialising a new bucket for each slot in the array";
        assertEquals(message, DatabaseConstants.HASHTABLE_SIZE, setOfBuckets.size());
    }

    @Test(timeout = 1000)
    public void testAdd() throws Exception {
        Tuple tuple = new Tuple(0, "Australia", "27 Degrees");
        hashTable.put(tuple);
        Tuple found = hashTable.get(tuple.getKey());
        String message = "Can't get an element from the HashTable. Check that you are using tuple.getKeyHash().";
        assertSame(message, tuple, found);
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
        for(int i = 0; i < DatabaseConstants.BUCKET_CAPACITY * 2; i++) {
            hashTable.put(tuple);
        }
        String message = "Make sure you're handing bucket overflows when getting elements correctly";
        assertEquals(message, DatabaseConstants.BUCKET_CAPACITY * 2, hashTable.numberOfTuples());
        Bucket found = hashTable.getBucket(tuple.getKeyHash());
        assertEquals(message, DatabaseConstants.BUCKET_CAPACITY * 2, found.size());
        assertEquals(message, DatabaseConstants.BUCKET_CAPACITY * 2, found.getAll(tuple.getKey()).size());
    }
}