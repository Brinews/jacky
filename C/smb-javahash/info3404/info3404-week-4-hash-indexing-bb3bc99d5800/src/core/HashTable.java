package core;

import java.util.*;

/**
 * Generic HashTable Class
 * - Used by the other hashing methods
 */
public abstract class HashTable implements DatabaseConstants {

    protected Bucket[] buckets;

    /**
     * Returns the bucket associated with the hash.
     * @param hash positive integer representing the hash
     * @return the bucket that corresponds to that hash value
     */
    abstract public Bucket getBucket(int hash);

    /**
     * Returns the tuple associated with the search key
     * Hint: to find the hash of the search key, you can just use `Math.abs(search_key.hashCode());`
     * Hint: you may find using the getBucket() method useful
     */
    public Tuple get(String search_key) {
        //TODO
		Bucket bkt = getBucket(Math.abs(search_key.hashCode()));

		if (null != bkt) return bkt.get(search_key);

		return null;
    }

    /**
     * Adds an entry to this HashTable
     * - Needs to find the bucket to insert the entry into
     * - If the bucket is full, needs to insert the entry into the next free overflow page of that bucket
     */
    public void put(Tuple entry) {
        //TODO
		Bucket bkt = getBucket(entry.getKeyHash());

		if (null != bkt) {
			if (!bkt.isFull()) bkt.add(entry);
			else {
				if (bkt.hasNextBucket()) {
					bkt.getNextBucket().add(entry);
				} else {
					bkt.setNextBucket(new Bucket());
					bkt.getNextBucket().add(entry);
				}
			}
		}
    }

    /**
     * Returns the number of tuples that are in this HashTable
     */
    public int numberOfTuples() {
        // Count the number of records from the unique buckets
        int size = 0;
        for(Bucket bucket : getUniqueBuckets()) {
                size += bucket.size();
        }
        return size;
    }

    /**
     * Returns the number of actual buckets that are stored in this HashTable (excluding nulls)
     */
    public int numberOfBuckets() {
        int count = 0;
        for(Bucket bucket : buckets) {
            if(bucket != null) {
                count ++;
            }
        }
        return count;
    }

    /**
     * Returns a Set of unique buckets in the HashTable
     */
    private Set<Bucket> getUniqueBuckets() {
        Set<Bucket> uniqueBuckets = new HashSet<Bucket>();
        for(Bucket bucket : buckets) {
            if(bucket != null) {
                uniqueBuckets.add(bucket);
            }
        }
        return uniqueBuckets;
    }

    /**
     * Returns a list of all tuples in the HashTable
     * @return
     */
    public List<Tuple> getAllTuples() {
        List<Tuple> tuples = new LinkedList<Tuple>();
        for(Bucket bucket : getUniqueBuckets()) {
            tuples.addAll(bucket.getAllElements());
        }
        return tuples;
    }

    /**
     * Only used for testing, you should not sure this method for your code
     */
    public Bucket[] getBucketsForTesting() {
        return buckets;
    }

}
