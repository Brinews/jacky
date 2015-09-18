package join;

import access.read.AccessIterator;
import global.DatabaseConstants;
import heap.HeapPage;
import heap.Tuple;
import heap.TupleDesc;
import parser.JoinArgs;

import java.util.ArrayList;
import java.util.List;

/**
 * Abstract Join class that handles most of the default behaviour
 * See comments below for more information
 */
abstract class GenericJoin extends AccessIterator {


    // The schema of the tuple that is returned from the join (you don't need to worry about this)
    protected TupleDesc schema;
    // These are iterators over the left and right tables to merge.
    protected AccessIterator left;
    protected AccessIterator right;
    // The indexes of the columns to join on in the respective tuples.
    // To get the value to join a tuple on, we can do `tuple.getColumn(index)`
    // This would get us the value at index.
    protected int leftColumn;
    protected int rightColumn;

    /**
     * Constructor for generic join
     * @param left the left table to join
     * @param right the right table to join
     * @param condition the details of the join. e.g. on what columns, etc.
     */
    public GenericJoin(AccessIterator left, AccessIterator right, JoinArgs condition) {
        this.left = left;
        this.right = right;
        this.schema = TupleDesc.join(left.getSchema(), right.getSchema());
        // Finds the index of the columns to join on from their names
        leftColumn = left.getSchema().getIndexFromName(condition.getLeftColumn());
        rightColumn = right.getSchema().getIndexFromName(condition.getRightColumn());
    }

    /**
     * Joins a tuple together based on its join columns
     * @param left the left tuple to join
     * @param right the right tuple to join
     * @return the joined tuple
     */
    public Tuple joinTuple(Tuple left, Tuple right) {
        Tuple tuple = new Tuple(getSchema());
        tuple.copyFrom(left);
        tuple.copyFrom(right);
        return tuple;
    }

    /**
     * Returns a list of tuples. Or, the next block of tuples to iterate over.
     * This can be used for the Block Nested Loop algorithm to get the next block of rows to loop over.
     * @param rows the iterator to load the next B pages from
     * @return a list of tuples from B pages, or an empty list
     */
    public static List<Tuple> getNextBlock(AccessIterator rows) {
        int spaceOnPage = HeapPage.PAGE_SIZE- HeapPage.RECORD_START_INDEX;
        int tuplesOnPage = (int)(spaceOnPage / (double) rows.getSchema().getTupleLength());
        int tuplesPerBlock =  tuplesOnPage * (DatabaseConstants.JOIN_BUFFER_FRAMES);
        ArrayList<Tuple> block = new ArrayList<>(tuplesPerBlock);
        while(rows.hasNext() && block.size() < tuplesPerBlock) {
            block.add(rows.next());
        }
        return block;
    }

    /**
     * You can ignore the below methods
     */
    @Override
    public void close() {
        left.close();
        right.close();
    }

    @Override
    public TupleDesc getSchema() {
        return schema;
    }

    @Override
    public void reset() {
        throw new UnsupportedOperationException("Cannot reset");
    }

    @Override
    public void savePosition() {
        throw new UnsupportedOperationException("Joins don't support saving the position");
    }

    @Override
    public void loadPosition() {
        throw new UnsupportedOperationException("Joins don't support loading the position");
    }
}
