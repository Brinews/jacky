package join;

import access.read.AccessIterator;
import heap.Tuple;
import parser.JoinArgs;

import java.util.List;
import java.util.NoSuchElementException;

/**
 * Implements a Blocked Nested Loop Join.
 * ==============================
 * It's algorithm is essentially:
 * for block R in left:
 *   for block S in right:
 *     for tuple r in R:
 *       for tuple s in S:
 *         if r can be joined with s:
 *           output join(r, s)
 *
 * To check if two tuples are equal, you can use:
 *  `tupleOne.getColumn(leftColumn).equals(tupleTwo.getColumn(rightColumn)) `
 *
 * There have been some changes to the AccessIterator. You can now:
 *    - Reset the iterator back to the start (you may find this useful for looping)
 *    - Save the current position of the iterator, and revert back to it if you need to (only relevant to hard)
 *
 * Hint: since you can't (or shouldn't) join in the constructor, you will need some way of knowing what tuple you are up
 * to. You may find you will need some temporary variables to store the current tuple you are iterating over, for example
 */
public class BlockNestedLoopJoin extends GenericJoin {

    private Tuple next;
	private JoinComparator comp;
	private List<Tuple> leftBlock, rightBlock;
	private int leftIndex, rightIndex;
	private String leftKey, rightKey;

    public BlockNestedLoopJoin(AccessIterator left, AccessIterator right, JoinArgs condition) {
        super(left, right, condition);

		comp = new JoinComparator(condition.getLeftColumn(),
				condition.getRightColumn());

        next = null;

		leftBlock = getNextBlock(left);
		rightBlock = getNextBlock(right);

		leftKey = condition.getLeftColumn();
		rightKey = condition.getRightColumn();

		//System.out.println(leftBlock.size() + ": " + rightBlock.size());

		leftIndex = rightIndex = 0;
    }

    /**
     * The bulk of the join will be done in the hasNext method. This is because in order to know whether there is
     * another joined tuple, you will need to find one first.
     *
     * The method in this class, getNextBlock(..) should help you generate B sized blocks of tuples from your iterators
     * @return true if there is another joined tuple in the file, false if there isn't
     */
    @Override
    public boolean hasNext() {
        if(next != null) return true;

		while (leftIndex < leftBlock.size()) {

			Tuple leftRow = leftBlock.get(leftIndex);

			while (rightIndex < rightBlock.size()) {
				
				Tuple rightRow = rightBlock.get(rightIndex);
				rightIndex++;

				/*
				if (leftRow.getColumn(leftKey).equals(
							rightRow.getColumn(rightKey))) {
				*/
				if (comp.compare(leftRow, rightRow) == 0) {
					next = joinTuple(leftRow, rightRow);

					return true;
				}
			}

			rightIndex = 0;
			leftIndex++;

			if (leftIndex == leftBlock.size()) {

				rightBlock = getNextBlock(right);

				if (rightBlock.size() == 0) {
					leftBlock = getNextBlock(left);

					if (leftBlock.size() > 0) {
						right.reset();
						rightBlock = getNextBlock(right);
					}
				}

				leftIndex = 0;
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
