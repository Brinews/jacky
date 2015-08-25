package extendiblehashing;

import core.Bucket;
import core.DatabaseConstants;
import core.HashTable;
import core.Tuple;
import org.junit.Before;
import org.junit.Test;

import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;

import static org.junit.Assert.*;

public class ExtendibleHashTableTest {

    private HashTable hashTable;
    private String[] countries = new String[]{"Australia", "Austria", "Belgium", "Cambodia", "China", "England",
            "France", "Germany", "Greece", "India", "Ireland", "Korea", "Mexico", "Nepal", "New Zealand", "Portugal",
            "Spain", "Tibet", "USA", "Vietnam" };

    @Before
    public void setUp() throws Exception {
        hashTable = new ExtendibleHashTable();
    }

    @Test(timeout = 1000)
    public void testInitialSize() throws Exception {
        String message = "The ExtendibleHashTable should start by having 2 buckets";
        assertEquals(message, 2, hashTable.numberOfBuckets());
    }

    @Test(timeout = 1000)
    public void testAdd() throws Exception {
        List<Tuple> countryList = new LinkedList<Tuple>();
        for(String country: countries) {
            Tuple entry = new Tuple(0, country, "21 Degrees");
            countryList.add(entry);
            hashTable.put(entry);
        }
        String message = "You are missing some entries. Make sure you aren't overwriting a Bucket";
        assertEquals(message, countries.length, hashTable.numberOfTuples());
        assertEquals("You don't have the right amount of buckets.", 4, hashTable.numberOfBuckets());
        assertTrue("Expected tuples and actual don't match", twoListsAreEqual(countryList, hashTable.getAllTuples()));
        Bucket[] expected = hashTable.getBucketsForTesting();
        assertSame("These pages shouldn't have been split since they didn't get full", expected[0], expected[2]);
        assertEquals("Make sure you update the local level", 0, hashTable.getBucket(0).getLocalLevel());
        assertNotSame("These pages should have been split", expected[1], expected[3]);
        assertEquals("Make sure you update the local level", 1, hashTable.getBucket(1).getLocalLevel());

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
        String message = "Make sure you're not splitting the page when we're adding a duplicate";
        assertEquals(message, hashTable.numberOfBuckets(), 2);
        assertEquals(message, DatabaseConstants.BUCKET_CAPACITY * 2, hashTable.numberOfTuples());
        Bucket found = hashTable.getBucket(tuple.getKeyHash());
        assertEquals(message, DatabaseConstants.BUCKET_CAPACITY * 2, found.size());
        assertEquals(message, DatabaseConstants.BUCKET_CAPACITY * 2, found.getAll(tuple.getKey()).size());
    }

    @Test(timeout = 1000)
    public void testGetBucket() {
        Bucket[] expected = hashTable.getBucketsForTesting();
        assertEquals("Initially everything will be put in the first two buckets", expected[0], hashTable.getBucket(0));
        assertEquals("Initially everything will be put in the first two buckets", expected[1], hashTable.getBucket(1));
        assertEquals("Make sure you update the local level", 0, hashTable.getBucket(0).getLocalLevel());
        for(int i = 0; i < DatabaseConstants.BUCKET_CAPACITY*3; i++) {
            Tuple tuple = new Tuple(0, "Australia"+i, "27 Degrees");
            hashTable.put(tuple);
        }
        expected = hashTable.getBucketsForTesting();
        assertEquals("Buckets are expected to be split once full.", expected[0], hashTable.getBucket(0));
        assertEquals("Buckets are expected to be split once full", expected[1], hashTable.getBucket(1));
    }

    @Test(timeout = 1000)
    public void testGetSplitWithWrapAround() {
        try {
            for (int i = 0; i < DatabaseConstants.BUCKET_CAPACITY * 3; i++) {
                Tuple tuple = new Tuple(0, "Australia" + i, "27 Degrees");
                hashTable.put(tuple);
            }
        }
        catch(ArrayIndexOutOfBoundsException e) {
            fail("Consider the case when the element that causes the split is in position [3] of the array. "
                    + "You may need to use %modulus bucket.length to get the right position of the new bucket to add");
        }
    }

    private boolean twoListsAreEqual(List<?> left, List<?> right) {
        HashSet<?> leftSet = new HashSet<Object>(left);
        HashSet<?> rightSet = new HashSet<Object>(right);
        return leftSet.equals(rightSet);
    }
}