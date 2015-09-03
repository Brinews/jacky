package engines;

import core.DTMatrix;
import core.Vector;

import java.util.HashMap;
import java.util.Map;
import java.util.Iterator;

/**
 * Boolean Retrieval Engine
 */
public class IRBooleanEngine extends Engine {

    private static final double CUT_OFF = 0.00;
    private Vector idf;

    /**
     * Initialise the IR Boolean Engine
     * Hint: You may wish to calculate the IDF vector and apply it to the matrix here. Look at dtMatrix.multiply(...)
     */
    public IRBooleanEngine(DTMatrix dtMatrix) {
        this.dtMatrix = dtMatrix;
    }

    /**
     * Returns the results of executing a query using the argument text
     * 1. Create the query vector
     * 2. Convert the query vector to it's tf-idf counterpart
     * 3. Calculate the similarity score between the query vector and each document in the matrix
     * 4. If the score is greater than CUT_OFF, add it to the results Map
     * @param text - the query to execute
     * @return a map of (document) => (score)
     */
    @Override
    public Map<String, Double> execute(String text) {
        Map<String, Double> results = new HashMap<String, Double>();

        /**
         * Hints: Look at dtMatrix.createQueryVector(...)
         *        Look at Vector.multiply
         */
		Vector qry = dtMatrix.createQueryVector(text);
		Vector idf = dtMatrix.getInverseDocumentFrequency();

		Vector idfQry = Vector.multiply(qry, idf);

		dtMatrix.multiply(idf);

		Iterator<Vector> rows = dtMatrix.rows();
		for(int i = 0; rows.hasNext(); i++) {
			Vector row = rows.next();

			double fz = 0.0, fm1 = 0.0, fm2 = 0.0;

			for (int j = 0; j < row.size(); j++) {
				fz += (idfQry.get(j) * row.get(j));
				fm1 += Math.pow(idfQry.get(j), 2.0);
				fm2 += Math.pow(row.get(j), 2.0);
			}

			double sim = fz;  //fz / (Math.sqrt(fm1 * fm2));
			if (sim > CUT_OFF) {
				results.put(dtMatrix.getDocumentNameFromIndex(i), sim);
			}
		}


        /**
         * To iterate over all the documents in the matrix:
            Iterator<Vector> rows = dtMatrix.rows();
            for(int i = 0; rows.hasNext(); i++) {
                Vector row = rows.next();
                /** Do something here
            }
         */

        return results;
    }
}
