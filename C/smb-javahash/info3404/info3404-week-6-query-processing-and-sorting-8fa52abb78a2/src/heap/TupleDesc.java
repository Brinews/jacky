package heap;

import global.Type;

import java.util.ArrayList;
import java.util.List;
import java.util.NoSuchElementException;

/**
 * TupleDescriptor Class for the Database
 * Essentially a "schema" object. Defines the types and field names of the tuples in the database
 */
public class TupleDesc {

    private List<TupleDescItem> columns;

    /**
     * Creates a new tuple descriptor
     */
    public TupleDesc() {
        columns = new ArrayList<>();
    }

    /**
     * Adds a string column with 'name' to the TupleDesc
     */
    public TupleDesc addString(String name) {
        columns.add(new TupleDescItem(Type.STRING, name));
        return this;
    }

    /**
     * Adds a integer column with 'name' to the TupleDesc
     */
    public TupleDesc addInteger(String name) {
        columns.add(new TupleDescItem(Type.INTEGER, name));
        return this;
    }

    /**
     * Adds a double column with 'name' to the TupleDesc
     */
    public TupleDesc addDouble(String name) {
        columns.add(new TupleDescItem(Type.DOUBLE, name));
        return this;
    }

    /**
     * Adds a boolean column with 'name' to the TupleDesc
     */
    public TupleDesc addBoolean(String name) {
        columns.add(new TupleDescItem(Type.BOOLEAN, name));
        return this;
    }

    /**
     * Returns the fixed length a tuple with this schema would have.
     * Iterates over the types in the TupleDesc to work out maximum length
     */
    public int getTupleLength() {
        int size = 0;
        for(TupleDescItem item : columns) {
            size += item.getType().getLen();
        }
        return size;
    }

    /**
     * Returns the number of columns / fields in the TupleDesc
     */
    public int getNumFields() {
        return columns.size();
    }

    /**
     * Returns the name of the field at position 'i'
     * @throws java.util.NoSuchElementException if no such element exists
     */
    public String getFieldName(int i) throws NoSuchElementException {
        if(i < 0 || i >= getNumFields())  {
            throw new NoSuchElementException();
        }
        return columns.get(i).getName();
    }

    /**
     * @return true if the field with the given name exists in the TupleDesc, else false
     */
    public boolean hasField(String name) {
        for(TupleDescItem item : columns) {
            if(item.getName().equals(name)) {
                return true;
            }
        }
        return false;
    }

    /**
     * Gets the type of the field at position 'i'
     * @throws java.util.NoSuchElementException if no such element exists
     */
    public Type getFieldType(int i) throws NoSuchElementException {
        if(i < 0 || i >= getNumFields())  {
            throw new NoSuchElementException();
        }
        return columns.get(i).getType();
    }

    /**
     * Gets the type of the field with the given name
     * @throws java.util.NoSuchElementException if no such element exists
     */
    public Type getFieldType(String name) throws NoSuchElementException {
        return getFieldType(getIndexFromName(name));
    }

    /**
     * Gets the position of the column with 'fieldName'
     * @throws java.util.NoSuchElementException if no such element exists
     */
    public int getIndexFromName(String fieldName) throws NoSuchElementException {
        for(int i = 0; i < columns.size(); i++) {
            String name = columns.get(i).getName();
            if(fieldName.equals(name)) {
                return i;
            }
        }
        throw new NoSuchElementException();
    }

    /**
     * Generates a new TupleDesc for an index on 'column'
     * @param column the column to index on
     * @return a new tuple desc describing the index
     */
    public TupleDesc generateIndex(String column) {
        int columnIndex = getIndexFromName(column);
        TupleDesc index = new TupleDesc();
        index.addInteger("_page");
        index.addInteger("_recordId");
        index.columns.add(this.columns.get(columnIndex));
        return index;
    }

    /**
     * Returns a new TupleDesc with only the given columns
     */
    public TupleDesc reduceIndex(String... columnNames) {
        TupleDesc reduced = new TupleDesc();
        for(String name : columnNames) {
            try {
                int i = getIndexFromName(name);
                reduced.columns.add(columns.get(i));
            }
            catch (NoSuchElementException e) {}
        }
        return reduced;
    }

    /**
     * Private Item Class to store the name => type pairs of the tuples in the database
     */
    private static class TupleDescItem {
        private Type type;
        private String name;

        public TupleDescItem(Type type, String name) {
            this.type = type;
            this.name = name;
        }

        public String getName() {
            return name;
        }

        public Type getType() {
            return type;
        }
    }

}
