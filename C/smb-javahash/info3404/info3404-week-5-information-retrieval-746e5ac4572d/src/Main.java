import core.Constants;
import core.DTMatrix;
import core.Text;
import engines.Engine;
import engines.IRBooleanEngine;

import java.util.Arrays;
import java.util.Map;

/**
 * Example usage of the engine
 */
public class Main {

    public static void main(String[] args) {
        DTMatrix matrix = new DTMatrix(Constants.TOKENIZER);
        matrix.addDocument("t1", Text.HAMLET);
        matrix.addDocument("t2", Text.EINSTEIN);
        matrix.addDocument("t3", Text.JFK);
        matrix.addDocument("t4", Text.MLK);

        Engine engine = new IRBooleanEngine(matrix);
        Map<String, Double> results = engine.execute("sleep at night");
        System.out.println(Arrays.toString(results.entrySet().toArray()));
    }
}
