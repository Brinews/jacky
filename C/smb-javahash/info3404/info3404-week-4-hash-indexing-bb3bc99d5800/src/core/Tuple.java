package core;

import java.util.Arrays;

/**
 * Simply Respresents a Row of Data
 * You should not need to modify this file
 */
public class Tuple implements Comparable<Tuple> {

    private int keyColumn;
    private String[] values;

    public Tuple(int column, String... values) {
        this.keyColumn = column;
        this.values = values;
    }

    /**
     * Returns the key index on. It is this key that then gets turned into a hash for comparison
     */
    public String getKey() {
        return values[keyColumn];
    }

    /**
     * Returns the hash of the key index for this tuple. Call this to use a tuple in getBucket(hash)
     */
    public int getKeyHash() {
        return Math.abs(getKey().hashCode());
    }

    public String[] getValues() {
        return values;
    }

    public int compareTo(String key) {
        return getKey().compareTo(key);
    }

    @Override
    public int compareTo(Tuple o) {
        return this.compareTo(o.getKey());
    }

    @Override
    public int hashCode() {
        int result = keyColumn;
        result = 31 * result + (values != null ? Arrays.hashCode(values) : 0);
        return result;
    }

    @Override
    public String toString() {
        int hash = getKeyHash();
        String hashAsBinary = Integer.toBinaryString(hash);
        return String.format("Hash: %d | Binary Hash: %s | Records: (%s)", hash, hashAsBinary, Arrays.toString(values));
    }
}
