package sort;

import access.read.AccessIterator;
import access.write.AccessInserter;
import global.DBComponent;
import global.DatabaseConstants;
import heap.HeapFile;
import heap.HeapPage;
import heap.Tuple;
import heap.TupleDesc;

import java.io.IOException;
import java.util.*;

/**
 * Implementation of an External Merge Sort
 * - Implement this for Task 3 (HARD)
 * - You will need to implement the first round (sorted runs phase)
 * - You will then need to do a multiway merge to merge (M-1) runs together
 * - Once there is only one run left, provide access to iterate over this
 *
 * Note: Since AccessIterator only supports hasNext() and next(), we implement peeking functionality by creating
 *       an array to store the topmost value, referred to as peekedTuples in the methods below.
 *
 * References:
 * - The lecture slides provide a good reference for the method involved.
 * - The lecture notes found here: http://people.cs.aau.dk/~simas/aalg04/esort.pdf are also a good reference point
 */
public class ExternalMergeOrderBy extends AccessIterator implements DatabaseConstants {

    private TupleDesc schema;
    private Comparator<Tuple> tupleComparator;
    private AccessIterator iterator;

	private String[] cols;

    /**
     * Constructor for the ExternalMergeOrderBy
     * - Once this constructor is finished, it is expected that `iterator` will point to an AccessIterator
     *   over a list of sorted elements
     * - Thus you will need to do your sort & merge here
     * @param rows
     * @param columns
     */
    public ExternalMergeOrderBy(AccessIterator rows, final String[] columns) {
        schema = rows.getSchema();
        tupleComparator = new ColumnComparator(columns);

		cols = columns;
        /**
         * use mergeSorted Runs and produceSortedSuns to populate the iterator with the sorted values.
         *
         * You can use HeapFile.iterator() to get an iterator over the last file you have merged
         *
         * You may also wish to avoid sorting if there are no columns to sort over. e.g. columns.length == 0
         */

    }

    /**
     * Constructor for testing. Don't use this.
     */
    protected ExternalMergeOrderBy(TupleDesc schema, final String[] columns) {
        this.schema = schema;
        this.tupleComparator = new ColumnComparator(columns);

		cols = columns;
    }


    /**
     * Reads all pages into memory and divides them into groups of B (MERGE_SORT_BUFFER_FRAMES) pages.
     * Each of these groups is sorted (in memory) and then written to disk. (though do one at a time)
     * Since we don't know the number of records that we have in total, we can do some crafty mathematics
     * to work out the maximum number of tuples that we can fit in B pages.
     *
     * To simplify matters, we will store the sorted blocks in a HeapFile.
     * You will want to create a new file to store this, try using:
     * `HeapFile file = new HeapFile(DBComponent.CATALOG().findNameOfSchema(schema), true);`
     * to create the file with the appropriate schema of the iterator
     *
     * To insert records into this file, try using:
     *      try(AccessInserter fileInserter = file.inserter()) {
     *          fileInserter.insert(...next tuple to insert...);
     *`     }
     *
     *
     * @param rows iterator over a whole relation/table
     * @return list of sorted runs
     */
    protected List<HeapFile> produceSortedRuns(AccessIterator rows) {
        // Crafty mathematics to work out the maximum number of tuples per block
        int spaceOnPage = HeapPage.PAGE_SIZE- HeapPage.RECORD_START_INDEX;
        int tuplesOnPage = (int)(spaceOnPage / (double) schema.getTupleLength());
        int tuplesPerBlock =  tuplesOnPage * MERGE_SORT_BUFFER_FRAMES;
        // Some helpful suggestions - you will obviously want a list to store the tuples while you load them into memory
        List<Tuple> tuples = new ArrayList<Tuple>();
        // Also probably want a list of HeapFiles to store the blocks that you have made
        List<HeapFile> blocks = new ArrayList<HeapFile>();

		int count = 0;

        // Iterate over each row to produce the sorted runs
        while(rows.hasNext()) {
			count++;
			tuples.add(rows.next());

			/* divide into blocks */
			if (count >= tuplesPerBlock) {

				HeapFile file = new HeapFile(DBComponent.
						CATALOG().findNameOfSchema(schema), true);

				try (AccessInserter fileInserter = file.inserter()) {

					/* sort */
					Collections.sort(tuples, tupleComparator);

					for (Tuple t : tuples ) {
						fileInserter.insert(t);
					}
				}

				blocks.add(file);

				tuples.clear();
				count = 0;
			}
        }

		/* last block */
		if (tuples.size() != 0) {
			HeapFile file = new HeapFile(DBComponent.CATALOG().
									findNameOfSchema(schema), true);

			try (AccessInserter fileInserter = file.inserter()) {

				Collections.sort(tuples, tupleComparator);

				for (Tuple t : tuples ) {
					fileInserter.insert(t);
				}
			}

			blocks.add(file);
			tuples.clear();
		}

		return blocks;
	}

