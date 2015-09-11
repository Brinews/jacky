package projection;

import access.read.AccessIterator;
import global.DBComponent;
import heap.Tuple;
import heap.TupleDesc;

import java.util.UUID;

/**
 * Applies a projection to an AccessIteration over the specified columns
 * - It creates a new schema over the projected columns using the existing schema (since we need to know the data types)
 * - It then just copies the data from the next tuple, to a new reduced tuple and returns that instead.
 */
public class Projection extends AccessIterator {

    private AccessIterator iterator;
    private TupleDesc schema;

    /**
     * Takes in an iterator of the tuples to apply the projection to, and a String[] of the columns to project over
     */
    public Projection(AccessIterator rows, String[] columns) {
        this.iterator = rows;
        this.schema = rows.getSchema().reduceIndex(columns);
        DBComponent.CATALOG().createSchema(schema, UUID.randomUUID().toString().substring(0, 30));
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
        Tuple tuple = iterator.next();
        Tuple reducedTuple = new Tuple(schema);
        reducedTuple.copyFrom(tuple);
        return reducedTuple;
    }
}
