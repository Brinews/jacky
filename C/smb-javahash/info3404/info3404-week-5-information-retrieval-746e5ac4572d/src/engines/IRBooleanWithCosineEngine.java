package engines;

import core.DTMatrix;
import core.Vector;

import java.util.HashMap;
import java.util.Map;
import java.util.Iterator;

/**
 * Boolean Retrieval Engine
 */
public class IRBooleanWithCosineEngine extends Engine {

    private Vector idf;
    private static final double CUT_OFF = 0.05;

    /**
     * Initialise the IR Boolean Engine
     * Hint: You may wish to calculate the IDF vector and apply it to the matrix here. Look at dtMatrix.multiply(...)
     */
    public IRBooleanWithCosineEngine(DTMatrix dtMatrix) {
        this.dtMatrix = dtMatrix;
    }

    /**
     * Returns the results of executing a query using the argument text
     * 1. Create the query vector
     * 2. Convert the query vector to it's tf-idf counterpart
     * 3. Calculate the similarity score between the query vector and each document in the matrix
     * 4. Normalise this score
     * 4. If the score is greater than CUT_OFF, add it to the results Map
     * @param text - the query to execute
     * @return a map of (document) => (score)
     */
    @Override
    public Map<String, Double> execute(String text) {
        Map<String, Double> results = new HashMap<String, Double>();

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

			double sim = fz / (Math.sqrt(fm1 * fm2));
			if (sim > CUT_OFF) {
				results.put(dtMatrix.getDocumentNameFromIndex(i), sim);
			}
		}

        return results;
    }

    /**
     * Calculates the normalisation value from a vector
     * Formula = sqrt(\sum{vector[i]^2}), or see lecture slides
     */
    protected double calculateNormalisationScalar(Vector vector) {
		double r = 0.0;
		for (int i = 0; i < vector.size(); i++) 
			r += Math.pow(vector.get(i), 2.0);

        return Math.sqrt(r);
    }
}
