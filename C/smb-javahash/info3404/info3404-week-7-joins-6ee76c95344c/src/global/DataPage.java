package global;

import disk.Page;
import disk.PageId;
import heap.Tuple;
import heap.TupleDesc;

/**
 * Abstract Data Page Class
 * Contains a pointer to the previous / next page
 * Stores the number of records currently on the page, and the name of the schema for it's file entries
 * Overridden by subclasses to implement different schema directories (standard vs. index)
 */
public abstract class DataPage extends Page {

    protected static final int PREV_PAGE_INDEX = 0;
    protected static final int NEXT_PAGE_INDEX = 4;
    protected static final int RECORD_COUNT_INDEX = 8;
    protected static final int SCHEMA_INDEX = 12;
    public static final int RECORD_START_INDEX = SCHEMA_INDEX + 2 + MAX_TABLE_NAME_LENGTH;

    /**
     * Initialise the page with the given schema
     */
    public void initialise(String schema) {
        PageId invalid = new PageId(INVALID_PAGE_ID);
        setPreviousPageId(invalid);
        setNextPageId(invalid);
        setRecordCount(0);
        setSchema(schema);
    }

    /**
     * Returns the PageId of the previous page
     */
    public PageId getPreviousPageId() {
        return new PageId(getIntegerValue(PREV_PAGE_INDEX));
    }

    /**
     * Sets the PageId of the previous page
     */
    public void setPreviousPageId(PageId previous) {
        setIntegerValue(previous.get(), PREV_PAGE_INDEX);
    }

    /**
     * Returns the PageId of the next page
     */
    public PageId getNextPageId() {
        return new PageId(getIntegerValue(NEXT_PAGE_INDEX));
    }

    /**
     * Sets the PageId of the next page
     * @param next
     */
    public void setNextPageId(PageId next) {
        setIntegerValue(next.get(), NEXT_PAGE_INDEX);
    }

    /**
     * Returns the number of records **currently** stored on the page
     * @return
     */
    public int getRecordCount() {
        return getIntegerValue(RECORD_COUNT_INDEX);
    }

    /**
     * Set the number of records currently stored on the page
     * @param count the number of records stored
     */
    private void setRecordCount(int count) {
        setIntegerValue(count, RECORD_COUNT_INDEX);
    }

    /**
     * Sets the name of the schema used by this page
     */
    public void setSchema(String name) {
        setStringValue(name, SCHEMA_INDEX);
    }

    /**
     * Returns the current schema used by the page
     */
    public abstract TupleDesc getSchema();

    /**
     * Insert a record into the next available slot on the record.
     * Increments the record count on insert
     * @param record the tuple to insert
     * @return true if the insert was successful, false if not
     */
    public boolean insertRecord(Tuple record) {
        int next = getRecordCount();
        if(next >= getMaxRecordsOnPage()) {
            return false;
        }
        insertRecord(next, record);
        setRecordCount(next+1);
        return true;
    }

    /**
     * Insert a record into the specified slot
     * @param slotNo which slot/row in the page to write to
     * @param record the tuple to insert
     */
    public void insertRecord(int slotNo, Tuple record) {
        if(!record.getSchema().equals(getSchema())) {
            throw new AssertionError("Schema of Page and Tuple don't match");
        }
        int offset = RECORD_START_INDEX + slotNo * getSchema().getTupleLength();
        write(record, offset);

    }

    /**
     * Reads the record at position slotNo from file into the given tuple
     * @param slotNo which slot/row in the page to read from
     * @param record the tuple to read the record into (must match the schema of the page)
     */
    public void getRecord(int slotNo, Tuple record) {
        if(!record.getSchema().equals(getSchema())) {
            throw new AssertionError("Schema of Page and Tuple don't match");
        }
        int offset = RECORD_START_INDEX + slotNo * getSchema().getTupleLength();
        read(record, offset);
        record.setTupleId(slotNo);
    }

    /**
     * Returns the **maximum** number of records that can be fit on the page.
     */
    public int getMaxRecordsOnPage() {
        return (PAGE_SIZE - RECORD_START_INDEX) / getSchema().getTupleLength();
    }

    /**
     * Low-level: Write the tuple to the given offset in the page.
     * Uses the schema to infer the types of data to write to file
     * @param tuple the tuple the write to disk
     * @param offset byte array offset for the tuple
     */
    private void write(Tuple tuple, int offset) {
        TupleDesc schema = getSchema();
        int length = schema.getNumFields();
        for(int i = 0; i < length; i++) {
            Type columnType = schema.getFieldType(i);
            switch(columnType) {
                case STRING:
                    setStringValue((String) tuple.getColumn(i), offset);
                    break;
                case INTEGER:
                    setIntegerValue((Integer) tuple.getColumn(i), offset);
                    break;
                case DOUBLE:
                    setDoubleValue((Double) tuple.getColumn(i), offset);
                    break;
                case BOOLEAN:
                    setBooleanValue((Boolean)tuple.getColumn(i), offset);
                    break;
                default:
                    throw new AssertionError("Invalid Column Type");
            }
            offset += columnType.getLen();
        }
    }

    /**
     * Low-level: Reads a tuple from the page starting at the offset
     * Uses the schema in the tuple to infer the types of it's arguments
     * @param tuple the tuple to read the row into
     * @param offset the start of the row in the page
     */
    private void read(Tuple tuple, int offset) {
        TupleDesc schema = getSchema();
        tuple.resetWithSchema(schema);
        int length = schema.getNumFields();
        for(int i = 0; i < length; i++) {
            Type columnType = schema.getFieldType(i);
            switch(columnType) {
                case STRING:
                    tuple.setColumn(i, getStringValue(offset));
                    break;
                case INTEGER:
                    tuple.setColumn(i, getIntegerValue(offset));
                    break;
                case DOUBLE:
                    tuple.setColumn(i, getDoubleValue(offset));
                    break;
                case BOOLEAN:
                    tuple.setColumn(i, getBooleanValue(offset));
                    break;
                default:
                    throw new AssertionError("Invalid Column Type");
            }
            offset += columnType.getLen();
        }
    }

}
