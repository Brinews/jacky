package utilities;

import access.read.AccessIterator;
import heap.Tuple;
import heap.TupleDesc;

/**
 * Ensures no large amount of work is being done in the constructor (by always returning true and a tuple)
 */
public class ConstructorBuster extends AccessIterator {

    private TupleDesc schema;
    private Tuple tuple;

    public ConstructorBuster() {
        schema = new TupleDesc().addString("name");
        tuple = new Tuple(schema, new Object[]{"Oppsies"});
    }

    @Override
    public void close() {}

    @Override
    public TupleDesc getSchema() {
        return schema;
    }

    @Override
    public void reset() {}

    @Override
    public void savePosition() {}

    @Override
    public void loadPosition() {}

    @Override
    public boolean hasNext() {
        return true;
    }

    @Override
    public Tuple next() {
        return tuple;
    }
}
