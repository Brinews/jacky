package statichashing;

import core.Bucket;
import core.HashTable;

import java.util.LinkedList;
import core.DatabaseConstants;

/**
 * Static Hash Table
 * - A fixed size HashTable
 * - Doesn't change size. If the buckets become full, overflow buckets are created
 * - Fixed size is defined in DatabaseConstants.HASHTABLE_SIZE
 */
public class StaticHashTable extends HashTable {

    /**
     * Need to initialise the Buckets array to HASHTABLE_SIZE and fill it will new buckets
     */
    public StaticHashTable() {
		buckets = new Bucket[DatabaseConstants.HASHTABLE_SIZE];

		for (int i = 0; i < DatabaseConstants.HASHTABLE_SIZE; i++)
			buckets[i] = new Bucket();
    }

    /**
     * Returns the bucket associated with the hash.
     * To find the corresponding bucket, we mod the hash by the size of our hash table
     * @param hash positive integer representing the hash
     * @return the bucket that corresponds to that hash value
     */
    @Override
    public Bucket getBucket(int hash) {
        if(hash < 0) throw new AssertionError("Hash must be positive");

		return buckets[hash%(DatabaseConstants.HASHTABLE_SIZE)];
    }
}