    /**
     * Merges groups of (M-1) HeapFiles together.
     * You should take the List of runs, divide them into (M-1) groups, and perform a multi-way merge on each group
     * For each of these groups, you should write the merged result out to file as you go.
     *
     * M is the MERGE_SORT_BUFFER_FRAMES variable
     *
     * You will want to create a new file to store this, try using:
     * `HeapFile mergedFile = new HeapFile(DBComponent.CATALOG().findNameOfSchema(schema), true);`
     * to create the file with the appropriate schema of the iterator
     *
     * To insert records into this file, try using:
     *      try(AccessInserter mergedFileInserter = mergedFile.inserter()) {
     *          mergedFileInserter.insert(...next tuple to insert...);
     *`     }
     *
     * Look at using pickNextTuple as a way to find the next tuple to add.
     * You will need to use this as part of multi-way merging.
     *
     * Also! Since there is no peek functionality in an AccessIterator, you will need to cache to topmost elements of each
     * AccessIterator in a Tuple[]
     *
     * @param runs the full list of runs from the previous round
     * @return a list of sorted heapfiles, merged in the method
     */
    protected List<HeapFile> mergeSortedRuns(List<HeapFile> runs) {
        if(runs.size() < 2) return runs;

		HeapFile mergedFile = new HeapFile(DBComponent.CATALOG().
								findNameOfSchema(schema), true);

		int idx = 0;
		AccessIterator[] unmergedRuns = new AccessIterator[runs.size()];
		Tuple[] topTuples  = new Tuple[runs.size()];

		for (HeapFile hf : runs) {
			unmergedRuns[idx] = hf.iterator();
			topTuples[idx] = null;

			if (unmergedRuns[idx].hasNext()) {
				topTuples[idx] = unmergedRuns[idx].next();
			}

			idx++;
		}

		try (AccessInserter mergedFileInserter = mergedFile.inserter()) {
			while (hasNextInList(topTuples)) {
				mergedFileInserter.insert(pickNextTuple(unmergedRuns, 
							topTuples));
			}
		}

		runs.clear();
		runs.add(mergedFile);

        return runs;
    }

    /**
     * Finds the next tuple to merged. I.e. finds the minimum tuple out of all the current sorted iterators.
     * Since we know these iterators are sorted, this tuple should be added to the end of our new file once we return
     * @param unmergedRuns the list of access iterators for this run
     * @param peekedTuples array of tuples representing the topmost elements of the access iterators
     */
    protected Tuple pickNextTuple(AccessIterator[] unmergedRuns, Tuple[] peekedTuples) {
        int index = findMinimum(peekedTuples);
        Tuple nextTuple = peekedTuples[index];
        // Replaces the tuple that we've just removed with the next one from the iterator if it has another value
        if (unmergedRuns[index].hasNext()) {
            peekedTuples[index] = unmergedRuns[index].next();
        }
        else {
            peekedTuples[index] = null;
        }
        return nextTuple;
    }

    /**
     * Checks if there is another tuple to be added / merged. If there is, then we need to continue so we can add this
     * to the merged run.
     */
    private boolean hasNextInList(Tuple[] peekedTuplesList) {
        for(Tuple i : peekedTuplesList) {
            if(i != null)
                return true;
        }
        return false;
    }

    /**
     * Finds the minimum / next tuple in a list of tuples to return.
     * Assumes null entries are exhausted iterators and ignores them; returns the next minimum tuple;
     */
    protected int findMinimum(Tuple[] current) {
        int i = 0;
        while(i < current.length && current[i] == null) {i++;}
        int minimum = i++;
        for(; i < current.length; i++) {
            if(current[i] == null) continue;
            if(tupleComparator.compare(current[i], current[minimum]) < 0) {
                minimum = i;
            }
        }
        return minimum;
    }


    @Override
    public void close() {
        iterator.close();
    }

    @Override
    public TupleDesc getSchema() {
        return schema;
    }

    @Override
    public boolean hasNext() {
        return iterator.hasNext();
    }

    @Override
    public Tuple next() {
        return iterator.next();
    }
}
