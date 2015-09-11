package heap;

import disk.PageId;
import global.Type;

import java.util.Arrays;
import java.util.NoSuchElementException;

/**
 * Represents a physical row in the database
 * - Comprised of a TupleDesc (schema) and a list of values for the row (Object[])
 */
public class Tuple {

    private TupleDesc schema;
    private PageId pageId;
    private int tupleId;
    private Object[] row;

    /**
     * Creates a new tuple with the given schema
     * @param schema the schema of the tuple
     */
    public Tuple(TupleDesc schema) {
        this.schema = schema;
        this.row = new Object[schema.getNumFields()];
        this.tupleId = -1;
        this.pageId = new PageId();
    }

    /**
     * Creates a new tuple with the given schema
     * @param schema the schema of the tuple
     * @param values object array of the values in the tuple
     */
    public Tuple(TupleDesc schema, Object[] values) {
        this.schema = schema;

		this.row = new Object[schema.getNumFields()];

        for(int i = 0; i < values.length; i++) {
            setColumn(i, values[i]);
        }

        this.tupleId = -1;
        this.pageId = new PageId();
    }

    /**
     * Returns the schema of the tuple
     */
    public TupleDesc getSchema() {
        return schema;
    }

    /**
     * Resets the tuple with the given schema
     * Clears all values in the Object array
     * @param schema the schema to reset the tuple with
     */
    public void resetWithSchema(TupleDesc schema) {
        this.schema = schema;
        this.row = new Object[schema.getNumFields()];
        this.pageId = new PageId();
        this.tupleId = -1;
    }

    /**
     * Sets the value of the column in the tuple
     * @param name the name of the column to insert
     * @param value the value to put in the column
     */
    public void setColumn(String name, Object value) {
        int i = schema.getIndexFromName(name);
        setColumn(i, value);
    }

    /**
     * Sets the value of the column in the tuple
     * @param index the index-th column to insert into
     * @param value the value to put in the column
     */
    public void setColumn(int index, Object value) {
        Type columnType = schema.getFieldType(index);
        // Checks that Object's type matches column type
        if(!columnType.getTypeClass().isInstance(value)) {
            throw new AssertionError("Invalid Object Type");
        }
        row[index] = value;
    }

    /**
     * Gets the value of the column at i-th
     * @param i the index of the column to access
     * @return the value in the column
     */
    public Object getColumn(int i) {
        if(i < 0 || i >= row.length) {
            throw new AssertionError("Invalid Column Index");
        }
        return row[i];
    }

    /**
     * Gets the value of the column with the given name
     */
    public Object getColumn(String name) {
        return getColumn(getSchema().getIndexFromName(name));
    }

    /**
     * Returns the id (slot number) of the tuple (on it's page)
     */
    public int getTupleId() {
        return tupleId;
    }

    /**
     * Sets the id (slot number) of the tuple (on it's page)
     */
    public void setTupleId(int tupleId) {
        this.tupleId = tupleId;
    }

    /**
     * Returns the page id of the tuple
     */
    public PageId getPageId() {
        return pageId;
    }

    /**
     * Sets the page id of the tuple
     */
    public void setPageId(PageId pageId) {
        this.pageId = pageId;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof Tuple)) return false;

        Tuple tuple = (Tuple) o;

        // Probably incorrect - comparing Object[] arrays with Arrays.equals
        if (!Arrays.equals(row, tuple.row)) return false;
        if (schema != null ? !schema.equals(tuple.schema) : tuple.schema != null) return false;

        return true;
    }

    // Copies values from matching columns in the given tuple to this tuple
    // Can be used for joining and for projection
    public void copyFrom(Tuple old) {
        for(int i = 0; i < schema.getNumFields(); i++) {
            try {
                row[i] = old.getColumn(old.getSchema().getIndexFromName(schema.getFieldName(i)));
            }
            catch(NoSuchElementException e) {}
        }
    }

    public String toRow() {
        return Arrays.toString(row);
    }

    public boolean equals(Tuple other) {
        if (this == other) return true;
        TupleDesc schema = getSchema();
        return rowEquals(other.row);
    }

    public boolean rowEquals(Object... values) {
        if(row.length != values.length) return false;
        for(int i = 0; i < row.length; i++) {
            if(!getColumn(i).equals(values[i])) {
                return false;
            }
        }
        return true;
    }

    @Override
    public String toString() {
        return "Tuple{" +
                "schema=" + schema +
                ", pageId=" + pageId +
                ", tupleId=" + tupleId +
                ", row=" + Arrays.toString(row) +
                '}';
    }

    /**
     * Converts the current tuple into it's indexed companion.
     * Removes all other columns except those in it's new schema
     * @return this tuple, reset and populated according to newSchema
     */
    public Tuple convertToIndex(TupleDesc newSchema) {
        String indexed = newSchema.getFieldName(2);
        Object value = row[getSchema().getIndexFromName(indexed)];
        int pageId = getPageId().get();
        int tupleId = getTupleId();
        resetWithSchema(newSchema);
        setColumn("_page", pageId);
        setColumn("_recordId", tupleId);
        setColumn(indexed, value);
        return this;
    }
}
