package global;

import heap.TupleDesc;

import java.util.HashMap;
import java.util.Map;

/**
 * Stores the Schemas for the Database
 * Currently just stores the Schema in-memory, does not write it to disk
 */
public class Catalog {

    private HashMap<String, TupleDesc> schemas;
    private HashMap<String, TupleDesc> indexes;

    public Catalog() {
        schemas = new HashMap<>();
        indexes = new HashMap<>();
    }

    /**
     * Stores the Schema definition in the Catalog
     * @param schema the description of the schema
     * @param name the name of the schema
     */
    public void createSchema(TupleDesc schema, String name) {
        if(!schemas.containsKey(name)) {
            schemas.put(name, schema);
        }
        else throw new IllegalStateException("Schema Already Exists");
    }

    /**
     * Returns the schema associated with the given name
     */
    public TupleDesc readSchema(String name) {
        return schemas.get(name);
    }

    /**
     * Creates an index with the given name from the schema given on the given column
     * @param indexName the name of the index
     * @param schemaName the name of the schema to create the index on
     * @param column the name of the column to index on
     */
    public void createIndex(String indexName, String schemaName, String column) {
        if(!schemas.containsKey(schemaName)) {
            throw new IllegalStateException("Schema does not exist");
        }
        TupleDesc schema = readSchema(schemaName);
        if(!schema.hasField(column)) {
            throw new AssertionError("No such column in the schema");
        }
        if(!indexes.containsKey(indexName)) {
            TupleDesc index = schema.generateIndex(column);
            indexes.put(indexName, index);
        }
        else throw new IllegalStateException("Index Already Exists");
    }

    /**
     * Returns the index's schema description (for index pages)
     */
    public TupleDesc readIndex(String indexName) {
        return indexes.get(indexName);
    }

    /**
     * Returns the name of the given schema (if any)
     */
    public String findNameOfSchema(TupleDesc schema) {
        for(Map.Entry<String, TupleDesc> entry : schemas.entrySet()) {
            if(schema == entry.getValue()) {
                return entry.getKey();
            }
        }
        return "_NO_SCHEMA_FOUND_";
    }

}
