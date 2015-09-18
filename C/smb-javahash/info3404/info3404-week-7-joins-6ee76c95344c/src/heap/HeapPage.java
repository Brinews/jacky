package heap;

import access.read.DataPageIterator;
import disk.Page;
import global.DBComponent;
import global.DataPage;

import java.util.Iterator;

/**
 * Represents a page full of records
 */
public class HeapPage extends DataPage {

    public HeapPage(Page page) {
        this.data = page.getData();
    }

    public Iterator<Tuple> iterator() {
        return new DataPageIterator(this);
    }

    @Override
    public TupleDesc getSchema()  {
        return DBComponent.CATALOG().readSchema(getStringValue(SCHEMA_INDEX));
    }
}
