package linearhashing;

import core.Bucket;
import core.HashTable;
import core.Tuple;

import java.util.List;
import core.DatabaseConstants;

/**
 * LinearHashTable
 * - Expanding size HashTable
 * - Keeps a pointer to the next bucket to split
 * - Keeps track of the current global level. Where size of the array is 2^G * N
 */
public class LinearHashTable extends HashTable {

    protected static final int INITIAL_BUCKETS = 2;
    protected int globalLevel;
    protected int next;

    /**
     * Initialises the buckets, initial number of buckets is INITIAL_BUCKETS, next bucket to split is buckets[0]
     */
    public LinearHashTable() {
        this.buckets = new Bucket[INITIAL_BUCKETS];
        for(int i = 0; i < INITIAL_BUCKETS; i++) {
            buckets[i] = new Bucket();
        }
        this.globalLevel = 0;
        this.next = 0;
    }

    /**
     * If the next bucket to split is less than the h_level(hash), then return the h_level+1(hash) bucket
     * If the next bucket to split is not less than h_level(hash), then we just return the h_level(hash) bucket
     */
    @Override
    public Bucket getBucket(int hash) {
        if(hash < 0) throw new AssertionError("Hash must be positive");

		int nHash = hash % (1<<(globalLevel+1));

		if (nHash < next) {
			return buckets[hash%((1<<(globalLevel+1)+next))];
		} else {
			return buckets[nHash];
		}
    }

    /**
     * 1. Inserts the entry into the appropriate bucket (you may want to use super.put(entry) to help you.
     * 2. If the HashTable has reached 80% capacity, then we should split the bucket pointed to by *next*
     * 3. If necessary, increase the global level of the HashTable
     *
     * Note: If you have done all the split's you can for a certain level (i.e. once next >= 2^level) you will need to
     *       double the directory (double the size of the array)
     */
    @Override
    public void put(Tuple entry) {
		super.put(entry);

		Bucket bkt = buckets[next];
		List<Tuple> ret = bkt.getAllElements();

		if (numberOfTuples() >= 0.8*DatabaseConstants.BUCKET_CAPACITY*buckets.length) {
			/* split *next* */

			Bucket[] nBuckets = new Bucket[buckets.length+1];
			int i;
			for (i = 0; i < buckets.length; i++) {
				nBuckets[i] = buckets[i];
			}

			nBuckets[next].clear();
			nBuckets[buckets.length] = new Bucket();

			buckets = new Bucket[nBuckets.length];
			for (i = 0; i < buckets.length; i++) {
				buckets[i] = nBuckets[i];
			}

			next = next + 1;

			/* update globalLevel */
			if (next > (1<<globalLevel)) {
				next = 0;
				globalLevel = globalLevel + 1;
			}

			/* redistribute */
			for (Tuple t : ret) {
				super.put(t);
			}
		}
    }
}
