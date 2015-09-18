package access.read;

import heap.Tuple;
import heap.TupleDesc;

import java.io.Closeable;
import java.util.Iterator;

/**
 * Generic Iterator Class to be used by the Database for Access patterns
 */
public abstract class AccessIterator implements Iterator<Tuple>, Closeable {

    public abstract void close();

    @Override
    public void remove() {
        throw new UnsupportedOperationException();
    }

    public abstract TupleDesc getSchema();

    public abstract void reset();

    /**
     * Saves the current position of the iterator
     */
    public abstract void savePosition();

    /**
     * Loads the last saved position of the iterator
     */
    public abstract void loadPosition();
}
