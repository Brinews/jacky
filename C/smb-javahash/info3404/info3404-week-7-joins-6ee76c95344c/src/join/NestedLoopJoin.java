package join;

import access.read.AccessIterator;
import heap.Tuple;
import parser.JoinArgs;

import java.util.NoSuchElementException;

/**
 * Implements a Nested Loop Join.
 * ==============================
 * It's algorithm is essentially:
 * for tuple r in left:
 *  for tuple s in right:
 *      if r can be joined with s:
 *          output join(r, s)
 *
 * To check if two tuples are equal, you can use:
 *  `tupleOne.getColumn(leftColumn).equals(tupleTwo.getColumn(rightColumn)) `
 *
 * Hint: since you can't (or shouldn't) join in the constructor, you will need some way of knowing what tuple you are up
 * to. You may find you will need some temporary variables to store the current tuple you are iterating over, for example
 */
public class NestedLoopJoin extends GenericJoin {

    private Tuple next;
	private JoinComparator colComp;
	private Tuple leftNow;

    public NestedLoopJoin(AccessIterator left, AccessIterator right, JoinArgs condition) {
        super(left, right, condition);
		colComp = new JoinComparator(condition.getLeftColumn(),
				condition.getRightColumn());
        next = null;
		leftNow = left.next();
    }

    /**
     * The bulk of the join will be done in the hasNext method. This is because in order to know whether there is
     * another joined tuple, you will need to find one first.
     * @return true if there is another joined tuple in the file, false if there isn't
     */
    @Override
    public boolean hasNext() {
        if(next != null) return true;

		while (leftNow != null) {

			while (right.hasNext()) {
				Tuple rightRow = right.next();

				if (colComp.compare(leftNow, rightRow) == 0) {
					next = joinTuple(leftNow, rightRow);
					return true;
				}

			}

			if (left.hasNext()) {
				leftNow = left.next();
			}
			else {
				return false;
			}

			right.reset();
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
