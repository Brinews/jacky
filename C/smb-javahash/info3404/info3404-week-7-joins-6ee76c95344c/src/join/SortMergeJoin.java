package join;

import access.read.AccessIterator;
import heap.Tuple;
import parser.JoinArgs;

import java.util.Comparator;
import java.util.NoSuchElementException;

/**
 * Implements a Sort-Merge Join.
 * ==============================
 * For this task, you may assume both iterators are in sorted order. However you may not assume is that the rows in
 * either relation are unique.
 *
 * This is a great gif to visualise how the merge sort works:
 *      http://sqlity.net/wp-content/uploads/2012/12/merge-join-algorithm.gif
 *
 * Since you won't have unique join keys as in the GIF, you may need to make use of savePosition and loadPosition in
 * AccessIterator to handle the special cases that arise. e.g. what if there are multiple entries for the same key
 * in BOTH join iterators.
 *
 * To check if two tuples are equal, you can use:
 *  `tupleOne.getColumn(leftColumn).equals(tupleTwo.getColumn(rightColumn)) `
 *
 * There have been some changes to the AccessIterator. You can now:
 *    - Reset the iterator back to the start (you may find this useful for looping)
 *    - Save the current position of the iterator, and revert back to it if you need to (only relevant to hard)
 *
 * Hint: since you can't (or shouldn't) join in the constructor, you will need some way of knowing what tuple you are up
 * to. You may find you will need some temporary variables, e.g. to store the current tuple you are iterating over
 */
public class SortMergeJoin extends GenericJoin {

    private Tuple next;
	private JoinComparator comp;

	private Tuple leftRow = null, rightRow = null;
	private int flag = 0;
	private Tuple prevRight;

    public SortMergeJoin(AccessIterator left, AccessIterator right, JoinArgs condition) {
        super(left, right, condition);
        next = null;

		comp = new JoinComparator(condition.getLeftColumn(),
				condition.getRightColumn());

		if (left.hasNext()) leftRow = left.next();
		if (right.hasNext()) rightRow = right.next();
    }

    /**
     * The bulk of the join will be done in the hasNext method. This is because in order to know whether there is
     * another joined tuple, you will need to find one first.
     * @return true if there is another joined tuple in the file, false if there isn't
     */
    @Override
    public boolean hasNext() {
        if(next != null) return true;

		while (leftRow != null && rightRow != null) {

			int ct = comp.compare(leftRow, rightRow);

			if (ct == 0) {
				next = joinTuple(leftRow, rightRow);

				if (flag == 0) {
					prevRight = rightRow;
					right.savePosition();
					flag = 1;
				}

				if (right.hasNext()) {
					rightRow = right.next();
				} else { 

					if (left.hasNext()) leftRow = left.next();
					else leftRow = null;

					right.loadPosition();
					rightRow = prevRight;
				}

				return true;
			}

			if (ct < 0) {

				if (flag == 1) {
					right.loadPosition();
					rightRow = prevRight;
					flag = 0;
				}

				if (left.hasNext()) leftRow = left.next();
				else leftRow = null;
			}

			if (ct > 0) {
				if (right.hasNext()) rightRow = right.next();
				else rightRow = null;
			}
		}

        return false;
    }

    /**
     * This should return the next joined tuple.
     * To join a tuple, see the GenericJoin class.
     */
    @Override
    public Tuple next() {
        if(!hasNext())
            throw new NoSuchElementException();
        Tuple temp = next;
        next = null;
        return temp;
    }
}
