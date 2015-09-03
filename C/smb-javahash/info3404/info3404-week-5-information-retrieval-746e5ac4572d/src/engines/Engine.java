package engines;

import core.DTMatrix;

import java.util.List;
import java.util.Map;

/**
 * Generic interface to define an engine
 */
public abstract class Engine {

    protected DTMatrix dtMatrix;

    /**
     * Executes a query and returns a map of the results
     */
    public abstract Map<String, Double> execute(String text);

    /**
     * Calculates the recall a result attempt
     * @param expected - the expected documents to be return
     * @param result - the actual documents that were returned
     * @return the recall percentage
     */
    public double calculateRecall(List<String> expected, Map<String, Double> result) {
		int R = expected.size();
		int fr = 0;

		for (String e : expected) {
			if (result.containsKey(e)) fr++;
		}

        return 1.0*fr/R;
    }

    /**
     * Calculates the precision of an engine execution
     * @param expected - the expected documents to be return
     * @param result - the actual documents that were returned
     * @return the precision percentage
     */
    public double calculatePrecision(List<String> expected, Map<String, Double> result) {
		int F = result.size();
		int fr = 0;

		for (String e : expected) {
			if (result.containsKey(e)) fr++;
		}

        return 1.0*fr/F;
    }

    /**
     * Calculates the fallout of an engine execution
     * @param expected - the expected documents to be return
     * @param result - the actual documents that were returned
     * @return the fallout percentage
     * Note: you can get the total documents in the Document-Term matrix with `dtMatrix.rowCount()`
     */
    public double calculateFallout(List<String> expected, Map<String, Double> result) {
		int A = dtMatrix.rowCount();
		int R = expected.size();
		int F = result.size();

		int fr = 0;

		for (String e : expected) {
			if (result.containsKey(e)) fr++;
		}

        return Math.abs(1.0*(F-fr)/(A-R));
    }
}
