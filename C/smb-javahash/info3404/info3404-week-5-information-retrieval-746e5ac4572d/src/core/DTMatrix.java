package core;

import tokeniser.Tokenizer;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

/**
 * Represents a Document-Term Matrix
 */
public class DTMatrix {

    private Map<String, Integer> documentName;
    private Map<String, Integer> columnNames;
    private Matrix matrix;
    private Tokenizer tokenizer;

    public DTMatrix(Tokenizer tokenizer) {
        documentName = new HashMap<String, Integer>();
        columnNames = new HashMap<String, Integer>();
        matrix = new Matrix();
        this.tokenizer = tokenizer;
    }

    /**
     * Adds a document to the matrix
     * 1. Checks that a document with a name doesn't already exist (throw DuplicateKeyException)
     * 2. Add a new row to the matrix
     * 3. Parse the tokens
     * 4. Increment the (row, column) associated with the token in the matrix
     *    This is just recording the frequency of each term for the document that you have added
     * @param name the name/identifier of the document (must be unique)
     * @param text the text string that comprises the document
     */
    public void addDocument(String name, String text) {
        if(documentName.containsKey(name)) throw new DuplicateKeyException();
        int row = matrix.addRow();
        // Add document name to matrix
        documentName.put(name, row);
        // Token text and add it to the matrix (we've already added the row, see above)
        // @TODO
		String[] tokens = tokenizer.parse(text);
		int column = 0;

		//System.out.println("\n");

		for (String token : tokens) {
			//System.out.println("[" + token + "]");
            if(columnNames.containsKey(token)) {
				column = columnNames.get(token);
				matrix.incrementValue(row, column);
			}
			else {
				/* add a column */
				column = columnNames.size();
				matrix.addColumn();

				columnNames.put(token, column);
				matrix.setValue(row, column, 1);
			}
		}
    }

    /**
     * Returns the document name/id associated with a row index
     */
    public String getDocumentNameFromIndex(int index) {
        for(Map.Entry<String, Integer> entry : documentName.entrySet()) {
            if(entry.getValue().equals(index)) {
                return entry.getKey();
            }
        }
        return "INVALID_DOCUMENT";
    }

    /**
     * Returns a vector of the inverse document frequency of each team.
     * Hint: use Math.log10(...) and look at getDocumentFrequency()
     */
    public Vector getInverseDocumentFrequency() {
		Vector idf = new Vector(matrix.numberOfColumns());
		int rowNum = matrix.numberOfRows();
		Vector nk = getDocumentFrequency();

		for (int i = 0; i < nk.size(); i++) {
			idf.set(i, Math.log10(1.0*rowNum/nk.get(i)));
		}

        return idf;
    }

    /**
     * Sums up the columns in the matrix
     */
    public Vector getDocumentFrequency() {
        Vector frequency = new Vector(matrix.numberOfColumns());
        Iterator<Vector> rows = matrix.iterator();
        // Iterates over documents
        while(rows.hasNext()) {
            Vector row = rows.next();
            // If document has a term at least once, increment the frequency
            for(int i = 0; i < row.size(); i++) {
                if(row.get(i) > 0) {
                    frequency.increment(i, 1);
                }
            }
        }
        return frequency;
    }

    /**
     * Multiplies the current matrix's row against the given vector
     */
    public void multiply(Vector other) {
        matrix.multiply(other);
    }

    /**
     * Converts a text query into a vector of term frequencies
     */
    public Vector createQueryVector(String text) {
        Vector query = new Vector(matrix.numberOfColumns());
        for(String token : tokenizer.parse(text)) {
            if(columnNames.containsKey(token)) {
                double value = query.get(columnNames.get(token));
                query.set(columnNames.get(token), value + 1);
            }
        }
        return query;
    }

    /**
     * Returns an iterator of the row vectors in the matrix
     * @return
     */
    public Iterator<Vector> rows() {
        return matrix.iterator();
    }

    /**
     * Returns the number of rows (or documents) in the matrix
     */
    public int rowCount() {
        return matrix.numberOfRows();
    }

    protected static final class DuplicateKeyException extends RuntimeException {}
}
