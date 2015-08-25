package core;

import java.util.LinkedList;
import java.util.List;

/**
 * Stores a list of entries in a HashTable
 * Number of entries that it can store is limited by BUCKET_CAPACITY. You shouldn't change this.
 * In reality, a bucket will be similar to a DataPage that you used last week.
 */
public class Bucket implements DatabaseConstants {

    private Tuple[] entries;
    private int size;
    private Bucket overflow;
    // Used for Extensible
    private int localLevel;

    public Bucket() {
        this.entries = new Tuple[BUCKET_CAPACITY];
        this.size = 0;
        this.localLevel = 0;
    }

    /**
     * Returns true if there are no tuples stored in this bucket
     */
    public boolean isEmpty() {
        return size == 0;
    }

    /**
     * Returns true if there is no more space in the this bucket
     */
    public boolean isFull() {
        return size >= BUCKET_CAPACITY;
    }

    /**
     * Returns true if this bucket has an overflow page
     */
    public boolean hasNextBucket() {
        return overflow != null;
    }

    /**
     * Returns the overflow page of this bucket (if it has one)
     */
    public Bucket getNextBucket() {
        return overflow;
    }

    /**
     * Sets the overflow page of this bucket
     */
    public void setNextBucket(Bucket nextBucket) {
        this.overflow = nextBucket;
    }

    /**
     * Returns the total number of records in this Bucket (including overflow Buckets)
     */
    public int size() {
        return hasNextBucket() ? size + overflow.size() : size;
    }

    /**
     * Removes all the records from this bucket
     */
    public void clear() {
        size = 0;
        overflow = null;
    }

    /**
     * Adds an entry to this bucket
     */
    public boolean add(Tuple entry) {
        if(isFull()) return false;
        entries[size] = entry;
        size += 1;
        return true;
    }

    /**
     * Gets an element from this bucket given a search_key
     * Compares the search_key against the key of each tuple in this bucket (and it's overflow) until it finds a match
     * @return the first tuple that matches search_key, else null
     */
    public Tuple get(String search_key) {
        // TODO
		for (int i = 0; i < size; i++) {
			Tuple t = entries[i];
			if (t.compareTo(search_key) == 0)
				return t;
		}

		if (null != overflow) {
			return overflow.get(search_key);
		}

		return null;
    }

    /**
     * Returns a list of all tuples stored in this bucket (including overflow pages)
     */
    public List<Tuple> getAllElements() {
        // TODO
		List<Tuple> allTuples = new LinkedList<Tuple>();

		for (int i = 0; i < size; i++)
			allTuples.add(entries[i]);

		if (null != overflow)
			allTuples.addAll(overflow.getAllElements());

		return allTuples;
    }

    /**
     * Returns a list of all tuples matching the search key (including overflow pages)
     */
    public List<Tuple> getAll(String search_key) {
        // TODO
		List<Tuple> matchTuples = new LinkedList<Tuple>();

		for (int i = 0; i < size; i++) {
			if (entries[i].compareTo(search_key) == 0)
				matchTuples.add(entries[i]);
		}

		if (null != overflow) {
			matchTuples.addAll(overflow.getAll(search_key));
		}

		return matchTuples;
    }

    /**
     * Gets the local level of this page
     */
    public int getLocalLevel() {
        return localLevel;
    }

    /**
     * Sets the local level of this page
     */
    public void setLocalLevel(int localLevel) {
        this.localLevel = localLevel;
    }
}
