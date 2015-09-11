package access.write;

import heap.Tuple;
import heap.TupleDesc;

import java.io.Closeable;

/**
 * Interface to define a class that allows elements to be inserted in order
 */
public abstract class AccessInserter implements Closeable {

    public void insert(Object... values) {
        Tuple item = new Tuple(getSchema(), values);
        insert(item);
    }

    abstract public TupleDesc getSchema();

    abstract public void insert(Tuple item);

    abstract public boolean canInsert();

    @Override
    abstract public void close();

}
