package extendiblehashing;

import core.Bucket;
import core.HashTable;
import core.Tuple;

import java.util.List;

/**
 * ExtendibleHashTable
 * - Dynamic Hash Table
 * - Once a Bucket becomes full, we split the bucket
 * - If the bucket's local level == global level, then we need to split the whole directory (double the array)
 */
public class ExtendibleHashTable extends HashTable {

    private static int INITIAL_BUCKETS = 2;
    private int globalLevel;

    /**
     * Initialises the global level to 2, and creates the first two buckets
     */
    public ExtendibleHashTable() {
        globalLevel = 0;
        buckets = new Bucket[INITIAL_BUCKETS];
        for(int i = 0; i < INITIAL_BUCKETS; i++) {
            buckets[i] = new Bucket();
        }
    }

    /**
     * Finds the bucket to return by h(key) mod(2^i M) where M is the # initial buckets
     */
    @Override
    public Bucket getBucket(int hash) {
        if(hash < 0) throw new AssertionError("Hash must be positive");

		return buckets[hash%(1<<(globalLevel+1))];
    }

    /**
     * Add an entry to the Extendible Hash Table
     * - If there is already a value in the page that matches your search key, just insert the value
     * - If the Bucket is full, then you will need to split the Bucket
     *      - Note: Splitting a bucket does not always cause a doubling of the directory; this only happens when the
     *              local level of a Bucket is the same as the global level of the page
     * - Otherwise just insert the value
     */
    @Override
    public void put(Tuple entry) {
		Bucket bkt = getBucket(entry.getKeyHash());

		if (!bkt.isFull()) {
			bkt.add(entry);
		} else if (bkt.get(entry.getKey()) != null && 
				(!bkt.hasNextBucket() || 
			(bkt.hasNextBucket() && !bkt.getNextBucket().isFull()))) {
			/* duplicate key insert into overflow bucket */
			if (!bkt.hasNextBucket()) {
				bkt.setNextBucket(new Bucket());
				bkt.getNextBucket().add(entry);
			}
			else if (bkt.hasNextBucket() 
					&& !bkt.getNextBucket().isFull()) {
				bkt.getNextBucket().add(entry);
			} 
		} else {
			/* need split */
			int orgHash = entry.getKeyHash()%(1<<(globalLevel+1));
			int level = bkt.getLocalLevel();

			List<Tuple> allTuple = bkt.getAllElements();

			if (globalLevel < level+1) {
				/* double directory */
				globalLevel = globalLevel + 1;

				Bucket[] nBuckets = new Bucket[1<<(globalLevel+1)];

				for (int i = 0; i < (1<<(globalLevel+1)); i++) {
					nBuckets[i] = buckets[i%(1<<globalLevel)];
				}

				buckets = new Bucket[1<<(globalLevel+1)];
				for (int i = 0 ; i < (1<<(globalLevel+1)); i++)
					buckets[i] = nBuckets[i];

				level = level + 1;
			}

			int newNo;

			if (orgHash < (1<<globalLevel))
				newNo = orgHash + (1<<globalLevel);
			else
				newNo = orgHash - (1<<globalLevel);

			buckets[newNo] = new Bucket();
			buckets[orgHash].clear();

			buckets[newNo].setLocalLevel(level);
			buckets[orgHash].setLocalLevel(level);

			/* reorgnize */
			allTuple.add(entry);
			for (Tuple t : allTuple) {
				Bucket pbkt = getBucket(t.getKeyHash());
				pbkt.add(t);
			}
		}
    }
}
